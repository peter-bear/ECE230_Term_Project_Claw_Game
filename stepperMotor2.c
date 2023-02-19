/*! \file */
/*!
 * stepperMotor2.c
 *
 * Description: Second stepper motor ULN2003 driver for MSP432P4111 Launchpad.
 *              Uses Timer_A1 and P6.7, P6.6, P6.5, P6.4
 *
 *  Created on: 01/09/2023
 *      Author: Vineet Ranade & Yao Xiong.
 */

#include "stepperMotor2.h"
#include "msp.h"

/* Global Variables  */
// Fill in array with 4-bit binary sequence for wave drive (half step)
const uint8_t stepperSequence2[STEP_SEQ_CNT2] = {0b1001, 0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000};
uint16_t stepPeriod2 = INIT_PERIOD2;
uint8_t currentStep2 = 0;

int clockWise2 = 0;


void initStepperMotor2(void) {
    // set stepper port pins as GPIO outputs
    STEPPER_PORT2->SEL0 = (STEPPER_PORT2->SEL0) & ~STEPPER_MASK2;
    STEPPER_PORT2->SEL1 = (STEPPER_PORT2->SEL1) & ~STEPPER_MASK2;
    STEPPER_PORT2->DIR = (STEPPER_PORT2->DIR) | STEPPER_MASK2;

    // initialize stepper outputs to LOW
    STEPPER_PORT2->OUT = (STEPPER_PORT2->OUT) & ~STEPPER_MASK2;

    /* Configure Timer_A0 and CCR0 */
    // Set period of Timer_A0 in CCR0 register for Up Mode
    TIMER_A1->CCR[0] = stepPeriod2;
    // Configure CCR0 for Compare mode with interrupt enabled (no output mode - 0)
    TIMER_A1->CCTL[0] = 0b0000000000010000;

    // Configure Timer_A0 in Stop Mode, with source SMCLK, prescale 12:1, and
    //  interrupt disabled  -  tick rate will be 4MHz (for SMCLK = 48MHz)
    // Configure Timer_A0 (requires setting control AND expansion register)
    TIMER_A1->CTL = 0b0000001000000100;
    TIMER_A1->EX0 = 0b0000000000000010;

    /* Configure global interrupts and NVIC */
    // Enable TA0CCR0 compare interrupt by setting IRQ bit in NVIC ISER0 register
    // Enable interrupt by setting IRQ bit in NVIC ISER0 register
    NVIC->ISER[0] |= 0x00000400;

    __enable_irq();                             // Enable global interrupt
}

void enableStepperMotor2(void) {
    // Configure Timer_A0 in Up Mode (leaving remaining configuration unchanged)
    TIMER_A1->CTL &= 0b1111111111011111;
    TIMER_A1->CTL |= 0b0000000000010000;
}

void disableStepperMotor2(void) {
    // For driver use
    //  Configure Timer_A0 in Stop Mode (leaving remaining configuration unchanged)
    TIMER_A1->CTL &= 0b1111111111001111;
}

void stepClockwise2(void) {
    currentStep2 = (currentStep2 + 1) % STEP_SEQ_CNT2;  // increment to next step position
    // Update output port for current step pattern
    //  do this as a single assignment to avoid transient changes on driver signals
    STEPPER_PORT2->OUT = (STEPPER_PORT2->OUT & 0x0F) + (stepperSequence2[currentStep2] << 4);
}

void stepCounterClockwise2(void) {
    currentStep2 = ((uint8_t)(currentStep2 - 1)) % STEP_SEQ_CNT2;  // decrement to previous step position (counter-clockwise)
    // For driver use
    //  update output port for current step pattern
    //  do this as a single assignment to avoid transient changes on driver signals
    STEPPER_PORT2->OUT = (STEPPER_PORT2->OUT & 0x0F) + (stepperSequence2[currentStep2] << 4);
}

// Timer A1 CCR0 interrupt service routine
void TA1_0_IRQHandler(void)
{
    /* Not necessary to check which flag is set because only one IRQ
     *  mapped to this interrupt vector     */
    if (clockWise2)
    {
        stepClockwise2();
    }
    else
    {
        stepCounterClockwise2();
    }
    // Clear timer compare flag in TA3CCTL0
    TIMER_A1->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG);
}

void setDirection2(int dir)
{
    if (dir == CW_DIR2)
    {
        clockWise2 = 1;
    }
    else
    {
        clockWise2 = 0;
    }
}

void toggleDirection2(void)
{
    if (clockWise2)
    {
        clockWise2 = 0;
    }
    else
    {
        clockWise2 = 1;
    }
}

void setRPM2(double RPM)
{
    stepPeriod2 = CLK_RATE2 * SEC_PER_MIN2 / (RPM * STEPS_PER_REV2);
    TIMER_A1->CCR[0] = stepPeriod2;
}
