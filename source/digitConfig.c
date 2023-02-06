/*
 * source/digitConfig.c
 */

#include <FreeRTOS.h>
#include <pico/stdlib.h>

#include "digitConfig.h"
#include "7SegmentPins.h"

void vSet0()
{
    gpio_put(SEVENSEG_A, 1);
    gpio_put(SEVENSEG_B, 1);
    gpio_put(SEVENSEG_C, 1);
    gpio_put(SEVENSEG_D, 1);
    gpio_put(SEVENSEG_E, 1);
    gpio_put(SEVENSEG_F, 1);
}

void vSet1()
{
    gpio_put(SEVENSEG_B, 1);
    gpio_put(SEVENSEG_C, 1);
}

void vSet2()
{
    gpio_put(SEVENSEG_A, 1);
    gpio_put(SEVENSEG_B, 1);
    gpio_put(SEVENSEG_G, 1);
    gpio_put(SEVENSEG_E, 1);
    gpio_put(SEVENSEG_D, 1);
}

void vSet3()
{
    gpio_put(SEVENSEG_A, 1);
    gpio_put(SEVENSEG_B, 1);
    gpio_put(SEVENSEG_G, 1);
    gpio_put(SEVENSEG_C, 1);
    gpio_put(SEVENSEG_D, 1);
}

void vSet4()
{
    gpio_put(SEVENSEG_F, 1);
    gpio_put(SEVENSEG_G, 1);
    gpio_put(SEVENSEG_B, 1);
    gpio_put(SEVENSEG_C, 1);
}

void vSet5()
{
    gpio_put(SEVENSEG_A, 1);
    gpio_put(SEVENSEG_F, 1);
    gpio_put(SEVENSEG_G, 1);
    gpio_put(SEVENSEG_C, 1);
    gpio_put(SEVENSEG_D, 1);
}

void vSet6()
{
    gpio_put(SEVENSEG_A, 1);
    gpio_put(SEVENSEG_F, 1);
    gpio_put(SEVENSEG_E, 1);
    gpio_put(SEVENSEG_D, 1);
    gpio_put(SEVENSEG_C, 1);
    gpio_put(SEVENSEG_G, 1);
}

void vSet7()
{
    gpio_put(SEVENSEG_A, 1);
    gpio_put(SEVENSEG_B, 1);
    gpio_put(SEVENSEG_C, 1);
}

void vSet8()
{
    gpio_put(SEVENSEG_A, 1);
    gpio_put(SEVENSEG_B, 1);
    gpio_put(SEVENSEG_C, 1);
    gpio_put(SEVENSEG_D, 1);
    gpio_put(SEVENSEG_E, 1);
    gpio_put(SEVENSEG_F, 1);
    gpio_put(SEVENSEG_G, 1);
}

void vSet9()
{
    gpio_put(SEVENSEG_G, 1);
    gpio_put(SEVENSEG_F, 1);
    gpio_put(SEVENSEG_A, 1);
    gpio_put(SEVENSEG_B, 1);
    gpio_put(SEVENSEG_C, 1);
}

void vUnset()
{
    gpio_put(SEVENSEG_A, 0);
    gpio_put(SEVENSEG_B, 0);
    gpio_put(SEVENSEG_C, 0);
    gpio_put(SEVENSEG_D, 0);
    gpio_put(SEVENSEG_E, 0);
    gpio_put(SEVENSEG_F, 0);
    gpio_put(SEVENSEG_G, 0);
}
