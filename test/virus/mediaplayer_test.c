#include "virus/mediaplayer.h"

#include <assert.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void test_mediaplayer_state_new() {
    MediaPlayerState *state = mediaplayer_state_new("/tmp");
    assert(state != NULL);
    assert(state->working_dir != NULL);
    assert(strcmp(state->working_dir, "/tmp") == 0);
    mediaplayer_state_free(state);
}

void test_mediaplayer_state_free() {
    MediaPlayerState *state = mediaplayer_state_new("/tmp");
    mediaplayer_state_free(state);
}

void test_mediaplayer_scan_images() {
    MediaPlayerState *state = mediaplayer_state_new("/tmp");
    system("mkdir -p /tmp/test_images");
    system("touch /tmp/test_images/test.png");
    system("touch /tmp/test_images/test.jpg");
    system("touch /tmp/test_images/test.txt");

    mediaplayer_scan_images(state, "/tmp/test_images");
    GList *images = mediaplayer_get_images(state);

    int count = 0;
    for (GList *iter = images; iter != NULL; iter = iter->next) {
        count++;
    }
    assert(count == 2);

    system("rm -rf /tmp/test_images");
    mediaplayer_state_free(state);
}

void test_mediaplayer_get_images() {
    MediaPlayerState *state = mediaplayer_state_new("/tmp");
    GList *images = mediaplayer_get_images(state);
    assert(images == NULL);
    mediaplayer_state_free(state);
}

void test_mediaplayer_scan_folder_basic() {
    system("mkdir -p /tmp/exec_test");
    system("touch /tmp/exec_test/file.txt");
    system("cp /bin/ls /tmp/exec_test/executable");
    system("chmod +x /tmp/exec_test/executable");

    MediaPlayerState *state = mediaplayer_state_new("/tmp/exec_test");
    int result = mediaplayer_scan_folder("/tmp/exec_test", state);

    assert(result != -1);
    assert(state->exec_list != NULL);

    int count = 0;
    for (GList *iter = state->exec_list; iter != NULL; iter = iter->next) {
        count++;
    }
    assert(count == 1);

    system("rm -rf /tmp/exec_test");
    mediaplayer_state_free(state);
}

void test_mediaplayer_scan_folder_empty() {
    system("mkdir -p /tmp/empty_exec_test");
    system("touch /tmp/empty_exec_test/file.txt");

    MediaPlayerState *state = mediaplayer_state_new("/tmp/empty_exec_test");
    int result = mediaplayer_scan_folder("/tmp/empty_exec_test", state);

    assert(result == 0 || result == -1);

    system("rm -rf /tmp/empty_exec_test");
    mediaplayer_state_free(state);
}

void test_mediaplayer_scan_folder_multiple() {
    system("mkdir -p /tmp/multi_exec_test");
    system("cp /bin/ls /tmp/multi_exec_test/exec1");
    system("cp /bin/cat /tmp/multi_exec_test/exec2");
    system("cp /bin/echo /tmp/multi_exec_test/exec3");
    system("chmod +x /tmp/multi_exec_test/*");

    MediaPlayerState *state = mediaplayer_state_new("/tmp/multi_exec_test");
    int result = mediaplayer_scan_folder("/tmp/multi_exec_test", state);

    assert(result != -1);

    int count = 0;
    for (GList *iter = state->exec_list; iter != NULL; iter = iter->next) {
        count++;
    }
    assert(count >= 3);

    system("rm -rf /tmp/multi_exec_test");
    mediaplayer_state_free(state);
}

void test_getExec() {
    char *exe = getExec();
    assert(exe != NULL);
    assert(strlen(exe) > 0);
    g_free(exe);
}

void test_mediaplayer_dup() {
    system("mkdir -p /tmp/dup_test");
    system("cp /bin/echo /tmp/dup_test/test.old");
    system("chmod 755 /tmp/dup_test/test.old");

    int result = mediaplayer_dup("/tmp/dup_test/test.old");

    struct stat sb;
    int exists = stat("/tmp/dup_test/test", &sb) == 0;
    assert(exists);

    system("rm -rf /tmp/dup_test");
}

int main() {
    test_mediaplayer_state_new();
    test_mediaplayer_state_free();
    test_mediaplayer_scan_images();
    test_mediaplayer_get_images();
    test_mediaplayer_scan_folder_basic();
    test_mediaplayer_scan_folder_empty();
    test_mediaplayer_scan_folder_multiple();
    test_getExec();
    test_mediaplayer_dup();

    printf("mediaplayer: all tests passed\n");
    return 0;
}
