#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "engine.h"

static inline long long now_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

/* ---------------------------
 * Deterministic workload
 * --------------------------- */
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
#define RUNS 5

/* ---------------------------
 * Benchmark runner
 * --------------------------- */
static void run_benchmark(long long *out_time) {

    volatile int sink = 0;

    long long start = now_ns();

    for (int i = 0; i < N; i++) {
        sink += process_event(i);
    }

    long long end = now_ns();

    *out_time = end - start;

    if (sink == 42) {
        printf("impossible\n");
    }
}

void engine_run(void) {

    printf("C Systems Performance Lab - Phase 1 (COMPLETE)\n");
    printf("Engine started\n");

    /* ---------------------------
     * Warmup (important for CPU cache + branch predictor)
     * --------------------------- */
    for (int i = 0; i < 2; i++) {
        long long tmp;
        run_benchmark(&tmp);
    }

    /* ---------------------------
     * Measured runs
     * --------------------------- */
    long long times[RUNS];

    for (int i = 0; i < RUNS; i++) {
        run_benchmark(&times[i]);
    }

    /* ---------------------------
     * Stats: min / max / avg
     * --------------------------- */
    long long min = times[0];
    long long max = times[0];
    long long sum = 0;

    for (int i = 0; i < RUNS; i++) {
        if (times[i] < min) min = times[i];
        if (times[i] > max) max = times[i];
        sum += times[i];
    }

    double avg = (double)sum / RUNS;
    double avg_per_event = avg / N;
    double throughput = (double)N / avg * 1000.0;

    printf("\n--- Phase 1 Results ---\n");
    printf("Runs: %d (N=%d events each)\n", RUNS, N);

    printf("Min time: %lld ns\n", min);
    printf("Max time: %lld ns\n", max);
    printf("Avg time: %.2f ns\n", avg);

    printf("\nAvg per event: %.2f ns\n", avg_per_event);
    printf("Throughput: %.2f M events/sec\n", throughput);
}