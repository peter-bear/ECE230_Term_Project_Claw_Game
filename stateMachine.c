/*
 * stateMachine.c
 *
 * Description: Includes all state transitions for the project along
 *              with some math computations needed.
 *
 *  Created on: Feb 3, 2023
 *      Author: Vineet Ranade & Yao Xiong
 */

#include "stateMachine.h"

// External pushbutton variables
extern int pressed;

// Real time variables
extern int record;
extern int curTime;
extern int resetNeeded;
extern int lastPressed;

// LCD variables
char lcdText[] = "                                "; // 32 spaces

// ADC variables
extern volatile bool resultReady;
extern int xVal, yVal, photoVal;

// File-specific variables related to scoring and time
int winTime;
int score;
int bonus;
int winTicks;

void initState()
{
    curState = RESETTING_STATE;
}

void showWon(void)
{
    // Steppers should be stationary
    disableStepperMotor();
    disableStepperMotor2();

    // Gripper should be open
    setServoAngle(MIN_ANGLE);

    // Show winning message
    sprintf(lcdText, "WINNER! Score=%d Button=restart", score);
    updateDispVal(lcdText);

    // If reset button pressed, change state
    if (resetNeeded)
    {
        curState = RESETTING_STATE;
        curTime = RESET_TIME;
        resetNeeded = 0;
    }
}

void showLost(void)
{
    // Steppers should be stationary
    disableStepperMotor();
    disableStepperMotor2();

    // Gripper should be open
    setServoAngle(MIN_ANGLE);

    // Show losing message
    sprintf(lcdText, "Game over! Button to restart");
    updateDispVal(lcdText);

    // If reset button pressed, change state and time
    if (resetNeeded)
    {
        curState = RESETTING_STATE;
        curTime = RESET_TIME;
        resetNeeded = 0;
    }
}

void reset(void)
{
    // Gripper should be open
    setServoAngle(MIN_ANGLE);

    // Show resetting message with countdown
    sprintf(lcdText, "Resetting... %d", curTime);
    updateDispVal(lcdText);

    // Move horizontal stepper motor to the right
    setDirection(CCW_DIR);
    enableStepperMotor();
    setRPM(MAX_RPM);

    // Move vertical stepper motor upwards
    setDirection2(CW_DIR);
    enableStepperMotor2();
    setRPM2(MAX_RPM);

    // If reset time is done, start the countdown
    // for the player's round
    if (curTime == 0)
    {
        curState = READY_START_STATE;
        curTime = COUNTDOWN_TIME;
    }
}

void countDown(void)
{
    // Steppers should be stationary
    disableStepperMotor();
    disableStepperMotor2();

    // Gripper should be closed
    setServoAngle(MAX_ANGLE);

    // Show countdown message
    sprintf(lcdText, "Starting in... %d", curTime);
    updateDispVal(lcdText);

    // If the countdown is over, start the round!
    if (curTime == 0)
    {
        curState = JOYSTICK_MOVE_STATE;
        curTime = GAMEPLAY_TIME;
    }
}

void moveJoystick(void)
{
    // Sample all ADC channels
    adcSample();
    while (!resultReady);
    resultReady = false;

    // If user has run out of time, they lose
    if (curTime == 0)
    {
        lastPressedTime = RESET_LAST_PRESSED_TIME;
        curState = GAME_OVER_STATE;
    }

    // If object is detected, they win
    else if (photoVal < TOO_DARK || photoVal2 < TOO_DARK)
    {
        photoVal = 1000;
        photoVal2 = 1000;
        winTime = curTime;
        winTicks = TIMER_A0->R;
        calculateBonus();
        score = winTime + bonus;
        lastPressedTime = RESET_LAST_PRESSED_TIME;
        curState = GAME_WON_STATE;
    }

    // Otherwise, update the actuators as necessary
    else if (record)
    {
        // Update LCD with time remaining
        sprintf(lcdText, "GO! Time: %d", curTime);
        updateDispVal(lcdText);
        record = 0;

        // Update stepper movement
        moveSteppers();
    }
}

void initializeAll(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    configClocks();
    setupT32();
    setupLCD();
    initializeSwitches();
    initializeRGBLEDs();
    initADCPorts();
    configureADC14();
    initStepperMotor2();
    initStepperMotor();
    disableStepperMotor();
    initServoMotor();
    initState();
    __enable_irq();
}

void moveSteppers(void)
{
    // Handle x-direction movement by translating x-coordinate to a direction and RPM
    if (xVal < MID_RANGE - REST_ERROR)
    {
        setDirection(CCW_DIR);
        enableStepperMotor();
        double RPM = (((MAX_VAL - xVal) - (MID_RANGE + REST_ERROR)) / MAX_DIFF) * (MAX_RPM - MIN_RPM) + MIN_RPM;
        setRPM(RPM);
    }
    else if (xVal > MID_RANGE + REST_ERROR)
    {
        setDirection(CW_DIR);
        enableStepperMotor();
        double RPM = ((xVal - (MID_RANGE + REST_ERROR + 1)) / (MAX_DIFF - 1.0)) * (MAX_RPM - MIN_RPM) + MIN_RPM;
        setRPM(RPM);
    }
    else
    {
        disableStepperMotor();
    }

    // Handle y-direction movement by translating y-coordinate to a direction and RPM
    if (yVal < MID_RANGE - REST_ERROR)
    {
        setDirection2(CW_DIR);
        enableStepperMotor2();
        double RPM = (((MAX_VAL - yVal) - (MID_RANGE + REST_ERROR)) / MAX_DIFF) * (MAX_RPM - MIN_RPM) + MIN_RPM;
        setRPM2(RPM);
    }
    else if (yVal > MID_RANGE + REST_ERROR)
    {
        setDirection2(CCW_DIR);
        enableStepperMotor2();
        double RPM = ((yVal - (MID_RANGE + REST_ERROR + 1)) / (MAX_DIFF - 1.0)) * (MAX_RPM - MIN_RPM) + MIN_RPM;
        setRPM2(RPM);
    }
    else
    {
        disableStepperMotor2();
    }
}

void calculateBonus(void)
{
    // drop right above: 6106 ticks elapse
    // drop from maximum height: 13109 ticks elapse

    // Prize has to have been dropped less than 1 second before detection
    // This is because an object dropped from rest 15 inches off the ground
    // will hit the ground in much less than 1 second
    if (lastPressedTime - winTime < 2)
    {
        // Compute the number of ticks elapsed between release and landing
        if (winTicks <= lastPressed)
        {
            bonus = (winTicks + (MAX_16_BIT - lastPressed));
        }
        else
        {
            bonus = winTicks - lastPressed;
        }

        // Re-scale bonus score based on how much time elapsed between release and landing
        if (bonus > BONUS_CUTOFF && bonus < ONE_SECOND_TICKS)
        {
            bonus = (bonus - BONUS_CUTOFF) / BONUS_SCALE;
        }
        else
        {
            bonus = 0; // Wasn't dropped from high enough, no bonus
        }
    }

    // If bonus criteria not satisfied from time constraints, no bonus
    else
    {
        bonus = 0;
    }
}
