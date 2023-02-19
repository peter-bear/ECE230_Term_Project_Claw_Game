/*!
 * lcd.h
 *
 *      Description: Helper file for LCD library. For Hitachi HD44780 parallel LCD
 *               in 4-bit mode. Assumes the following connections:
 *               P5.7 <-----> RS
 *               P5.5 <-----> E
 *                            R/W --->GND
 *                P4  <-----> DB
 *
 *          This module uses SysTick timer for delays.
 *
 *      Author: Vineet Ranade & Yao Xiong
 */

#ifndef LCD_H_
#define LCD_H_

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

#define LCD_DB_PORT         P4
#define LCD_RS_PORT         P5
#define LCD_EN_PORT         P5
#define LCD_RS_MASK         BIT7
#define LCD_EN_MASK         BIT5

#define CTRL_MODE           0
#define DATA_MODE           1
#define FOUR_BIT            0
#define EIGHT_BIT           1
#define LINE1_OFFSET        0x0
#define LINE2_OFFSET        0x40
#define LINE1_START_POS     1
#define LINE2_START_POS     17
#define NUM_SPOTS           32
#define NUM_PER_LINE        16

/* Instruction masks */
#define CLEAR_DISPLAY_MASK  0x01
#define RETURN_HOME_MASK    0x02
#define ENTRY_MODE_MASK     0x04
// Complete values for instruction masks
#define DISPLAY_CTRL_MASK   0x08
#define CURSOR_SHIFT_MASK   0x10
#define FUNCTION_SET_MASK   0x20
#define SET_CGRAM_MASK      0x40
#define SET_CURSOR_MASK     0x80

/* Field masks for instructions:
 * DL   = 1: 8 bits, DL = 0: 4 bits
 * N    = 1: 2 lines, N = 0: 1 line
 * S/C  = 1: Display shift
 * S/C  = 0: Cursor move
 * F    = 1: 5 x 10 dots, F = 0: 5 x 8 dots
 * R/L  = 1: Shift to the right
 * R/L  = 0: Shift to the left
 * D    = 1: Display On, D = 0: Display Off
 * C    = 1: Cursor On, D = 0: Cursor Off
 * I/D  = 1: Increment
 * I/D  = 0: Decrement
 * B    = 1: Cursor blink On, D = 0: Cursor blink Off
 * S    = 1: Accompanies display shift
 * BF   = 1: Internally operating
 * BF   = 0: Instructions acceptable
 */
#define DL_FLAG_MASK        0x10
#define N_FLAG_MASK         0x08
#define SC_FLAG_MASK        0x08
// Complete values for instruction field masks
#define F_FLAG_MASK         0x04
#define RL_FLAG_MASK        0x04
#define D_FLAG_MASK         0x04
#define C_FLAG_MASK         0x02
#define ID_FLAG_MASK        0x02
#define B_FLAG_MASK         0x01
#define S_FLAG_MASK         0x80

#define CLK_FREQUENCY       12000000

#define START_DISP_POS_X  4
#define START_DISP_POS_Y  20

#define X 0
#define Y 1
#define Z 2

#define ASCII_OFFSET      48

volatile uint32_t i;

/*!
 *
 *  \brief This function merely calls subroutines for LCD Setup
 *
 *  \param  None
 *
 *  \return None
 */
extern void setupLCD(void);

/*!
 *
 *  \brief This function configures the selected pins for an LCD
 *
 *  This function configures the selected pins as output pins to interface
 *      with a Hitachi HD44780 LCD in 8-bit mode. Also initializes sysTickDelay
 *      library based on system clock frequency.
 *
 *  \param clkFreq is the frequency of the system clock (MCLK) in Hz
 *
 *  Modified bits of \b P2DIR register and \b P4DIR register, and bits of
 *      \b P2SEL and \b P4SEL registers.
 *
 *  \return None
 */
extern void configLCD(uint32_t clkFreq);

/*!
 *  \brief This function initializes LCD
 *
 *  This function generates initialization sequence for LCD for 8-bit mode.
 *      Delays set by worst-case 2.7 V
 *
 *  \return None
 */
extern void initLCD(void);

/*!
 *  \brief This function prints character to current cursor position
 *
 *  This function prints ASCII character to current cursor position on LCD.
 *
 *  \param character is the character to display on LCD
 *
 *  \return None
 */
extern void printChar(char character);

/*!
 *  \brief This function clears display.
 *
 *  This function utilizes the clear display command
 *
 *  \param  None
 *
 *  \return None
 */
extern void clearDisplay(void);

/*!
 *  \brief This function moves cursor to start of line 1
 *
 *  This function utilizes the set cursor mask
 *
 *  \param  None
 *
 *  \return None
 */
extern void returnHome();

extern void updateDispVal(char* str);

/*!
 *  \brief This function moves cursor.
 *
 *  This function utilizes the set cursor mask
 *
 *  \param  pos from 1-16
 *
 *  \return None
 */
extern void moveCursor(uint8_t pos);

/*!
 *  \brief This function sets up the LCD to show accelerometer data.
 *
 *  This function mainly writes the more static characters to LCD.
 *
 *  \param  None
 *
 *  \return None
 */
extern void showLCD(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* LCD_H_ */
