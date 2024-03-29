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
 * @file    LPC214x/spi_lld.c
 * @brief   LPC214x low level SPI driver code.
 *
 * @addtogroup SPI
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if LPC214x_SPI_USE_SSP || defined(__DOXYGEN__)
/** @brief SPI1 driver identifier.*/
SPIDriver SPID1;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Preloads the transmit FIFO.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
static void ssp_fifo_preload(SPIDriver *spip) {
  SSP *ssp = spip->spd_ssp;
  uint32_t n = spip->spd_txcnt > LPC214x_SSP_FIFO_DEPTH ?
               LPC214x_SSP_FIFO_DEPTH : spip->spd_txcnt;

  while(((ssp->SSP_SR & SR_TNF) != 0) && (n > 0)) {
    if (spip->spd_txptr != NULL) {
      if ((ssp->SSP_CR0 & CR0_DSSMASK) > CR0_DSS8BIT)
        ssp->SSP_DR = *(uint16_t *)spip->spd_txptr++;
      else
        ssp->SSP_DR = *(uint8_t *)spip->spd_txptr++;
    }
    else
      ssp->SSP_DR = 0xFFFFFFFF;
    n--;
    spip->spd_txcnt--;
  }
}

#if defined(__GNUC__)
__attribute__((noinline))
#endif
/**
 * @brief   Common IRQ handler.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
static void serve_interrupt(SPIDriver *spip) {
  SSP *ssp = spip->spd_ssp;

  if ((ssp->SSP_MIS & MIS_ROR) != 0) {
    /* The overflow condition should never happen because priority is given
       to receive but a hook macro is provided anyway...*/
    LPC214x_SPI_SSP_ERROR_HOOK();
  }
  ssp->SSP_ICR = ICR_RT | ICR_ROR;
  while ((ssp->SSP_SR & SR_RNE) != 0) {
    if (spip->spd_rxptr != NULL) {
      if ((ssp->SSP_CR0 & CR0_DSSMASK) > CR0_DSS8BIT)
        *(uint16_t *)spip->spd_rxptr++ = ssp->SSP_DR;
      else
        *(uint8_t *)spip->spd_rxptr++ = ssp->SSP_DR;
    }
    else
      (void)ssp->SSP_DR;
    if (--spip->spd_rxcnt == 0) {
      chDbgAssert(spip->spd_txcnt == 0,
                  "spi_serve_interrupt(), #1", "counter out of synch");
      /* Stops the IRQ sources.*/
      ssp->SSP_IMSC = 0;
      /* Portable SPI ISR code defined in the high level driver, note, it is
         a macro.*/
      _spi_isr_code(spip);
      return;
    }
  }
  ssp_fifo_preload(spip);
  if (spip->spd_txcnt == 0)
    ssp->SSP_IMSC = IMSC_ROR | IMSC_RT | IMSC_RX;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if LPC214x_SPI_USE_SSP || defined(__DOXYGEN__)
/**
 * @brief   SPI1 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPI1IrqHandler) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SPID1);
  VICVectAddr = 0;

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SPI driver initialization.
 *
 * @notapi
 */
