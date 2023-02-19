/*! \file */
/*!
 * csLFXT.c
 *
 * Description: Basic driver for configuring LFXT clock source on MSP432P4111
 *               Launchpad.
 *
 *  Created on: 12/28/2022
 *      Author: Vineet Ranade & Yao Xiong
 */


#include "clocks.h"
#include "msp.h"

void configClocks(void)
{
    CS->KEY = CS_KEY_VAL;                   // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_3;           // Set DCO to 12MHz (nominal, center of 8-16MHz range)
    CS->CTL1 = CS_CTL1_SELA_2 |             // Select ACLK = REFO
            CS_CTL1_SELS_3 |                // SMCLK = DCO
            CS_CTL1_SELM_3;                 // MCLK = DCO
    CS->KEY = 0;                            // Lock CS module from unintended accesses

}
