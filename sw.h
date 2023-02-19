/*
 * sw.h
 *
 * Description: Header file for joystick pushbutton input
 *
 *  Created on: Jan 30, 2023
 *      Author: Vineet Ranade & Yao Xiong
 */

#ifndef SW_H_
#define SW_H_

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

#include <msp.h>

#define SwitchPort          P2               // Port 2
#define JoystickSwitch      0b00010000       // P2.4 is the joystick button

#define DELAY_TIME              1500
#define RESET_LAST_PRESSED_TIME 100

int pressed;
int resetNeeded;

int lastPressed;
int lastPressedTime;

/*!
 * \brief Initializes P2.4 for primary module function.
 *
 * This involves capture input for TA0 CCR1.
 *
 * \param       None
 * \return      None
 */
extern void initializeSwitches(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* SW_H_ */
