/*! \file */
/*!
 * csLFXT.h
 *
 * Description: Basic driver for configuring LFXT clock source on MSP432P4111
 *               Launchpad.
 *
 *  Created on: 12/28/2022
 *      Author: Vineet Ranade & Yao Xiong
 */

#ifndef CLOCKS_H_
#define CLOCKS_H_

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

/*!
 * \brief This function configures REFO as clock source for ACLK and DCO
 *        as clock source for MCLK and SMCLK
 *
 * This function configures DCO as clock source for MCLK and SMCLK with
 * frequency set to 12 MHz and configures REFO as clock source for ACLK
 * with frequency set to 32kHz.
 *
 * Modified CS peripheral registers.
 *
 * \return None
 */
extern void configClocks(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif


#endif /* CLOCKS_H_ */
