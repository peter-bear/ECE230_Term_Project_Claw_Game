/*
 * adc.h
 *
 * Description: Header file for 4 ten-bit ADCs used in the project.
 *
 *  Created on: Jan 30, 2023
 *      Author: Vineet Ranade & Yao Xiong
 */

#ifndef ADC_H_
#define ADC_H_

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
#include <stdbool.h>

/*******************/
/********ADC********/
/*******************/


#define ADC_PORT        P6                      // Port 6
#define ADC_X_BIT       0b00000001              // P6.0 for x-direction
#define ADC_Y_BIT       0b00000010              // P6.1 for y-direction
#define ADC_PORT_2      P5                      // Port 5
#define ADC_PHOTO       0b00010000              // P5.4 for photoresistor 1
#define ADC_PHOTO_2     0b00000100              // P5.2 for photoresistor 2

#define MID_RANGE       512                     // Since we use 10 bits, 512 is middle
#define REST_ERROR      12                      // Error for resting position of joystick
#define MAX_DIFF        499.0                   // 499.0 for less than, 498.0 for greater than
#define MAX_VAL         1023                    // 2^10 - 1
#define TOO_DARK        150                     // Dark range for photoresistors based on experimentation

int xVal, yVal, photoVal, photoVal2;            // Variables holding ADC results
volatile bool resultReady;                      // Stores whether ADC value is ready


extern void configureADC14(void);
extern void initADCPorts(void);
extern void adcSample(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* ADC_H_ */
