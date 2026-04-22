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
 * Deterministic CPU workload.
 * Purpose: generate stable measurable compute cost.
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
#define BUCKETS 1000

static long long latencies[N];

void engine_run(void) {
    printf("Engine started\n");

    long long total_start = now_ns();

    volatile int sink = 0;

    /* ---------------------------
     * Measure per-event latency
     * --------------------------- */
    for (int i = 0; i < N; i++) {

        long long start = now_ns();

        sink += process_event(i);

        long long end = now_ns();

        latencies[i] = end - start;
    }

    long long total_end = now_ns();
    long long total_time = total_end - total_start;

    /* ---------------------------
     * Histogram construction
     * --------------------------- */
    static long long histogram[BUCKETS] = {0};

    long long max_latency = 0;

    for (int i = 0; i < N; i++) {
        if (latencies[i] > max_latency)
            max_latency = latencies[i];
    }

    for (int i = 0; i < N; i++) {
        long long v = latencies[i];

        int bucket = (int)((v * BUCKETS) / (max_latency + 1));
        if (bucket >= BUCKETS)
            bucket = BUCKETS - 1;

        histogram[bucket]++;
    }

    /* ---------------------------
     * Percentile estimation
     * --------------------------- */
    long long count = 0;
    int p50_bucket = 0;
    int p99_bucket = 0;

    for (int i = 0; i < BUCKETS; i++) {
        count += histogram[i];

        if (p50_bucket == 0 && count >= N * 0.50)
            p50_bucket = i;

        if (p99_bucket == 0 && count >= N * 0.99)
            p99_bucket = i;
    }

    /* ---------------------------
     * Output
     * --------------------------- */
    printf("Processed %d events\n", N);
    printf("Total time: %lld ns\n", total_time);
    printf("Throughput: %.2f M events/sec\n",
           (double)N / total_time * 1000.0);

    printf("\nLatency stats (histogram-based):\n");
    printf("p50 bucket: %d\n", p50_bucket);
    printf("p99 bucket: %d\n", p99_bucket);
    printf("max observed: %lld ns\n", max_latency);

    if (sink == 42) {
        printf("impossible\n");
    }
}