/*
 * sw.c
 *
 * Description: Handles all joystick pushbutton inputs with ISR.
 *
 *  Created on: Jan 30, 2023
 *      Author: Vineet Ranade & Yao Xiong
 */

#include "sw.h"
#include "led.h"
#include "stateMachine.h"

extern int curState;

/**
 * @brief Initializes S2 switch
 *
 * Configures S2 as GPIO input with internal pull-up resistor.
 */
void initializeSwitches(void)
{
    pressed = 0;
    resetNeeded = 0;
    lastPressedTime = RESET_LAST_PRESSED_TIME;

    // Set P2.4 to be primary module function input (capture CCIxA for TA0) and pull-up
    SwitchPort->SEL0 |= (JoystickSwitch);
    SwitchPort->SEL1 &= ~(JoystickSwitch);
    SwitchPort->DIR &= ~(JoystickSwitch);
    SwitchPort->REN |= (JoystickSwitch);
    SwitchPort->OUT |= (JoystickSwitch);

    // CCR1 for capture mode on both rising and falling edges, interrupt enabled
    TIMER_A0->CCTL[1] = 0b1100000100010000;

    // Configure Timer_A0 in Continuous Mode with source ACLK prescale 1:1 and
    //  interrupt enabled
    //      Tick rate will be 32kHz with rollover at 0xFFFF
    // Configure Timer_A0
    TIMER_A0->CTL = 0b0000000100100100;

    NVIC->ISER[0] |= 0x00000200;                    // Set IRQ bit for TA0_N interrupt

}

/*!
 * \brief TA0 CCRN interrupt service routine
 *
 * Handles press/release of pushbutton.
 *
 * \return None
 */
/* Timer_A0 and CCRx (except CCR0) interrupt service routine */
void TA0_N_IRQHandler(void)
{
    /* Check if interrupt triggered by CCR1 */
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG)
    {
        // Update press/release status
        if (pressed)
        {
            pressed = 0;
        }

        else
        {
            // First toggle Red LED for debugging purposes & feedback
            RGB_PORT->OUT ^= (RGB_RED_PIN);

            // If pressed during gameplay, toggle claw and save lastPressed times
            if (curState == JOYSTICK_MOVE_STATE)
            {
                toggleServo();
                lastPressed = TIMER_A0->CCR[1];
                lastPressedTime = curTime;
            }

            // If pressed during end of game state, a reset is needed
            if (curState == GAME_WON_STATE || curState == GAME_OVER_STATE)
            {
                resetNeeded = 1;
            }

            pressed = 1;
        }

        int delayLeft = DELAY_TIME; // Delay time for debouncing switches

        /* Lazy debounce since using SysTick sometimes broke pushbutton input
         * if the button was pressed very quickly
         */
        while (delayLeft != 0)
        {
            delayLeft--;
        }

        // Clear flag
        TIMER_A0->CCTL[1] &= ~(TIMER_A_CCTLN_CCIFG);
    }
}
