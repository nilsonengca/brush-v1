/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for STMicroelectronics STM8L-Discovery board.
 */

/*
 * Board identifiers.
 */
#define BOARD_ST_STM8L_DISCOVERY
#define BOARD_NAME          "ST STM8L-Discovery"

/*
 * Board frequencies and bypass modes.
 *
 * The bypass must be set to TRUE if the chip is driven by an external
 * oscillator rather than a crystal. Frequency must be set to zero if
 * the clock source is not used at all.
 * The following constants are used by the HAL low level driver for
 * correct clock initialization.
 */
#define HSECLK              0
#define HSEBYPASS           FALSE
#define LSECLK              32768
#define LSEBYPASS           FALSE

/*
 * MCU model used on the board.
 */
#define STM8L152C6
#define STM8L15X_MD

/*
 * Pin definitions.
 */
#define PA_OSC_IN           2
#define PA_OSC_OUT          3
#define PA_LCD_COM0         4
#define PA_LCD_COM1         5
#define PA_LCD_COM2         6
#define PA_LCD_SEG0         7

#define PB_LCD_SEG10        0
#define PB_LCD_SEG11        1
#define PB_LCD_SEG12        2
#define PB_LCD_SEG13        3
#define PB_LCD_SEG14        4
#define PB_LCD_SEG15        5
#define PB_LCD_SEG16        6
#define PB_LCD_SEG17        7

#define PC_UNUSED           0
#define PC_BUTTON           1
#define PC_LCD_SEG22        2
#define PC_LCD_SEG23        3
#define PC_IDD_CNT_EN       4
#define PC_LED4             7

#define PD_LCD_SEG7         0
#define PD_LCD_COM3         1
#define PD_LCD_SEG8         2
#define PD_LCD_SEG9         3
#define PD_LCD_SEG18        4
#define PD_LCD_SEG19        5
#define PD_LCD_SEG20        6
#define PD_LCD_SEG21        7

#define PE_LCD_SEG1         0
#define PE_LCD_SEG2         1
#define PE_LCD_SEG3         2
#define PE_LCD_SEG4         3
#define PE_LCD_SEG5         4
#define PE_LCD_SEG6         5
#define PE_IDD_WAKEUP       6
#define PE_LED3             7

#define PF0_IDD_MEASUREMENT 0

/*
 * Port A initial setup.
 */
#define VAL_GPIOAODR        0
#define VAL_GPIOADDR        0               /* All inputs.                  */
#define VAL_GPIOACR1        0xFF            /* All pull-up/push-pull.       */
#define VAL_GPIOACR2        0

/*
 * Port B initial setup.
 */
#define VAL_GPIOBODR        0
#define VAL_GPIOBDDR        0               /* All inputs.                  */
#define VAL_GPIOBCR1        0xFF            /* All pull-up/push-pull.       */
#define VAL_GPIOBCR2        0

/*
 * Port C initial setup.
 */
#define VAL_GPIOCODR        0
#define VAL_GPIOCDDR        (1 << PC_LED4)
#define VAL_GPIOCCR1        0xFF            /* All pull-up/push-pull.       */
#define VAL_GPIOCCR2        0

/*
 * Port D initial setup.
 */
#define VAL_GPIODODR        0
#define VAL_GPIODDDR        0               /* All inputs.                  */
#define VAL_GPIODCR1        0xFF            /* All pull-up/push-pull.       */
#define VAL_GPIODCR2        0

/*
 * Port E initial setup.
 */
#define VAL_GPIOEODR        0
#define VAL_GPIOEDDR        (1 << PE_LED3)
#define VAL_GPIOECR1        0xFF            /* All pull-up/push-pull.       */
#define VAL_GPIOECR2        0

/*
 * Port F initial setup.
 */
#define VAL_GPIOFODR        0
#define VAL_GPIOFDDR        0               /* All inputs.                  */
#define VAL_GPIOFCR1        0xFF            /* All pull-up/push-pull.       */
#define VAL_GPIOFCR2        0

/*
 * TIM2-update ISR segment code. This code is injected into the appropriate
 * ISR by the HAL.
 */
#define _TIM2_UPDATE_ISR() {                                                \
  if (TIM2->SR1 & TIM_SR1_UIF) {                                            \
    chSysLockFromIsr();                                                     \
    chSysTimerHandlerI();                                                   \
    chSysUnlockFromIsr();                                                   \
    TIM2->SR1 = 0;                                                          \
  }                                                                         \
}

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
