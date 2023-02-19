/*! \file */
/*!
 * servoDriver.c
 *
 * Description: Servo motor driver for MSP432P4111 Launchpad.
 *              Assumes SMCLK configured with 48MHz HFXT as source.
 *              Uses Timer_A2 and P5.6 (TA2.1)
 *
 *  Created on: 1/16/23
 *      Author: Vineet Ranade & Yao Xiong
 */

#include "servoDriver.h"
#include "msp.h"

/* Global Variables  */
uint16_t pulseWidthTicks = SERVO_MIN_ANGLE;

#define ServoPort P5
#define ServoBit  0b01000000

int curAngle;

void initServoMotor(void) {
    // Configure servo pin (P5.6) for primary module function (TA2.1),
    // output, initially LOW
    ServoPort->SEL1 &= ~(ServoBit); // SEL1 = 0 for primary function
    ServoPort->SEL0 |= (ServoBit);  // SEL0 = 1 for primary function
    ServoPort->DIR |= (ServoBit);   // Configure as output
    ServoPort->OUT &= ~(ServoBit);  // Turn off

    /* Configure Timer_A2 and CCR1 */
    TIMER_A2->CCTL[1] = 0b0000000011100000;
    // Set period of Timer_A2 in CCR0 register for Up Mode
    TIMER_A2->CCR[0] = SERVO_TMR_PERIOD;
    // Set initial positive pulse-width of PWM in CCR1 register
    TIMER_A2->CCR[1] = SERVO_MIN_ANGLE;

    // Configure TA2CCR1 for Compare mode, Reset/Set output mode, with interrupt disabled

    // Configure Timer_A2 in Up Mode, with source SMCLK, prescale 16:1, and
    //  interrupt disabled  -  tick rate will be 2MHz (for SMCLK = 12MHz)
    // Configure Timer_A2 (requires setting control AND expansion register)
    TIMER_A2->EX0 = 0b0000000000000101;
    TIMER_A2->CTL = 0b0000001000010100;

    setServoAngle(MAX_ANGLE);
}

void incrementTenDegree(void) {
    // update pulse-width for <current angle> + <10 degrees>
    pulseWidthTicks += TEN_DEGREE_TICKS;
    if (pulseWidthTicks > SERVO_MAX_ANGLE) {
        pulseWidthTicks = SERVO_MIN_ANGLE;
    }
    // Update CCR1 register to set new positive pulse-width
    TIMER_A2->CCR[1] = pulseWidthTicks;
}

void setServoAngle(uint8_t angle) {
    // Useful function for driver
    TIMER_A2->CCR[1] = SERVO_MIN_ANGLE + ONE_DEGREE_TICKS * angle;
    curAngle = angle;
}

void toggleServo()
{
    // Toggle and set angle
    if (curAngle == MAX_ANGLE)
    {
        curAngle = MIN_ANGLE;
    }
    else
    {
        curAngle = MAX_ANGLE;
    }
    setServoAngle(curAngle);
}
