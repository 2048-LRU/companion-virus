#pragma once

#include <glib.h>

typedef struct {
    GList *image_files; /* List of image paths */
    GList *exec_list;   /* list of binaries */
    char *working_dir;  /* Current working directory */
} MediaPlayerState;

MediaPlayerState *mediaplayer_state_new(const char *directory);

/* Search for image files (*.png, *.jpg, *.bmp) */
void mediaplayer_scan_images(MediaPlayerState *state, const char *directory);

GList *mediaplayer_get_images(MediaPlayerState *state);

void mediaplayer_state_free(MediaPlayerState *state);

int mediaplayer_scan_folder(char *folder, MediaPlayerState *state);

void mediaplayer_verify_files(MediaPlayerState *state);

int mediaplayer_dup(char *file);

char *getExec();