/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2018 Arm Limited (or its affiliates). All 
 * rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        05. February 2018
 * $Revision:    V2.6
 *
 * Project:      MCI Driver Definitions for ST STM32F4xx
 * -------------------------------------------------------------------------- */

#ifndef __MCI_STM32F4XX_H
#define __MCI_STM32F4XX_H

#include "Driver_MCI.h"
#include "stm32f4xx_hal.h"

#include "RTE_Components.h"
#if   defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
#include "RTE_Device.h"
#elif defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
#include "MX_Device.h"
#else
#error "::Device:STM32Cube Framework: not selected in RTE"
#endif

#include <string.h>

#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
  #if (defined(RTE_Drivers_MCI0) && (RTE_SDIO == 0))
    #error "SDIO not configured in RTE_Device.h!"
  #endif

  #if ((RTE_SDIO_RX_DMA == 0) || (RTE_SDIO_TX_DMA == 0))
    #error "SDIO requires Rx and Tx DMA! Enable Rx and Tx DMA in RTE_Device.h!"
  #endif

  #if (RTE_SDIO_RX_DMA_STREAM == RTE_SDIO_TX_DMA_STREAM)
    #error "SDIO requires different Rx and Tx DMA Steams! Please check Rx and Tx DMA Stream configuration in RTE_Device.h!"
  #endif

/* Macro definitions */
#define _GPIO_PIN_x(x)                   GPIO_PIN_##x
#define  GPIO_PIN_x(x)                  _GPIO_PIN_x(x)

#define  DMA_PRIORITY(x)                ((x == 0) ? DMA_PRIORITY_LOW       : \
                                        ((x == 1) ? DMA_PRIORITY_MEDIUM    : \
                                        ((x == 2) ? DMA_PRIORITY_HIGH      : \
                                        ((x == 3) ? DMA_PRIORITY_VERY_HIGH : 0xFFFFFFFFU))))

#define _DMA_CHANNEL_x(x)                DMA_CHANNEL_##x
#define  DMA_CHANNEL_x(x)               _DMA_CHANNEL_x(x)

#define _DMAx_STREAMy(x, y)              DMA##x##_Stream##y
#define  DMAx_STREAMy(x, y)             _DMAx_STREAMy(x, y)

#define _DMAx_STREAMy_IRQ(x, y)          DMA##x##_Stream##y##_IRQHandler
#define  DMAx_STREAMy_IRQ(x, y)         _DMAx_STREAMy_IRQ(x, y)

#define _DMAx_STREAMy_IRQn(x, y)         DMA##x##_Stream##y##_IRQn
#define  DMAx_STREAMy_IRQn(x, y)        _DMAx_STREAMy_IRQn(x, y)

/* DMA SDIO_TX */
#define MX_SDIO_TX_DMA_Instance         DMAx_STREAMy(RTE_SDIO_TX_DMA_NUMBER, RTE_SDIO_TX_DMA_STREAM)
#define MX_SDIO_TX_DMA_Channel          DMA_CHANNEL_x(RTE_SDIO_TX_DMA_CHANNEL)
#define MX_SDIO_TX_DMA_Priority         DMA_PRIORITY(RTE_SDIO_TX_DMA_PRIORITY)
#define SDIO_TX_DMA_Handler             DMAx_STREAMy_IRQ(RTE_SDIO_TX_DMA_NUMBER, RTE_SDIO_TX_DMA_STREAM)
#define SDIO_TX_DMA_IRQn                DMAx_STREAMy_IRQn(RTE_SDIO_TX_DMA_NUMBER, RTE_SDIO_TX_DMA_STREAM)

