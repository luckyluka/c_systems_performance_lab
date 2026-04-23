#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>

typedef struct {
    uint32_t id;
    uint32_t payload;
} event_t;

void engine_run(void);

#endif
