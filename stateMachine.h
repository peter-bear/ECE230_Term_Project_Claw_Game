/*
 * stateMachine.h
 *
 * Description: Header file defining states for our project.
 *
 *  Created on: Feb 3, 2023
 *      Author: Vineet Ranade & Yao Xiong
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

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

#include "clocks.h"
#include <timer32.h>
#include "msp.h"
#include "lcd.h"
#include "sysTickDelays.h"
#include "adc.h"
#include "sw.h"
#include "led.h"
#include "stepperMotor.h"
#include "stepperMotor2.h"
#include "servoDriver.h"
#include "stateMachine.h"
#include <stdint.h>
#include <stdio.h>

#define RESETTING_STATE     0
#define READY_START_STATE   1
#define JOYSTICK_MOVE_STATE 2
#define GAME_WON_STATE      3
#define GAME_OVER_STATE     4

#define MAX_16_BIT          65535
#define BONUS_CUTOFF        4000
#define BONUS_SCALE         1000
#define ONE_SECOND_TICKS    32768

int curState;

/*
 * \brief Shows player that they won and displays score.
 *
 * Also prompts user to press button to play again.
 *
 * \param       None
 * \return      None
 */
void showWon(void);

/*
 * \brief Sets the first state of the entire program.
 *
 * This is the reset state.
 *
 * \param       None
 * \return      None
 */
void initState(void);

/*
 * \brief Shows player that they lost.
 *
 * Also prompts user to press button to play again.
 *
 * \param       None
 * \return      None
 */
void showLost(void);

/*
 * \brief Resets the claw position autonomously.
 *
 * \param       None
 * \return      None
 */
void reset(void);

/*
 * \brief Shows the player a countdown before the round starts.
 *
 * \param       None
 * \return      None
 */
void countDown(void);

/*
 * \brief Handles the actual gameplay state.
 *
 * \param       None
 * \return      None
 */
void moveJoystick(void);

/*
 * \brief Initializes ALL components of this project.
 *
 * \param       None
 * \return      None
 */
void initializeAll(void);

/*
 * \brief Translates joystick position to claw movement.
 *
 * \param       None
 * \return      None
 */
void moveSteppers(void);

/*
 * \brief Computes bonus score when player wins a round.
 *
 * \param       None
 * \return      None
 */
void calculateBonus(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* STATEMACHINE_H_ */
