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
 * STM8 drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the driver
 * is enabled in halconf.h.
 */

/*
 * HAL general settings.
 */
#define STM8S_NO_CLOCK_INIT             FALSE
#define STM8S_HSI_ENABLED               FALSE
#define STM8S_LSI_ENABLED               TRUE
#define STM8S_HSE_ENABLED               TRUE
#define STM8S_SYSCLK_SOURCE             CLK_SYSSEL_HSE
#define STM8S_HSI_DIVIDER               CLK_HSI_DIV1
#define STM8S_CPU_DIVIDER               CLK_CPU_DIV1
#define STM8S_CAN_DIVIDER_VALUE         1

/*
 * SERIAL driver system settings.
 */
#define STM8S_SERIAL_USE_UART1          FALSE
#define STM8S_SERIAL_USE_UART2          TRUE
#define STM8S_SERIAL_USE_UART3          FALSE

/*
 * SPI driver system settings.
 */
#define STM8S_SPI_USE_SPI               TRUE
#define STM8S_SPI_ERROR_HOOK(spip)      chSysHalt()