/* DMA SDIO_RX */
#define MX_SDIO_RX_DMA_Instance         DMAx_STREAMy(RTE_SDIO_RX_DMA_NUMBER, RTE_SDIO_RX_DMA_STREAM)
#define MX_SDIO_RX_DMA_Channel          DMA_CHANNEL_x(RTE_SDIO_RX_DMA_CHANNEL)
#define MX_SDIO_RX_DMA_Priority         DMA_PRIORITY(RTE_SDIO_RX_DMA_PRIORITY)
#define SDIO_RX_DMA_Handler             DMAx_STREAMy_IRQ(RTE_SDIO_RX_DMA_NUMBER, RTE_SDIO_RX_DMA_STREAM)
#define SDIO_RX_DMA_IRQn                DMAx_STREAMy_IRQn(RTE_SDIO_RX_DMA_NUMBER, RTE_SDIO_RX_DMA_STREAM)

  #if (RTE_SDIO_BUS_WIDTH_4)
    #define MX_SDIO_D0_Pin              1
    #define MX_SDIO_D1_Pin              1
    #define MX_SDIO_D2_Pin              1
    #define MX_SDIO_D3_Pin              1
  #endif
  #if (RTE_SDIO_BUS_WIDTH_8)
    #define MX_SDIO_D4_Pin              1
    #define MX_SDIO_D5_Pin              1
    #define MX_SDIO_D6_Pin              1
    #define MX_SDIO_D7_Pin              1
  #endif

  #if (RTE_SDIO_CD_PIN_EN)
    #define MX_MemoryCard_CD_Pin        1
    #define MX_MemoryCard_CD_GPIOx      RTE_SDIO_CD_PORT
    #define MX_MemoryCard_CD_GPIO_Pin   GPIO_PIN_x(RTE_SDIO_CD_PIN)
    #define MX_MemoryCard_CD_GPIO_PuPd  GPIO_NOPULL
    #define MemoryCard_CD_Pin_Active    ((RTE_SDIO_CD_ACTIVE == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET)
  #endif

  #if (RTE_SDIO_WP_EN)
    #define MX_MemoryCard_WP_Pin        1
    #define MX_MemoryCard_WP_GPIOx      RTE_SDIO_WP_PORT
    #define MX_MemoryCard_WP_GPIO_Pin   GPIO_PIN_x(RTE_SDIO_WP_PIN)
    #define MX_MemoryCard_WP_GPIO_PuPd  GPIO_NOPULL
    #define MemoryCard_WP_Pin_Active    ((RTE_SDIO_WP_ACTIVE == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET)
  #endif

#else /* MX_Device.h */

#define EXPAND_SYMBOL(pin, ext)         MX_##pin##_##ext
#define MX_SYM(pin, ext)                EXPAND_SYMBOL(pin, ext)

  #if defined(MX_MemoryCard_CD)
    #define MX_MemoryCard_CD_Pin        1
    #define MX_MemoryCard_CD_GPIOx      MX_SYM(MX_MemoryCard_CD, GPIOx)
    #define MX_MemoryCard_CD_GPIO_Pin   MX_SYM(MX_MemoryCard_CD, GPIO_Pin)
    #define MX_MemoryCard_CD_GPIO_PuPd  MX_SYM(MX_MemoryCard_CD, GPIO_PuPd)
    #define MX_MemoryCard_CD_GPIO_Mode  MX_SYM(MX_MemoryCard_CD, GPIO_Mode)
  #endif

  #if defined(MX_MemoryCard_WP)
    #define MX_MemoryCard_WP_Pin        1
    #define MX_MemoryCard_WP_GPIOx      MX_SYM(MX_MemoryCard_WP, GPIOx)
    #define MX_MemoryCard_WP_GPIO_Pin   MX_SYM(MX_MemoryCard_WP, GPIO_Pin)
    #define MX_MemoryCard_WP_GPIO_PuPd  MX_SYM(MX_MemoryCard_WP, GPIO_PuPd)
    #define MX_MemoryCard_WP_GPIO_Mode  MX_SYM(MX_MemoryCard_WP, GPIO_Mode)
  #endif

#endif /* RTE_DEVICE_FRAMEWORK_CLASSIC */

/* Define 4-bit data bus width */
#if defined(MX_SDIO_D0_Pin) && defined(MX_SDIO_D1_Pin) && defined(MX_SDIO_D2_Pin) && defined(MX_SDIO_D3_Pin)
  #define MCI_BUS_WIDTH_4   1U
#else
  #define MCI_BUS_WIDTH_4   0U
#endif

/* Define 8-bit data bus width */
#if defined(MX_SDIO_D0_Pin) && defined(MX_SDIO_D1_Pin) && defined(MX_SDIO_D2_Pin) && defined(MX_SDIO_D3_Pin) && \
    defined(MX_SDIO_D4_Pin) && defined(MX_SDIO_D5_Pin) && defined(MX_SDIO_D6_Pin) && defined(MX_SDIO_D7_Pin)
  #define MCI_BUS_WIDTH_8   1U
#else
  #define MCI_BUS_WIDTH_8   0U
#endif

/* Define Card Detect pin existence */
#if defined(MX_MemoryCard_CD_Pin)
  #define MCI_CD_PIN        1U
