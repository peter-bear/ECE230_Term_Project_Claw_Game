/*! \file */
/*!
 * stepperMotor2.h
 *
 * Description: Second stepper motor ULN2003 driver for MSP432P4111 Launchpad.
 *              Uses Timer_A1 and P6.7, P6.6, P6.5, P6.4
 *
 *  Created on: 01/09/2023
 *      Author: Vineet Ranade & Yao Xiong
 */

#ifndef STEPPERMOTOR2_H_
#define STEPPERMOTOR2_H_

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

#define STEPPER_PORT2                    P6
#define STEPPER_MASK2                    (0x00F0)
#define STEPPER_IN12                     (0x0080)
#define STEPPER_IN22                     (0x0040)
#define STEPPER_IN32                     (0x0020)
#define STEPPER_IN42                     (0x0010)

#define INIT_PERIOD2                     10000
#define CLK_RATE2                        4000000
#define STEPS_PER_REV2                   32*64*2
#define SEC_PER_MIN2                     60
#define STEP_SEQ_CNT2                    8
#define MIN_RPM2                         1
#define MAX_RPM2                         15
#define CW_DIR2                          1
#define CCW_DIR2                         0

/*!
 * \brief This function configures pins and timer for stepper motor driver
 *
 * This function configures P6.4, P6.5, P6.6, and P6.7 as output pins
 *  for the ULN2003 stepper driver IN port, and initializes Timer_A3 to
 *  increment step position with compare interrupt
 *
 * Modified bits 4 to 7 of \b P6DIR register and \b P6SEL registers.
 * Modified \b TA1CTL register and \b TA1CCTL0 registers.
 *
 * \return None
 */
extern void initStepperMotor2(void);


/*!
 * \brief This starts stepper motor rotation by turning on Timer_A1
 *
 * This function starts stepper motor rotation by turning on Timer_A1.
 * Assumes stepper motor has already been configured by initStepperMotor2().
 *
 * Modified \b TA1CTL register.
 *
 * \return None
 */
extern void enableStepperMotor2(void);


/*!
 * \brief This stops stepper motor rotation by turning off timer
 *
 * This function stops stepper motor rotation by turning off Timer_A1.
 * Stepper motor is still configured after calling this function.
 *
 * Modified \b TA1CTL register.
 *
 * \return None
 */
extern void disableStepperMotor2(void);


/*!
 * \brief This increments step clockwise
 *
 * This function increments to next clockwise step position
 *
 * Modified bit 4, 5, 6, and 7 of \b P6OUT register.
 *
 * \return None
 */
extern void stepClockwise2(void);


/*!
 * \brief This increments step counter-clockwise
 *
 * This function increments to next counter-clockwise step position
 *
 * Modified bit 4, 5, 6, and 7 of \b P6OUT register.
 *
 * \return None
 */
extern void stepCounterClockwise2(void);

/*!
 * \brief Changes direction of stepper motor
 *
 * This function changes from clockwise to counter-clockwise or vice-versa
 *
 * \return None
 */
extern void toggleDirection2(void);

extern void setDirection2(int dir);

/*!
 * \brief Changes rotation speed of stepper motor
 *
 * Updates TA1 CCR0
 *
 * \param RPM How fast the stepper motor should rotate
 *
 * \return None
 */
extern void setRPM2(double RPM);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* STEPPERMOTOR2_H_ */
