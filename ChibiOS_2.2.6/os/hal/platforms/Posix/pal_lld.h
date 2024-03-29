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

/**
 * @file    Posix/pal_lld.h
 * @brief   Posix low level simulated PAL driver header.
 *
 * @addtogroup POSIX_PAL
 * @{
 */

#ifndef _PAL_LLD_H_
#define _PAL_LLD_H_

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Unsupported modes and specific modes                                      */
/*===========================================================================*/

#undef PAL_MODE_INPUT_PULLUP
#undef PAL_MODE_INPUT_PULLDOWN
#undef PAL_MODE_OUTPUT_OPENDRAIN
#undef PAL_MODE_INPUT_ANALOG

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @brief   VIO port structure.
 */
typedef struct {
  /**
   * @brief   VIO_LATCH register.
   * @details This register represents the output latch of the VIO port.
   */
  uint32_t          latch;
  /**
   * @brief   VIO_PIN register.
   * @details This register represents the logical level at the VIO port
   *          pin level.
   */
  uint32_t          pin;
  /**
   * @brief   VIO_DIR register.
   * @details Direction of the VIO port bits, 0=input, 1=output.
   */
  uint32_t          dir;
} sim_vio_port_t;

/**
 * @brief   Virtual I/O ports static initializer.
 * @details An instance of this structure must be passed to @p palInit() at
 *          system startup time in order to initialized the digital I/O
 *          subsystem. This represents only the initial setup, specific pads
 *          or whole ports can be reprogrammed at later time.
 */
typedef struct {
  /**
   * @brief Virtual port 1 setup data.
   */
  sim_vio_port_t    VP1Data;
  /**
   * @brief Virtual port 2 setup data.
   */
  sim_vio_port_t    VP2Data;
} PALConfig;

/**
 * @brief   Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 32

/**
 * @brief   Whole port mask.
 * @brief   This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT ((ioportmask_t)0xFFFFFFFF)

/**
 * @brief   Digital I/O port sized unsigned type.
 */
typedef uint32_t ioportmask_t;

/**
 * @brief   Port Identifier.
 */
typedef sim_vio_port_t *ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/*===========================================================================*/

/**
 * @brief   VIO port 1 identifier.
 */
#define IOPORT1         (&vio_port_1)

/**
 * @brief   VIO port 2 identifier.
 */
#define IOPORT2         (&vio_port_2)

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief   Low level PAL subsystem initialization.
 *
 * @param[in] config    architecture-dependent ports configuration
 */
#define pal_lld_init(config) {                                              \
  vio_port_1 = (config)->VP1Data;                                           \
  vio_port_2 = (config)->VP2Data;                                           \
}

/**
 * @brief   Reads the physical I/O port states.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 *
 * @param[in] port      port identifier
 * @return              The port bits.
 */
#define pal_lld_readport(port) ((port)->pin)

/**
 * @brief   Reads the output latch.
 * @details The purpose of this function is to read back the latched output
 *          value.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 *
 * @param[in] port      port identifier
 * @return              The latched logical states.
 */
#define pal_lld_readlatch(port) ((port)->latch)

/**
 * @brief   Writes a bits mask on a I/O port.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be written on the specified port
 */
#define pal_lld_writeport(port, bits) ((port)->latch = (bits))

/**
 * @brief   Pads group mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] mode      group mode
 */
#define pal_lld_setgroupmode(port, mask, mode) \
  _pal_lld_setgroupmode(port, mask, mode)

#if !defined(__DOXYGEN__)
extern sim_vio_port_t vio_port_1;
extern sim_vio_port_t vio_port_2;
extern const PALConfig pal_default_config;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void _pal_lld_setgroupmode(ioportid_t port,
                             ioportmask_t mask,
                             uint_fast8_t mode);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PAL */

#endif /* _PAL_LLD_H_ */

/** @} */
