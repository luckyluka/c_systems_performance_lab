#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../src/engine.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <output_file> <num_events>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int n = atoi(argv[2]);

    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    event_t *events = malloc(n * sizeof(event_t));
    for (int i = 0; i < n; i++) {
        events[i].id = i;
        events[i].payload = i * 1103515245 + 12345;
    }
    fwrite(events, sizeof(event_t), n, f);
    free(events);

    fclose(f);
    printf("Generated %d events to %s\n", n, filename);

    return 0;
}