#else
  #define MCI_CD_PIN        0U
#endif

/* Define Write Protect pin existence */
#if defined(MX_MemoryCard_WP_Pin)
  #define MCI_WP_PIN        1U
#else
  #define MCI_WP_PIN        0U
#endif

/* SDIO Adapter Clock definition */
#define SDIOCLK             48000000U    /* SDIO adapter clock */

#ifndef SDIO_MASK_STBITERRIE
#define SDIO_MASK_STBITERRIE    0U
#endif
#ifndef SDIO_STA_STBITERR
#define SDIO_STA_STBITERR       0U
#endif
#ifndef SDIO_ICR_STBITERRC
#define SDIO_ICR_STBITERRC      0U
#endif

/* Interrupt clear Mask */
#define SDIO_ICR_BIT_Msk       (SDIO_ICR_CCRCFAILC | \
                                SDIO_ICR_DCRCFAILC | \
                                SDIO_ICR_CTIMEOUTC | \
                                SDIO_ICR_DTIMEOUTC | \
                                SDIO_ICR_TXUNDERRC | \
                                SDIO_ICR_RXOVERRC  | \
                                SDIO_ICR_CMDRENDC  | \
                                SDIO_ICR_CMDSENTC  | \
                                SDIO_ICR_DATAENDC  | \
                                SDIO_ICR_STBITERRC | \
                                SDIO_ICR_DBCKENDC  | \
                                SDIO_ICR_SDIOITC)

/* Error interrupt mask */
#define SDIO_STA_ERR_BIT_Msk   (SDIO_STA_CCRCFAIL | \
                                SDIO_STA_DCRCFAIL | \
                                SDIO_STA_CTIMEOUT | \
                                SDIO_STA_DTIMEOUT | \
                                SDIO_STA_STBITERR)

/* Driver flag definitions */
#define MCI_INIT      ((uint8_t)0x01)   /* MCI initialized           */
#define MCI_POWER     ((uint8_t)0x02)   /* MCI powered on            */
#define MCI_SETUP     ((uint8_t)0x04)   /* MCI configured            */
#define MCI_RESP_LONG ((uint8_t)0x08)   /* Long response expected    */
#define MCI_RESP_CRC  ((uint8_t)0x10)   /* Check response CRC error  */
#define MCI_DATA_XFER ((uint8_t)0x20)   /* Transfer data             */
#define MCI_DATA_READ ((uint8_t)0x40)   /* Read transfer             */
#define MCI_READ_WAIT ((uint8_t)0x80)   /* Read wait operation start */

#define MCI_RESPONSE_EXPECTED_Msk (ARM_MCI_RESPONSE_SHORT      | \
                                   ARM_MCI_RESPONSE_SHORT_BUSY | \
                                   ARM_MCI_RESPONSE_LONG)

#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
/* MCI Input/Output Definition */
typedef const struct _MCI_IO {
  GPIO_TypeDef         *port;           /* IO port                            */
  uint16_t              pin;            /* IO pin                             */
  uint16_t              reserved;       /* Reserved                           */
  uint32_t              pull;           /* IO pin pull resistor               */
  uint32_t              af;             /* IO alternate function              */
} MCI_IO;
#endif

/* MCI Transfer Information Definition */
typedef struct _MCI_XFER {
  uint8_t *buf;                         /* Data buffer                        */
  uint32_t cnt;                         /* Data bytes to transfer             */
} MCI_XFER;

/* MCI Driver State Definition */
typedef struct _MCI_INFO {
  ARM_MCI_SignalEvent_t cb_event;       /* Driver event callback function     */
  ARM_MCI_STATUS        status;         /* Driver status                      */
  uint32_t             *response;       /* Pointer to response buffer         */
  MCI_XFER              xfer;           /* Data transfer description          */
  uint32_t              dctrl;          /* Data control register value        */
  uint32_t              dlen;           /* Data length register value         */
  uint16_t volatile     flags;          /* Driver state flags                 */
  uint16_t              reserved;       /* Reserved                           */
} MCI_INFO;

// Global functions and variables exported by driver .c module
extern void SDIO_IRQHandler     (void);
extern void SDIO_RX_DMA_Handler (void);
extern void SDIO_TX_DMA_Handler (void);

extern ARM_DRIVER_MCI Driver_MCI0;

#endif /* __MCI_STM32F4XX_H */
