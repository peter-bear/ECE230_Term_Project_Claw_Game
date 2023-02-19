/*
 * timer32.c
 *
 * Timer32 configuration for 8 Hz sampling
 *
 *  Created on: Jan 27, 2023
 *      Author: Vineet Ranade & Yao Xiong
 */

#include <msp.h>
#include <timer32.h>

void setupT32()
{
    record = 1;
    curTime = RESET_TIME;
    secondTicks = 0;

    // Reload value = 1/8 second
    TIMER32_1->LOAD = ONE_EIGHTH;

    // Interrupt enabled, periodic mode
    TIMER32_1->CONTROL = 0x000000F2;

    // Clear interrupt flag
    TIMER32_1->INTCLR = 0;

    // Set IRQ bit
    NVIC->ISER[0] |= 0x02000000;
}

/* Timer_A1 and CCRx (except CCR0) interrupt service routine */
void T32_INT1_IRQHandler(void)
{
    record = 1; // Need to update LCD and stepper speed
    secondTicks++; // Increment ticks
    if (secondTicks == 8)
    {
        // Count down 1 second since 8 * 1/8 = 1 full second
        curTime--;
        secondTicks = 0;
    }

    // Clear interrupt flag
    TIMER32_1->INTCLR = 0;
}
