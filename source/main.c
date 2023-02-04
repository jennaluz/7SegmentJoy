#include <FreeRTOS.h>
#include <stdio.h>
#include <task.h>
#include <queue.h>
#include <pico/stdlib.h>

/*private*/
#include "tasks.h"

int main()
{
    stdio_init_all();

    xTaskCreate(vCounter, "Counter", 256, NULL, 3, NULL);

    vTaskStartScheduler();

    while (true) {};
}
