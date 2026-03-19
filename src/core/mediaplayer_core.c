#include "core/mediaplayer_core.h"

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MediaPlayerState *mediaplayer_state_new(const char *directory) {
    MediaPlayerState *state = g_new0(MediaPlayerState, 1);
    state->working_dir = g_strdup(directory);
    state->image_files = NULL;
    return state;
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
