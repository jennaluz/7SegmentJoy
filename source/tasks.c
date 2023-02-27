/*
 * source/tasks.c
 *
 * Defines all tasks created in source/main.c.
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

extern QueueHandle_t qBlink;
extern QueueHandle_t qOnesCount;
extern QueueHandle_t qTensCount;

extern SemaphoreHandle_t s7SegmentDisplay;

/*
 * Stores the current count value.
 * Sends the one's and ten's place over queues to be received by the appropriate task.
 * Signals when D13 should blink.
 */
void vCounter()
{
    bool bBlink = true;
    uint uiCount = 0;
    uint uiOnes = 0;
    uint uiTens = 0;

    while (true) {
        // count down from 42 to 00
        for (uiCount = 42; uiCount > 0; uiCount--) {
            // store uiCount place values
            uiOnes = uiCount % 10;
            uiTens = uiCount / 10;

            // send data to queues
            xQueueSend(qBlink, &bBlink, 0);
            xQueueSend(qOnesCount, &uiOnes, 0);
            xQueueSend(qTensCount, &uiTens, 0);

            // delay half a second
            vTaskDelay(0.5 * configTICK_RATE_HZ);
        }

        // count up from 42 to 00
        for (uiCount = 0; uiCount < 42; uiCount++) {
            // store uiCount place values
            uiOnes = uiCount % 10;
            uiTens = uiCount / 10;

            // send data to queues
            xQueueSend(qOnesCount, &uiOnes, 0);
            xQueueSend(qTensCount, &uiTens, 0);
            xQueueSend(qBlink, &bBlink, 0);

            // delay half a second
            vTaskDelay(0.5 * configTICK_RATE_HZ);
        }
    }
}

/*
 * Waits for signal over qBlink to blink D13 LED.
 */
void vBlinkD13()
{
    bool bBlink = false;

    while (true) {
        // receive data from queue
        xQueueReceive(qBlink, &bBlink, 0);

        // blink D13 LED
        if (bBlink) {
            gpio_put(LED_PIN, 1);
            vTaskDelay(0.3 * configTICK_RATE_HZ);
            gpio_put(LED_PIN, 0);

            bBlink = false;
        }

        // give up processor time
        taskYIELD();
    }
}

/*
 * Uses s7SegmentDisplay to displays the one's place of the current count.
 * Receives data over qOnesPlace.
 */
void vOnesDigit()
{
    uint uiDigit = 2;

    while (true) {
        // take s7SegmentDisplay mutex
        if (xSemaphoreTake(s7SegmentDisplay, 0) == pdTRUE) {
            // receive data from queue
            xQueueReceive(qOnesCount, &uiDigit, 0);

            // turn off tens digit display
            gpio_put(SEVENSEG_CC1, 1);

            // reset all segments
            gpio_put(SEVENSEG_A, 0);
            gpio_put(SEVENSEG_B, 0);
            gpio_put(SEVENSEG_C, 0);
            gpio_put(SEVENSEG_D, 0);
            gpio_put(SEVENSEG_E, 0);
            gpio_put(SEVENSEG_F, 0);
            gpio_put(SEVENSEG_G, 0);

            // config digit segments
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

            // turn on ones digit display
            gpio_put(SEVENSEG_CC2, 0);
            xSemaphoreGive(s7SegmentDisplay);
        }

        // give up processor time
        taskYIELD();
    }
}

/*
 * Uses s7SegmentDisplay to displays the ten's place of the current count.
 * Receives data over qTensPlace.
 */
void vTensDigit()
{
    uint uiDigit = 4;

    while (true) {
        // take s7SegmentDisplay
        if (xSemaphoreTake(s7SegmentDisplay, 0) == pdTRUE) {
            // receive data from queue
            xQueueReceive(qTensCount, &uiDigit, 0);

            // turn off ones digit display
            gpio_put(SEVENSEG_CC2, 1);

            // reset all segments
            gpio_put(SEVENSEG_A, 0);
            gpio_put(SEVENSEG_B, 0);
            gpio_put(SEVENSEG_C, 0);
            gpio_put(SEVENSEG_D, 0);
            gpio_put(SEVENSEG_E, 0);
            gpio_put(SEVENSEG_F, 0);
            gpio_put(SEVENSEG_G, 0);

            // config digit segments
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

            // turn on tens digit display
            gpio_put(SEVENSEG_CC1, 0);
            xSemaphoreGive(s7SegmentDisplay);
        }

        // give up processor time
        taskYIELD();
    }
}
