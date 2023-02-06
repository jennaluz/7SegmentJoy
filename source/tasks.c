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
#include <semphr.h>

/*private*/
#include "tasks.h"
#include "7SegmentPins.h"
#include "digitConfig.h"

extern QueueHandle_t qBlink;
extern QueueHandle_t qOnesCount;
extern QueueHandle_t qTensCount;

extern SemaphoreHandle_t m7SegmentDisplay;

void vCounter()
{
    const int xDelayHalfSecond = 0.5 * configTICK_RATE_HZ;
    TickType_t xTicksToWait = 0;

    bool bBlink = true;
    uint uiCount = 0;
    uint uiOnes = 0;
    uint uiTens = 0;

    while (true) {
        for (uiCount = 42; uiCount > 0; uiCount--) {
            uiOnes = uiCount % 10;
            uiTens = uiCount / 10;

            //printf("%d %d %d\n", uiCount, uiTens, uiOnes);
            xQueueSend(qBlink, &bBlink, xTicksToWait);
            xQueueSend(qOnesCount, &uiOnes, xTicksToWait);
            xQueueSend(qTensCount, &uiTens, xTicksToWait);

            vTaskDelay(xDelayHalfSecond);
        }

        for (uiCount = 0; uiCount < 42; uiCount++) {
            uiOnes = uiCount % 10;
            uiTens = uiCount / 10;

            //printf("%d %d %d\n", uiCount, uiTens, uiOnes);
            xQueueSend(qOnesCount, &uiOnes, xTicksToWait);
            xQueueSend(qTensCount, &uiTens, xTicksToWait);
            xQueueSend(qBlink, &bBlink, xTicksToWait);

            vTaskDelay(xDelayHalfSecond);
        }
    }
}

void vBlinkD13()
{
    const int xBlinkDelay = 0.3 * configTICK_RATE_HZ;
    bool bBlink;

    while (true) {
        xQueueReceive(qBlink, &bBlink, 0);

        if (bBlink) {
            gpio_put(LED_PIN, 1);
            vTaskDelay(xBlinkDelay);
            gpio_put(LED_PIN, 0);

            bBlink = false;
        }

        vTaskDelay(0);
    }
}

void vOnesDigit()
{
    uint uiDigit = 2;

    while (true) {
            // take m7SegmentDisplay mutex
        if (xSemaphoreTake(m7SegmentDisplay, 0) == pdTRUE) {
            xQueueReceive(qOnesCount, &uiDigit, 0);

            gpio_put(SEVENSEG_CC1, 1);

            gpio_put(SEVENSEG_A, 0);
            gpio_put(SEVENSEG_B, 0);
            gpio_put(SEVENSEG_C, 0);
            gpio_put(SEVENSEG_D, 0);
            gpio_put(SEVENSEG_E, 0);
            gpio_put(SEVENSEG_F, 0);
            gpio_put(SEVENSEG_G, 0);

            switch (uiDigit) {
                case 0:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_C, 1);
                    gpio_put(SEVENSEG_D, 1);
                    gpio_put(SEVENSEG_E, 1);
                    gpio_put(SEVENSEG_F, 1);
                    break;
                case 1:
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_C, 1);
                    break;
                case 2:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_G, 1);
                    gpio_put(SEVENSEG_E, 1);
                    gpio_put(SEVENSEG_D, 1);
                    break;
                case 3:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_G, 1);
                    gpio_put(SEVENSEG_C, 1);
                    gpio_put(SEVENSEG_D, 1);
                    break;
                case 4:
                    gpio_put(SEVENSEG_F, 1);
                    gpio_put(SEVENSEG_G, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_C, 1);
                    break;
                case 5:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_F, 1);
                    gpio_put(SEVENSEG_G, 1);
                    gpio_put(SEVENSEG_C, 1);
                    gpio_put(SEVENSEG_D, 1);
                    break;
                case 6:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_F, 1);
                    gpio_put(SEVENSEG_E, 1);
                    gpio_put(SEVENSEG_D, 1);
                    gpio_put(SEVENSEG_C, 1);
                    gpio_put(SEVENSEG_G, 1);
                    break;
                case 7:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_C, 1);
                    break;
                case 8:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_C, 1);
                    gpio_put(SEVENSEG_D, 1);
                    gpio_put(SEVENSEG_E, 1);
                    gpio_put(SEVENSEG_F, 1);
                    gpio_put(SEVENSEG_G, 1);
                    break;
                case 9:
                    gpio_put(SEVENSEG_G, 1);
                    gpio_put(SEVENSEG_F, 1);
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_C, 1);
                    break;
            }

            gpio_put(SEVENSEG_CC2, 0);
            xSemaphoreGive(m7SegmentDisplay);
        }

        vTaskDelay(0.5);
    }
}

void vTensDigit()
{
    uint uiDigit = 4;

    while (true) {
        if (xSemaphoreTake(m7SegmentDisplay, 0) == pdTRUE) {
            xQueueReceive(qTensCount, &uiDigit, 0);

            gpio_put(SEVENSEG_CC2, 1);

            gpio_put(SEVENSEG_A, 0);
            gpio_put(SEVENSEG_B, 0);
            gpio_put(SEVENSEG_C, 0);
            gpio_put(SEVENSEG_D, 0);
            gpio_put(SEVENSEG_E, 0);
            gpio_put(SEVENSEG_F, 0);
            gpio_put(SEVENSEG_G, 0);

            switch (uiDigit) {
                case 0:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_C, 1);
                    gpio_put(SEVENSEG_D, 1);
                    gpio_put(SEVENSEG_E, 1);
                    gpio_put(SEVENSEG_F, 1);
                    break;
                case 1:
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_C, 1);
                    break;
                case 2:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_G, 1);
                    gpio_put(SEVENSEG_E, 1);
                    gpio_put(SEVENSEG_D, 1);
                    break;
                case 3:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_G, 1);
                    gpio_put(SEVENSEG_C, 1);
                    gpio_put(SEVENSEG_D, 1);
                    break;
                case 4:
                    gpio_put(SEVENSEG_F, 1);
                    gpio_put(SEVENSEG_G, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_C, 1);
                    break;
                case 5:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_F, 1);
                    gpio_put(SEVENSEG_G, 1);
                    gpio_put(SEVENSEG_C, 1);
                    gpio_put(SEVENSEG_D, 1);
                    break;
                case 6:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_F, 1);
                    gpio_put(SEVENSEG_E, 1);
                    gpio_put(SEVENSEG_D, 1);
                    gpio_put(SEVENSEG_C, 1);
                    gpio_put(SEVENSEG_G, 1);
                    break;
                case 7:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_C, 1);
                    break;
                case 8:
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_C, 1);
                    gpio_put(SEVENSEG_D, 1);
                    gpio_put(SEVENSEG_E, 1);
                    gpio_put(SEVENSEG_F, 1);
                    gpio_put(SEVENSEG_G, 1);
                    break;
                case 9:
                    gpio_put(SEVENSEG_G, 1);
                    gpio_put(SEVENSEG_F, 1);
                    gpio_put(SEVENSEG_A, 1);
                    gpio_put(SEVENSEG_B, 1);
                    gpio_put(SEVENSEG_C, 1);
                    break;
            }

            gpio_put(SEVENSEG_CC1, 0);
            xSemaphoreGive(m7SegmentDisplay);
        }

        vTaskDelay(0.5);
    }
}
