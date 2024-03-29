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
 * Setup for STMicroelectronics STM8S-Discovery board.
 */

/*
 * Board identifiers.
 */
#define BOARD_ST_STM8S_DISCOVERY
#define BOARD_NAME      "ST STM8S-Discovery"

/*
 * Board frequencies.
 */
#define HSECLK          16000000

/*
 * MCU model used on the board.
 */
#define STM8S105

/*
 * Pin definitions.
 */
#define PA_OSCIN        1
#define PA_OSCOUT       2

#define PC_TS_KEY       1
#define PC_TS_LOADREF   2
#define PC_TS_SHIELD    3

#define PD_LD10         0
#define PD_SWIM         1
#define PD_TX           5
#define PD_RX           6

/*
 * Port A initial setup.
 */
#define VAL_GPIOAODR    0
#define VAL_GPIOADDR    0               /* All inputs.                      */
#define VAL_GPIOACR1    0xFF            /* All pull-up or push-pull.        */
#define VAL_GPIOACR2    0

/*
 * Port B initial setup.
 */
#define VAL_GPIOBODR    0
#define VAL_GPIOBDDR    0               /* All inputs.                      */
#define VAL_GPIOBCR1    0xFF            /* All push-pull.                   */
#define VAL_GPIOBCR2    0

/*
 * Port C initial setup.
 */
#define VAL_GPIOCODR    0
#define VAL_GPIOCDDR    0               /* All inputs.                      */
#define VAL_GPIOCCR1    0xFF            /* All pull-up.                     */
#define VAL_GPIOCCR2    0

/*
 * Port D initial setup.
 */
#define VAL_GPIODODR    (1 << PD_LD10) | (1 << PD_TX)
#define VAL_GPIODDDR    (1 << PD_LD10) | (1 << PD_TX)
#define VAL_GPIODCR1    0xFF            /* All pull-up.                     */
#define VAL_GPIODCR2    0

/*
 * Port E initial setup.
 */
#define VAL_GPIOEODR    0 
#define VAL_GPIOEDDR    0               /* All inputs.                      */
#define VAL_GPIOECR1    0xFF            /* All pull-up.                     */
#define VAL_GPIOECR2    0

/*
 * Port F initial setup.
 */
#define VAL_GPIOFODR    0
#define VAL_GPIOFDDR    0               /* All inputs.                      */
#define VAL_GPIOFCR1    0xFF            /* All pull-up.                     */
#define VAL_GPIOFCR2    0

/*
 * Port G initial setup.
 */
#define VAL_GPIOGODR    0
#define VAL_GPIOGDDR    0               /* All inputs.                      */
#define VAL_GPIOGCR1    0xFF            /* All pull-up or push-pull.        */
#define VAL_GPIOGCR2    0

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
