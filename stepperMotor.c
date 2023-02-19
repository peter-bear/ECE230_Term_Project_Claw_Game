/*! \file */
/*!
 * stepperMotor.c
 *
 * Description: Stepper motor ULN2003 driver for MSP432P4111 Launchpad.
 *              Uses Timer_A3 and P2.7, P2.6, P2.5, P2.4
 *
 *  Created on: 01/09/2023
 *      Author: Vineet Ranade & Yao Xiong
 */

#include "stepperMotor.h"
#include "msp.h"

/* Global Variables  */
// Fill in array with 4-bit binary sequence for wave drive (half step)
const uint8_t stepperSequence[STEP_SEQ_CNT] = {0b1001, 0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000};
uint16_t stepPeriod = INIT_PERIOD;
uint8_t currentStep = 0;

int clockWise = 1;


void initStepperMotor(void) {
    // set stepper port pins as GPIO outputs
    STEPPER_PORT->SEL0 = (STEPPER_PORT->SEL0) & ~STEPPER_MASK;
    STEPPER_PORT->SEL1 = (STEPPER_PORT->SEL1) & ~STEPPER_MASK;
    STEPPER_PORT->DIR = (STEPPER_PORT->DIR) | STEPPER_MASK;

    // initialize stepper outputs to LOW
    STEPPER_PORT->OUT = (STEPPER_PORT->OUT) & ~STEPPER_MASK;

    /* Configure Timer_A3 and CCR0 */
    // Set period of Timer_A3 in CCR0 register for Up Mode
    TIMER_A3->CCR[0] = stepPeriod;
    // TODO configure CCR0 for Compare mode with interrupt enabled (no output mode - 0)
    TIMER_A3->CCTL[0] = 0b0000000000010000;

    // Configure Timer_A3 in Stop Mode, with source SMCLK, prescale 3:1, and
    //  interrupt disabled  -  tick rate will be 4MHz (for SMCLK = 12MHz)
    // TODO configure Timer_A3 (requires setting control AND expansion register)
    TIMER_A3->CTL = 0b0000001000000100;
    TIMER_A3->EX0 = 0b0000000000000010;

    /* Configure global interrupts and NVIC */
    // Enable TA3CCR0 compare interrupt by setting IRQ bit in NVIC ISER0 register
    // Enable interrupt by setting IRQ bit in NVIC ISER0 register
    NVIC->ISER[0] |= 0x00004000;

    __enable_irq();                             // Enable global interrupt
}

void enableStepperMotor(void) {
    // Configure Timer_A3 in Up Mode (leaving remaining configuration unchanged)
    TIMER_A3->CTL &= 0b1111111111011111;
    TIMER_A3->CTL |= 0b0000000000010000;
}

void disableStepperMotor(void) {
    // For driver use
    //  Configure Timer_A3 in Stop Mode (leaving remaining configuration unchanged)
    TIMER_A3->CTL &= 0b1111111111001111;
}

void stepClockwise(void) {
    currentStep = (currentStep + 1) % STEP_SEQ_CNT;  // increment to next step position
    // Update output port for current step pattern
    //  do this as a single assignment to avoid transient changes on driver signals
    //STEPPER_PORT->OUT = (STEPPER_PORT->OUT & 0x0F) + (stepperSequence[currentStep] << 4);
    STEPPER_PORT->OUT = (STEPPER_PORT->OUT & 0b00010111) + ((stepperSequence[currentStep] << 4) & 0b11100000) + (stepperSequence[currentStep] << 3 & 0b00001000);
}

void stepCounterClockwise(void) {
    currentStep = ((uint8_t)(currentStep - 1)) % STEP_SEQ_CNT;  // decrement to previous step position (counter-clockwise)
    // For future driver use
    //  update output port for current step pattern
    //  do this as a single assignment to avoid transient changes on driver signals
    STEPPER_PORT->OUT = (STEPPER_PORT->OUT & 0b00010111) + ((stepperSequence[currentStep] << 4) & 0b11100000) + (stepperSequence[currentStep] << 3 & 0b00001000);
}

// Timer A3 CCR0 interrupt service routine
void TA3_0_IRQHandler(void)
{
    /* Not necessary to check which flag is set because only one IRQ
     *  mapped to this interrupt vector     */
    if (clockWise)
    {
        stepClockwise();
    }
    else
    {
        stepCounterClockwise();
    }
    // Clear timer compare flag in TA3CCTL0
    TIMER_A3->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG);
}

void setDirection(int dir)
{
    if (dir == CW_DIR)
    {
        clockWise = 1;
    }
    else
    {
        clockWise = 0;
    }
}

void toggleDirection(void)
{
    if (clockWise)
    {
        clockWise = 0;
    }
    else
    {
        clockWise = 1;
    }
}

void setRPM(double RPM)
{
    stepPeriod = CLK_RATE * SEC_PER_MIN / (RPM * STEPS_PER_REV);
    TIMER_A3->CCR[0] = stepPeriod;
}
