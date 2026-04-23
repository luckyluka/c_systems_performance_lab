#ifndef REPLAY_H
#define REPLAY_H

#include "engine.h"
#include <stddef.h>

int replay_load(const char *filename, event_t **events, size_t *count);
void replay_free(event_t *events);

#endif
