#include <stdio.h>
#include <stdlib.h>
#include "replay.h"

int replay_load(const char *filename, event_t **events, size_t *count) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("replay_load: fopen");
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (size % sizeof(event_t) != 0) {
        fprintf(stderr, "replay_load: invalid file size\n");
        fclose(f);
        return -1;
    }

    *count = size / sizeof(event_t);
    *events = malloc(size);
    if (!*events) {
        perror("replay_load: malloc");
        fclose(f);
        return -1;
    }

    if (fread(*events, sizeof(event_t), *count, f) != *count) {
        perror("replay_load: fread");
        free(*events);
        fclose(f);
        return -1;
    }

    fclose(f);
    return 0;
}

void replay_free(event_t *events) {
    free(events);
}
