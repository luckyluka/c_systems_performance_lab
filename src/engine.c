#include <stdio.h>
#include "engine.h"

void engine_run(void) {
    printf("Engine started (placeholder loop)\n");

    for (int i = 0; i < 5; i++) {
        printf("Processing event %d\n", i);
    }
}