#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "engine.h"

static inline long long now_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

/*
 * Simple deterministic “work” function.
 * Designed to:
 * - prevent compiler optimization
 * - create consistent CPU load
 */
static inline int process_event(int i) {
    volatile int x = i;

    // pseudo-random mix of arithmetic + bit ops
    for (int j = 0; j < 8; j++) {
        x = x * 1664525 + 1013904223;
        x ^= (x >> 13);
        x += (x << 5);
    }

    return x;
}

void engine_run(void) {
    printf("Engine started\n");

    const int N = 1000000;

    long long start = now_ns();

    volatile int sink = 0; // prevents dead-code elimination

    for (int i = 0; i < N; i++) {
        sink += process_event(i);
    }

    long long end = now_ns();

    long long duration = end - start;

    printf("Processed %d events\n", N);
    printf("Total time: %lld ns\n", duration);
    printf("Avg per event: %lld ns\n", duration / N);

    // use sink so compiler cannot remove loop
    if (sink == 42) {
        printf("impossible\n");
    }
}