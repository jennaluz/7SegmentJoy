#include <FreeRTOS.h>
#include <stdio.h>
#include <task.h>
#include <queue.h>
#include <pico/stdlib.h>
#include <semphr.h>

/*private*/
#include "tasks.h"
#include "7SegmentPins.h"

QueueHandle_t qBlink = NULL;
QueueHandle_t qOnesCount = NULL;
QueueHandle_t qTensCount = NULL;

SemaphoreHandle_t m7SegmentDisplay = NULL;

int main()
{
    stdio_init_all();

    gpio_init(LED_PIN);

    gpio_init(SEVENSEG_CC1);
    gpio_init(SEVENSEG_CC2);

    gpio_init(SEVENSEG_A);
    gpio_init(SEVENSEG_B);
    gpio_init(SEVENSEG_C);
    gpio_init(SEVENSEG_D);
    gpio_init(SEVENSEG_E);
    gpio_init(SEVENSEG_F);
    gpio_init(SEVENSEG_G);
    gpio_init(SEVENSEG_DP);

    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_set_dir(SEVENSEG_CC1, GPIO_OUT);
    gpio_set_dir(SEVENSEG_CC2, GPIO_OUT);

    gpio_set_dir(SEVENSEG_A, GPIO_OUT);
    gpio_set_dir(SEVENSEG_B, GPIO_OUT);
    gpio_set_dir(SEVENSEG_C, GPIO_OUT);
    gpio_set_dir(SEVENSEG_D, GPIO_OUT);
    gpio_set_dir(SEVENSEG_E, GPIO_OUT);
    gpio_set_dir(SEVENSEG_F, GPIO_OUT);
    gpio_set_dir(SEVENSEG_G, GPIO_OUT);
    gpio_set_dir(SEVENSEG_DP, GPIO_OUT);

    gpio_put(SEVENSEG_CC1, 1);
    gpio_put(SEVENSEG_CC2, 1);

    qBlink = xQueueCreate(1, sizeof(bool));
    qOnesCount = xQueueCreate(1, sizeof(uint));
    qTensCount = xQueueCreate(1, sizeof(uint));

    m7SegmentDisplay = xSemaphoreCreateMutex();

    xTaskCreate(vCounter, "Counter", 256, NULL, 2, NULL);
    xTaskCreate(vBlinkD13, "Blink D13", 256, NULL, 1, NULL);
    xTaskCreate(vOnesDigit, "One's Digit", 256, NULL, 1, NULL);
    xTaskCreate(vTensDigit, "Ten's Digit", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true) {};
}
