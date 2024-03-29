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

/*
 * LPC13xx drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the driver
 * is enabled in halconf.h.
 *
 * IRQ priorities:
 * 7...0        Lowest...highest.
 */

/*
 * HAL driver system settings.
 */
#define LPC13xx_PLLCLK_SOURCE               SYSPLLCLKSEL_SYSOSC
#define LPC13xx_SYSPLL_MUL                  6
#define LPC13xx_SYSPLL_DIV                  4
#define LPC13xx_MAINCLK_SOURCE              SYSMAINCLKSEL_PLLOUT
#define LPC13xx_SYSABHCLK_DIV               1

/*
 * ADC driver system settings.
 */

/*
 * CAN driver system settings.
 */

/*
 * PWM driver system settings.
 */

/*
 * SERIAL driver system settings.
 */
#define LPC13xx_SERIAL_USE_UART0            TRUE
#define LPC13xx_SERIAL_FIFO_PRELOAD         16
#define LPC13xx_SERIAL_UART0CLKDIV          1
#define LPC13xx_SERIAL_UART0_IRQ_PRIORITY   3

/*
 * SPI driver system settings.
 */
#define LPC13xx_SPI_USE_SSP0                TRUE
#define LPC13xx_SPI_SSP0CLKDIV              1
#define LPC13xx_SPI_SSP0_IRQ_PRIORITY       5
#define LPC13xx_SPI_SSP_ERROR_HOOK(spip)    chSysHalt()
#define LPC13xx_SPI_SCK0_SELECTOR           SCK0_IS_PIO2_11
