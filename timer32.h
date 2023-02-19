/*
 * timer32.h
 *
 * Header file Timer32
 *
 *  Created on: Jan 27, 2023
 *      Author: Vineet Ranade & Yao Xiong
 */

#ifndef TIMER32_H_
#define TIMER32_H_

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

#define ONE_EIGHTH      (1500000*2)-1
#define RESET_TIME      10
#define COUNTDOWN_TIME  3
#define GAMEPLAY_TIME   80

int record;
int secondTicks;
int curTime;

/*!
 *
 * \brief This function initializes T32
 *
 * This function provides T32 with a reload value for 1/8 seconds
 *
 * \param  None
 *
 * \return None
 */
extern void setupT32(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* TIMER32_H_ */
