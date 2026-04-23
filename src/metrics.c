#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "metrics.h"

static int compare_uint64(const void *a, const void *b) {
    uint64_t arg1 = *(const uint64_t*)a;
    uint64_t arg2 = *(const uint64_t*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

metrics_t* metrics_create(size_t initial_capacity) {
    metrics_t *m = malloc(sizeof(metrics_t));
    if (!m) return NULL;
    m->latencies = malloc(initial_capacity * sizeof(uint64_t));
    if (!m->latencies) {
        free(m);
        return NULL;
    }
    m->count = 0;
    m->capacity = initial_capacity;
    return m;
}

void metrics_record(metrics_t *m, uint64_t latency_cycles) {
    if (m->count >= m->capacity) {
        return; // For Phase 1 we expect N events to match capacity
    }
    m->latencies[m->count++] = latency_cycles;
}

void metrics_report(metrics_t *m) {
    if (m->count == 0) return;

    qsort(m->latencies, m->count, sizeof(uint64_t), compare_uint64);

    uint64_t min = m->latencies[0];
    uint64_t max = m->latencies[m->count - 1];
    uint64_t p50 = m->latencies[m->count / 2];
    uint64_t p99 = m->latencies[m->count * 99 / 100];

    printf("\n--- Latency Histogram (Cycles) ---\n");
    printf("Min: %lu\n", min);
    printf("P50: %lu\n", p50);
    printf("P99: %lu\n", p99);
    printf("Max: %lu\n", max);
}

void metrics_free(metrics_t *m) {
    if (m) {
        free(m->latencies);
        free(m);
    }
}