void spi_lld_init(void) {

#if LPC214x_SPI_USE_SSP
  spiObjectInit(&SPID1);
  SPID1.spd_ssp = SSPBase;
  SetVICVector(SPI1IrqHandler, LPC214x_SPI_SSP_IRQ_PRIORITY, SOURCE_SPI1);
#endif
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_start(SPIDriver *spip) {

  if (spip->spd_state == SPI_STOP) {
    /* Clock activation.*/
#if LPC214x_SPI_USE_SSP
    if (&SPID1 == spip) {
      PCONP = (PCONP & PCALL) | PCSPI1;
      VICIntEnable = INTMASK(SOURCE_SPI1);
    }
#endif
  }
  /* Configuration.*/
  spip->spd_ssp->SSP_CR1  = 0;
  /* Emptying the receive FIFO, it happens to not be empty while debugging.*/
  while (spip->spd_ssp->SSP_SR & SR_RNE)
    (void) spip->spd_ssp->SSP_DR;
  spip->spd_ssp->SSP_ICR  = ICR_RT | ICR_ROR;
  spip->spd_ssp->SSP_CR0  = spip->spd_config->spc_cr0;
  spip->spd_ssp->SSP_CPSR = spip->spd_config->spc_cpsr;
  spip->spd_ssp->SSP_CR1  = CR1_SSE;
}

/**
 * @brief   Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_stop(SPIDriver *spip) {

  if (spip->spd_state != SPI_STOP) {
    spip->spd_ssp->SSP_CR1  = 0;
    spip->spd_ssp->SSP_CR0  = 0;
    spip->spd_ssp->SSP_CPSR = 0;
#if LPC214x_SPI_USE_SSP
    if (&SPID1 == spip) {
      PCONP = (PCONP & PCALL) & ~PCSPI1;
      VICIntEnClear = INTMASK(SOURCE_SPI1);
    }
#endif
  }
}

/**
 * @brief   Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_select(SPIDriver *spip) {

  palClearPad(spip->spd_config->spc_ssport, spip->spd_config->spc_sspad);
}

/**
 * @brief   Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_unselect(SPIDriver *spip) {

  palSetPad(spip->spd_config->spc_ssport, spip->spd_config->spc_sspad);
}

/**
 * @brief   Ignores data on the SPI bus.
 * @details This function transmits a series of idle words on the SPI bus and
 *          ignores the received data. This function can be invoked even
 *          when a slave select signal has not been yet asserted.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 *
 * @notapi
 */
void spi_lld_ignore(SPIDriver *spip, size_t n) {

  spip->spd_rxptr = NULL;
  spip->spd_txptr = NULL;
  spip->spd_rxcnt = spip->spd_txcnt = n;
  ssp_fifo_preload(spip);
  spip->spd_ssp->SSP_IMSC = IMSC_ROR | IMSC_RT | IMSC_TX | IMSC_RX;
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This asynchronous function starts a simultaneous transmit/receive
 *          operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_exchange(SPIDriver *spip, size_t n,
                      const void *txbuf, void *rxbuf) {

  spip->spd_rxptr = rxbuf;
  spip->spd_txptr = txbuf;
  spip->spd_rxcnt = spip->spd_txcnt = n;
  ssp_fifo_preload(spip);
  spip->spd_ssp->SSP_IMSC = IMSC_ROR | IMSC_RT | IMSC_TX | IMSC_RX;
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf) {

  spip->spd_rxptr = NULL;
  spip->spd_txptr = txbuf;
  spip->spd_rxcnt = spip->spd_txcnt = n;
  ssp_fifo_preload(spip);
  spip->spd_ssp->SSP_IMSC = IMSC_ROR | IMSC_RT | IMSC_TX | IMSC_RX;
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This asynchronous function starts a receive operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {

  spip->spd_rxptr = rxbuf;
  spip->spd_txptr = NULL;
  spip->spd_rxcnt = spip->spd_txcnt = n;
  ssp_fifo_preload(spip);
  spip->spd_ssp->SSP_IMSC = IMSC_ROR | IMSC_RT | IMSC_TX | IMSC_RX;
}

/**
 * @brief   Exchanges one frame using a polled wait.
 * @details This synchronous function exchanges one frame using a polled
 *          synchronization method. This function is useful when exchanging
 *          small amount of data on high speed channels, usually in this
 *          situation is much more efficient just wait for completion using
 *          polling than suspending the thread waiting for an interrupt.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] frame     the data frame to send over the SPI bus
 * @return              The received data frame from the SPI bus.
 */
uint16_t spi_lld_polled_exchange(SPIDriver *spip, uint16_t frame) {

  spip->spd_ssp->SSP_DR = (uint32_t)frame;
  while ((spip->spd_ssp->SSP_SR & SR_RNE) == 0)
    ;
  return (uint16_t)spip->spd_ssp->SSP_DR;
}

#endif /* HAL_USE_SPI */

/** @} */
