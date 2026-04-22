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
 * Deterministic workload (stable CPU cost)
 * IMPORTANT: no timing inside here
 */
static inline int process_event(int i) {
    volatile int x = i;

    for (int j = 0; j < 8; j++) {
        x = x * 1664525 + 1013904223;
        x ^= (x >> 13);
        x += (x << 5);
    }

    return x;
}

#define N 1000000

void engine_run(void) {
    printf("Engine started (Phase 1 - batch benchmark)\n");

    volatile int sink = 0;

    /* ---------------------------
     * PURE batch timing section
     * --------------------------- */
    long long start = now_ns();

    for (int i = 0; i < N; i++) {
        sink += process_event(i);
    }

    long long end = now_ns();

    long long total_time = end - start;

    /* ---------------------------
     * Core metrics
     * --------------------------- */
    double avg_ns = (double)total_time / (double)N;
    double throughput_m = (double)N / (double)total_time * 1000.0;

    printf("Processed %d events\n", N);
    printf("Total time: %lld ns\n", total_time);
    printf("Avg per event: %.2f ns\n", avg_ns);
    printf("Throughput: %.2f M events/sec\n", throughput_m);

    /* ---------------------------
     * Optional sanity check
     * --------------------------- */
    if (sink == 42) {
        printf("impossible\n");
    }
}