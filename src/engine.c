#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "engine.h"
#include "replay.h"
#include "metrics.h"

#if defined(__x86_64__) || defined(__i386__)
static inline uint64_t rdtsc(void) {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}
#else
#warning "rdtsc not supported on this architecture, falling back to clock_gettime (lower precision)"
static inline uint64_t rdtsc(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}
#endif

static inline long long now_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

/* ---------------------------
 * Deterministic workload
 * --------------------------- */
static inline uint32_t process_event(event_t *ev) {
    uint32_t x = ev->payload;

    for (int j = 0; j < 8; j++) {
        x = x * 1664525 + 1013904223;
        x ^= (x >> 13);
        x += (x << 5);
    }

    return x;
}

#define RUNS 5

/* ---------------------------
 * Benchmark runner
 * --------------------------- */
static void run_benchmark(event_t *events, size_t count, long long *out_time_ns, metrics_t *m) {

    volatile uint32_t sink = 0;

    long long start_ns = now_ns();

    for (size_t i = 0; i < count; i++) {
        uint64_t start_cycles = rdtsc();
        sink += process_event(&events[i]);
        uint64_t end_cycles = rdtsc();

        if (m) {
            metrics_record(m, end_cycles - start_cycles);
        }
    }

    long long end_ns = now_ns();

    *out_time_ns = end_ns - start_ns;

    if (sink == 42) {
        printf("impossible\n");
    }
}

void engine_run(void) {

    printf("C Systems Performance Lab - Phase 1 (REPLAY + RDTSC)\n");
    printf("Engine started\n");

    event_t *events = NULL;
    size_t count = 0;

    if (replay_load("events.bin", &events, &count) != 0) {
        fprintf(stderr, "Failed to load events.bin. Run 'make gen_events && ./gen_events events.bin 1000000' first.\n");
        return;
    }

    /* ---------------------------
     * Warmup (important for CPU cache + branch predictor)
     * --------------------------- */
    for (int i = 0; i < 2; i++) {
        long long tmp;
        run_benchmark(events, count, &tmp, NULL);
    }

    /* ---------------------------
     * Measured runs
     * --------------------------- */
    long long times[RUNS];
    metrics_t *m = metrics_create(count);

    for (int i = 0; i < RUNS; i++) {
        // We only collect per-event metrics for the last run to avoid excessive memory usage if RUNS was large,
        // but here we can just do it for one run or all. Let's do it for the last run to have a clean report.
        metrics_t *m_run = (i == RUNS - 1) ? m : NULL;
        run_benchmark(events, count, &times[i], m_run);
    }

    /* ---------------------------
     * Stats: min / max / avg
     * --------------------------- */
    long long min_ns = times[0];
    long long max_ns = times[0];
    long long sum_ns = 0;

    for (int i = 0; i < RUNS; i++) {
        if (times[i] < min_ns) min_ns = times[i];
        if (times[i] > max_ns) max_ns = times[i];
        sum_ns += times[i];
    }

    double avg_ns = (double)sum_ns / RUNS;
    double avg_per_event_ns = avg_ns / count;
    double throughput = (double)count / avg_ns * 1000.0;

    printf("\n--- Phase 1 Results ---\n");
    printf("Runs: %d (N=%zu events each)\n", RUNS, count);

    printf("Min time: %lld ns\n", min_ns);
    printf("Max time: %lld ns\n", max_ns);
    printf("Avg time: %.2f ns\n", avg_ns);

    printf("\nAvg per event: %.2f ns\n", avg_per_event_ns);
    printf("Throughput: %.2f M events/sec\n", throughput);

    metrics_report(m);

    metrics_free(m);
    replay_free(events);
}
