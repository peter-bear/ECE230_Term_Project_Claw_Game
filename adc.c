/*
 * adc.c
 *
 * Description: Initialization and ISR for ADC14 captures
 *
 *  Created on: Jan 30, 2023
 *      Author: Vineet Ranade & Yao Xiong
 */

#include "adc.h"

/*!
 * \brief Initializes ADC Ports for Use
 *
 * Sets P6.0 and P6.1 to tertiary module function
 *
 * \return None
 */
void initADCPorts(void)
{
    resultReady = false;   // Stores whether ADC value is ready
    ADC_PORT->SEL0 |= ADC_X_BIT;
    ADC_PORT->SEL1 |= ADC_X_BIT;
    ADC_PORT->SEL0 |= ADC_Y_BIT;
    ADC_PORT->SEL1 |= ADC_Y_BIT;
    ADC_PORT_2->SEL0 |= (ADC_PHOTO | ADC_PHOTO_2);
    ADC_PORT_2->SEL1 |= (ADC_PHOTO | ADC_PHOTO_2);
}

/*!
 * \brief Configures ADC14 for Use
 *
 * Sequence-of-channels allows potentiometer result
 * to be stored in MEM1 and photoresistor result to
 * be stored in MEM2
 *
 * \return None
 */
void configureADC14(void)
{
    /* Configure ADC (CTL0 and CTL1) registers for:
     *      clock source - default MODCLK, clock prescale 1:1,
     *      sample input signal (SHI) source - software controlled (ADC14SC),
     *      Pulse Sample mode with sampling period of 16 ADC14CLK cycles,
     *      Single-channel, single-conversion mode, 12-bit resolution,
     *      ADC14 conversion start address ADC14MEM1, and Low-power mode
     */
    ADC14->CTL0 = ADC14_CTL0_SHP                // Pulse Sample Mode
                    | ADC14_CTL0_SHT0__16       // 16 cycle sample-and-hold time (for ADC14MEM1)
                    | ADC14_CTL0_PDIV__1        // Predivide by 1
                    | ADC14_CTL0_DIV__1         // /1 clock divider
                    | ADC14_CTL0_SHS_0          // ADC14SC bit sample-and-hold source select
                    | ADC14_CTL0_SSEL__MODCLK   // clock source select MODCLK
                    | ADC14_CTL0_CONSEQ_1       // Sequence-of-channels
                    | ADC14_CTL0_ON;            // ADC14 on

    ADC14->CTL1 = ADC14_CTL1_RES__10BIT         // 10-bit conversion results
            | (0x1 << ADC14_CTL1_CSTARTADD_OFS) // ADC14MEM1 - conversion start address
            | ADC14_CTL1_PWRMD_2;               // Low-power mode

    // Configure ADC14MCTL1 as storage register for result of A15 P6.0
    //          Single-ended mode with Vref+ = Vcc and Vref- = Vss,
    //          Input channel - A15, and comparator window disabled
    ADC14->MCTL[1] = 0x0000000F;

    // Configure ADC14MCTL2 as storage register for result of A14 P6.1
    //          Single-ended mode with Vref+ = Vcc and Vref- = Vss,
    //          Input channel - A14, and comparator window disabled
    ADC14->MCTL[2] = 0x000000E;

    // Make sure photoVal is greater than 200 upon start
    photoVal = 1000;
    // Setup 1st photoresistor ADC (A1)
    ADC14->MCTL[3] = 0x0000001;

    // Make sure photoVal2 is greater than 200 upon start
    photoVal2 = 1000;
    // Setup 2nd photoresistor ADC, indicating end of sequence (A3)
    ADC14->MCTL[4] = 0x0000083;

    // Enable ADC conversion complete interrupt
    ADC14->IER0 = 0x0000001E;

    // Enable ADC interrupt in NVIC module
    NVIC->ISER[0] |= (1 << ADC14_IRQn);
}

void adcSample(void)
{
    ADC14->CTL0 |= 0x00000003;
}

/*!
 * \brief ADC14 interrupt service routine
 *
 * Acts upon potentiometer or photoresistor input, adjusting
 * the stepper speed and servo angle accordingly
 *
 * \return None
 */
void ADC14_IRQHandler(void)
{
    // Check if interrupt triggered by ADC14MEM1 conversion value loaded
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG1)
    {
        xVal = ADC14->MEM[1];
        resultReady = true;
        // not necessary to clear flag because reading ADC14MEMx clears flag
    }
    // Check if interrupt triggered by ADC14MEM2 conversion value loaded
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG2)
    {
        yVal = ADC14->MEM[2];
        resultReady = true;
        // not necessary to clear flag because reading ADC14MEMx clears flag
    }
    // Check if interrupt triggered by ADC14MEM1 conversion value loaded
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG3)
    {
        photoVal = ADC14->MEM[3];
        resultReady = true;
        // not necessary to clear flag because reading ADC14MEMx clears flag
    }

    // Check if interrupt triggered by ADC14MEM1 conversion value loaded
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG4)
    {
        photoVal2 = ADC14->MEM[4];
        resultReady = true;
        // not necessary to clear flag because reading ADC14MEMx clears flag
    }
}
