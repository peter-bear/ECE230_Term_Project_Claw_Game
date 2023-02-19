/*
 * led.h
 *
 * Description: Header file for Red LED
 *
 *  Created on: Jan 30, 2023
 *      Author: Vineet Ranade & Yao Xiong
 */

#ifndef LED_H_
#define LED_H_

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

#define RGB_PORT            P2          // Port 2
#define RGB_RED_PIN         0b00000001  // P2.0

/*!
 * Initializes Red LED. It will toggle on and off with each press of the joystick
 * pushbutton
 *
 * \param   None
 *
 * \return  None
 */
extern void initializeRGBLEDs(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* LED_H_ */
