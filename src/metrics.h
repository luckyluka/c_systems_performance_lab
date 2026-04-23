#ifndef METRICS_H
#define METRICS_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint64_t *latencies;
    size_t count;
    size_t capacity;
} metrics_t;

metrics_t* metrics_create(size_t initial_capacity);
void metrics_record(metrics_t *m, uint64_t latency_cycles);
void metrics_report(metrics_t *m);
void metrics_free(metrics_t *m);

#endif
