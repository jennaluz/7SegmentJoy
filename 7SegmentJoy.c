#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "pico/stdlib.h"

void vCounter();

int main()
{
    stdio_init_all();

    xTaskCreate(vCounter, "Counter", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1){};
}

void vCounter()
{
    const float xDelayHalfSecond = 0.5 * configTICK_RATE_HZ;
    int iCount = 0;

    while (true) {
        for (iCount = 42; iCount > 0; iCount--) {
            printf("%d\n", iCount);
            vTaskDelay(xDelayHalfSecond);
        }

        for (iCount = 0; iCount <= 42; iCount++) {
            printf("%d\n", iCount);
            vTaskDelay(xDelayHalfSecond);
        }
    }
}
