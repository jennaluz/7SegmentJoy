/*
 * source/tasks.c
 *
 * defines all tasks created in source/main.c.
 */

#include <FreeRTOS.h>
#include <stdio.h>
#include <task.h>
#include <queue.h>
#include <pico/stdlib.h>

/*private*/
#include "tasks.h"

void vCounter()
{
    const int xDelayHalfSecond = 0.5 * configTICK_RATE_HZ;
    uint uiCount = 0;

    while (true) {
        for (uiCount = 42; uiCount > 0; uiCount--) {
            printf("%d\n", uiCount);
            vTaskDelay(xDelayHalfSecond);
        }

        for (uiCount = 0; uiCount < 42; uiCount++) {
            printf("%d\n", uiCount);
            vTaskDelay(xDelayHalfSecond);
        }
    }
}
