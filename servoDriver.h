/*! \file */
/*!
 * servoDriver.h
 *
 * Description: Servo motor driver for MSP432P4111 Launchpad.
 *              Assumes SMCLK configured with 12 MHz DCO as source.
 *              Uses Timer_A2 and P5.6 (TA2.1)
 *
 *  Created on: 1/16/23
 *      Author: Vineet Ranade & Yao Xiong
 */

#ifndef SERVODRIVER_H_
#define SERVODRIVER_H_

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include "msp.h"

// TODO add tick count values for constants
#define FREQUENCY                       2000000
#define SERVO_TMR_PERIOD                FREQUENCY * 0.025                                                       // ticks for 50Hz signal
#define SERVO_MID_ANGLE                 FREQUENCY * 0.0015                                                     // defined to be middle position
#define SERVO_90_TICKS                  9
#define SERVO_MIN_ANGLE                 FREQUENCY * 0.00051         // ticks for 0.75ms pulse, originally 2250
#define SERVO_MAX_ANGLE                 FREQUENCY * 0.00249         // ticks for 2.25ms pulse, originally 6750 (4500 average)
#define TEN_DEGREE_TICKS                (SERVO_MAX_ANGLE - SERVO_MIN_ANGLE) / 18                                                       // ticks 10 degree shift, originally 250
#define ONE_DEGREE_TICKS                TEN_DEGREE_TICKS / 10
#define MAX_ANGLE                       120
#define MIN_ANGLE                       40


/*!
 * \brief This function configures pins and timer for servo motor driver
 *
 * This function configures P5.6 as output pin for servo drive signal and
 *  initializes Timer_A2 CCR1 for PWM output
 *
 * Modified bit 6 of \b P2DIR register and \b P2SEL registers.
 * Modified \b TA2CTL, \b TA2CCTL1 and CCR registers.
 *
 * \return None
 */
extern void initServoMotor(void);


/*!
 * \brief This increments servo angle 10 degrees, with wrap-around
 *
 * This function increments servo angle by 10 degrees. If new angle exceeds max
 *  angle (+90), it wraps around to min angle (-90)
 *
 * Modified \b TA2CCR1 register.
 *
 * \return None
 */
extern void incrementTenDegree(void);


/*!
 * \brief This function sets angle of servo
 *
 * This function sets angle of servo to \a angle (between 0 to 180)
 *
 *  \param angle Angle in degrees to set servo (between 0 to 180)
 *
 * Modified \b TA2CCR1 register.
 *
 * \return None
 */
extern void setServoAngle(uint8_t angle);

/*!
 * \brief  This function toggles the servo between the minimum and maximum
 *         angle for this project.
 *
 * If the angle is 120 degrees, it will become 40 degrees.
 * If the angle is 40 degrees, it will become 120 degrees.
 *
 * \param  None
 * \return None
 */
extern void toggleServo(void);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* SERVODRIVER_H_ */
