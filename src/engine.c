#include <stdio.h>
#include <time.h>
#include "engine.h"

static long long now_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

void engine_run(void) {
    printf("Engine started\n");

    long long start = now_ns();

    for (int i = 0; i < 100000; i++) {
        // simulate event processing
        volatile int x = i * 2;
        (void)x;
    }

    long long end = now_ns();

    long long duration = end - start;

    printf("Processed 100000 events\n");
    printf("Total time: %lld ns\n", duration);
    printf("Avg per event: %lld ns\n", duration / 100000);
}