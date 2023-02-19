/*
 * led.c
 *
 * Description: Includes initialization for Red LED
 *
 *  Created on: Jan 30, 2023
 *      Author: Vineet Ranade & Yao Xiong
 */

#include "led.h"

void initializeRGBLEDs(void)
{
    // Set RED LED to be output
    RGB_PORT->SEL0 &= ~(RGB_RED_PIN);
    RGB_PORT->SEL1 &= ~(RGB_RED_PIN);
    RGB_PORT->DIR |= (RGB_RED_PIN);
    // Set LED2 output to LOW
    RGB_PORT->OUT &= ~(RGB_RED_PIN);
}
// End InitializeRGBLEDs(void)
