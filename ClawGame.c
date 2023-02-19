/*
 * ClawGame.c
 *
 * Description: Driver program for claw game using state machine
 *
 * Project Description:

 *  Overview of Project Features
    We created a variation of the traditional arcade claw game. The game serves as a microcontroller-based embedded system which acts on a player�s inputs.
    Our inspiration for the project came from a suggested idea in the term project specification document. Upon further thought, we realized that implementing
    three-dimensional movement of the claw was surprisingly challenging. Although the software would be simple with the use of three stepper motors, the
    physical build would have been quite complex. Thus, we decided to implement two-dimensional movement of the claw: side-to-side motion and up-and-down
    motion. With more time, the project can be extended to add the third dimension.
    In a normal claw game, the user uses the x- and y-coordinates of a joystick to move the claw along the upper plane of the build. A button is then
    pressed to prompt the claw to automatically descend and try to grab an object. However, we used the joystick�s x-coordinate for the side-to-side movement
    and the y-coordinate for up-and-down movement. This gives the player more control over the claw and makes the game easier to win. The player is given 80
    seconds each round. A round is won if the player can grab an object and drop it into a small box. The score will be equivalent to the time remaining when
    the round is won plus any added bonus, which depends on the height from which the object was dropped.

    Alignment with ECE230 Project Requirements
    The following subsections list the basic project requirements with a description of how we satisfied them. We have also elaborated on additional
    features we included to go beyond the minimum requirements.

    Microcontroller: Must be MSP432P4111
    The microcontroller used for the project was the MSP432P4111. It was mounted on the top portion of the build so it could be close to the stepper motors.

    Sensors: At least 1
    Evidently, the most important sensor in a claw game is the joystick. The analog signals generated by the x- and y- position of the joystick
    translate directly and linearly to the direction and speed of the claw. We were able to make use of the joystick�s pushbutton to serve as a reset
    switch when the game is finished as well as the mechanism to open or close the claw during gameplay. Finally, two photoresistors were used in the
    small box. With a simple voltage divider, we could detect if an object was dropped in the box. The analog signal generated by one or both photoresistors
    would drop below a predefined cutoff voltage if an object obstructed light from reaching them. All ADC inputs were used in a 10-bit sequence-of-channels
    configuration.

    Actuators: At least 1
    Several actuators were used in the project. The LCD was configured in 4-bit mode. It reflected which state the program was in (resetting, countdown,
    gameplay, game win, or game loss). The resetting, countdown, and gameplay states all had a time countdown shown on the LCD. The game win and loss
    states prompt the user to press the pushbutton to play again. Additionally, the game win state displays the score of the most recent winner. Two
    stepper motors served as the actuators related to the joystick input, translating the claw in two dimensions to the player�s intended destination.
    The motors spin faster the farther the joystick is pushed. A servo motor in the claw was toggled between two angles. In this way, we could open and
    close the claw in response to a button press during gameplay.

    Timers: At least 1
    All four timer modules provided by Timer A and SysTick were required. They were used to control the steppers� rotational velocity, generate the
    servo�s PWM signal, track all time in the game, and create various delays.

    Interrupts: At least 1
    Several interrupts were used in tandem with the timers. They were used to act on pushbutton presses, schedule the next step of the stepper motors,
    time the positive pulse width and period of the PWM signal, and create a 1/8-second cadence for the game to follow.

 *
 * Additional Features
    To track all real time in the game, we used Timer32.  Timer32 is an MSP432P4111 peripheral which was never used in a lab exercise or project.
    We also implemented a small bonus score feature in the game. While users have more control over our claw than in a traditional arcade game,
    we wanted to incentivize them to drop the object into the box from as high as possible (even though one can simply place the object on top of the
    photoresistor if desired). To gauge the height from which the object was dropped, we used a capture module of Timer A0 to detect the final button press.
    The magnitude of the bonus score depends on how much time elapsed between the drop and landing of the object. If the object was merely pushed or placed
    into the box, no bonus will be awarded. This is the first time we have used a Timer A module in its capture configuration in the class.
    One extended feature we began implementing was the use of an ESP8266 to host a webpage tracking all players� scores. We wanted to use SPI communication for this.
    Unfortunately, we had to prioritize dealing with power issues over this extra feature and did not have time to complete it to our satisfaction
 *
 *
 *  Created on: Feb 5, 2023
 *      Author: Vineet Ranade & Yao Xiong
 */

#include "stateMachine.h"

/**
 * main.c
 */
void main(void)
{
    initializeAll();
    while (1)
    {
        switch (curState)
        {
            case RESETTING_STATE:
                reset();
                break;
            case READY_START_STATE:
                countDown();
                break;
            case JOYSTICK_MOVE_STATE:
                moveJoystick();
                break;
            case GAME_WON_STATE:
                showWon();
                break;
            case GAME_OVER_STATE:
                showLost();
                break;
            default:
                break;
        }

    }
}

