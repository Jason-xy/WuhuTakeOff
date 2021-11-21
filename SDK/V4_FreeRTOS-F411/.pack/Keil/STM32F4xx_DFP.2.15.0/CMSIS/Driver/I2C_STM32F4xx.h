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
 * $Date:        4. April 2018
 * $Revision:    V2.9
 *
 * Project:      I2C Driver definitions for ST STM32F4xx
 * -------------------------------------------------------------------------- */

#ifndef __I2C_STM32F4XX_H
#define __I2C_STM32F4XX_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "Driver_I2C.h"
#include "stm32f4xx_hal.h"

#include "RTE_Components.h"
#if   defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
#include "RTE_Device.h"
#elif defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
#include "MX_Device.h"
#else
#error "::Device:STM32Cube Framework: not selected in RTE"
#endif

#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
  #if ((defined(RTE_Drivers_I2C1) || \
        defined(RTE_Drivers_I2C2) || \
        defined(RTE_Drivers_I2C3))   \
       && (RTE_I2C1 == 0)            \
       && (RTE_I2C2 == 0)            \
       && (RTE_I2C3 == 0))
    #error "I2C not configured in RTE_Device.h!"
  #endif

/* RTE macros */
#define _DMA_CHANNEL_x(x)               DMA_CHANNEL_##x
#define  DMA_CHANNEL_x(x)              _DMA_CHANNEL_x(x)

#define  DMA_PRIORITY(x)               ((x == 0) ? DMA_PRIORITY_LOW    : \
                                       ((x == 1) ? DMA_PRIORITY_MEDIUM : \
                                       ((x == 2) ? DMA_PRIORITY_HIGH   : \
                                                   DMA_PRIORITY_VERY_HIGH)))

#define _DMAx_STREAMy(x, y)             DMA##x##_Stream##y
#define  DMAx_STREAMy(x, y)            _DMAx_STREAMy(x, y)

#define _DMAx_STREAMy_IRQ(x, y)         DMA##x##_Stream##y##_IRQHandler
#define  DMAx_STREAMy_IRQ(x, y)        _DMAx_STREAMy_IRQ(x, y)

#define _DMAx_STREAMy_IRQn(x, y)        DMA##x##_Stream##y##_IRQn
#define  DMAx_STREAMy_IRQn(x, y)       _DMAx_STREAMy_IRQn(x, y)

/* I2C1 configuration definitions */
#if defined (RTE_I2C1) && (RTE_I2C1 == 1)
  #if (((RTE_I2C1_RX_DMA != 0) && (RTE_I2C1_TX_DMA == 0)) || \
       ((RTE_I2C1_RX_DMA == 0) && (RTE_I2C1_TX_DMA != 0)))
    #error "I2C1 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h!"
  #endif

  #define MX_I2C1

  #if (RTE_I2C1_RX_DMA == 1)
    #define MX_I2C1_RX_DMA_Instance DMAx_STREAMy(RTE_I2C1_RX_DMA_NUMBER, RTE_I2C1_RX_DMA_STREAM)
    #define MX_I2C1_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_I2C1_RX_DMA_NUMBER, RTE_I2C1_RX_DMA_STREAM)
    #define MX_I2C1_RX_DMA_Channel  DMA_CHANNEL_x(RTE_I2C1_RX_DMA_CHANNEL)
    #define MX_I2C1_RX_DMA_Priority DMA_PRIORITY(RTE_I2C1_RX_DMA_PRIORITY)

    #define I2C1_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_I2C1_RX_DMA_NUMBER, RTE_I2C1_RX_DMA_STREAM)
  #endif
  #if (RTE_I2C1_TX_DMA == 1)
    #define MX_I2C1_TX_DMA_Instance DMAx_STREAMy(RTE_I2C1_TX_DMA_NUMBER, RTE_I2C1_TX_DMA_STREAM)
    #define MX_I2C1_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_I2C1_TX_DMA_NUMBER, RTE_I2C1_TX_DMA_STREAM)
    #define MX_I2C1_TX_DMA_Channel  DMA_CHANNEL_x(RTE_I2C1_TX_DMA_CHANNEL)
    #define MX_I2C1_TX_DMA_Priority DMA_PRIORITY(RTE_I2C1_TX_DMA_PRIORITY)

    #define I2C1_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_I2C1_TX_DMA_NUMBER, RTE_I2C1_TX_DMA_STREAM)
  #endif

  #define MX_I2C1_SCL_GPIOx         RTE_I2C1_SCL_PORT
  #define MX_I2C1_SCL_GPIO_Pin      (1U << RTE_I2C1_SCL_BIT)
  #define MX_I2C1_SCL_GPIO_PuPdOD   GPIO_NOPULL
  #define MX_I2C1_SCL_GPIO_AF       GPIO_AF4_I2C1

  #define MX_I2C1_SDA_GPIOx         RTE_I2C1_SDA_PORT
  #define MX_I2C1_SDA_GPIO_Pin      (1U << RTE_I2C1_SDA_BIT)
  #define MX_I2C1_SDA_GPIO_PuPdOD   GPIO_NOPULL
  #define MX_I2C1_SDA_GPIO_AF       GPIO_AF4_I2C1
#endif

/* I2C2 configuration definitions */
#if defined (RTE_I2C2) && (RTE_I2C2 == 1)

  #if !defined(I2C2)
    #error "I2C2 not available for selected device!"
  #endif

  #if (((RTE_I2C2_RX_DMA != 0) && (RTE_I2C2_TX_DMA == 0)) || \
       ((RTE_I2C2_RX_DMA == 0) && (RTE_I2C2_TX_DMA != 0)))
    #error "I2C2 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h!"
  #endif

  #if (!defined (STM32F401xC)) && (!defined (STM32F401xE)) && (!defined (STM32F411xE)) && (!defined (STM32F446xx))
    /* SDA available on pins: PB11, PF0, PH5 */
    #if (RTE_I2C2_SDA_PORT_ID != 2 && RTE_I2C2_SDA_PORT_ID != 0 && RTE_I2C2_SDA_PORT_ID != 1)
      #error "Only PB11, PF0 and PH5 can be configured as I2C2 SDA on selected device!"
    #endif
  #endif

  #if defined (STM32F410Rx) || defined (STM32F410Cx) || defined (STM32F410Tx)
    /* SCL available on pin: PB10 */
    #if (RTE_I2C2_SCL_PORT_ID != 2)
      #error "Only PB10 can be configured as I2C2 SCL on selected device!"
    #endif

    #if defined (STM32F410Rx)
      /* SDA available on pin: PB3 */
      #if (RTE_I2C2_SDA_PORT_ID != 3)
        #error "Only PB3 can be configured as I2C2 SDA on selected device!"
      #endif
    #endif

    #if defined (STM32F410Cx)
      /* SDA available on pins: PB3, PB9 */
      #if ((RTE_I2C2_SDA_PORT_ID != 3) && (RTE_I2C2_SDA_PORT_ID != 4))
        #error "Only PB3 and PB9 can be configured as I2C2 SDA on selected device!"
      #endif
    #endif

    #if defined (STM32F410Tx)
      /* SDA available on pins: PB3, PB9, PB11 */
      #if ((RTE_I2C2_SDA_PORT_ID != 2) && (RTE_I2C2_SDA_PORT_ID != 3) && (RTE_I2C2_SDA_PORT_ID != 4))
        #error "Only PB3, PB9 and PB11 can be configured as I2C2 SDA on selected device!"
      #endif
    #endif

  #endif

  #if defined (STM32F411xE)
    /* SDA available on pins: PB3, PB9, PB11 */
    #if (RTE_I2C2_SDA_PORT_ID != 3 && RTE_I2C2_SDA_PORT_ID != 4 && RTE_I2C2_SDA_PORT_ID != 2)
      #error "Only PB3, PB9 and PB11 can be configured as I2C2 SDA on selected device!"
    #endif
  #endif

  #if defined (STM32F401xC) || defined (STM32F401xE)
    /* SDA available on pin: PB3 */
    #if (RTE_I2C2_SDA_PORT_ID != 3)
      #error "Only PB3 can be configured as I2C2 SDA on selected device!"
    #endif
  #endif

  #if defined (STM32F401xC) || defined (STM32F401xE) || defined (STM32F411xE)
    /* SCL available on pin:  PB10 */
    #if (RTE_I2C2_SCL_PORT_ID != 2)
      #error "Only PB10 can be configured as I2C2 SCL on selected device!"
    #endif
  #endif

  #if defined (STM32F446xx)
    /* SDA available on pins: PB3, PB11, PF0 */
    #if (RTE_I2C2_SDA_PORT_ID != 3 && RTE_I2C2_SDA_PORT_ID != 2 && RTE_I2C2_SDA_PORT_ID != 0)
      #error "Only PB3, PB11 and PF0 can be configured as I2C2 SDA on selected device!"
    #endif
    /* SCL available on pin:  PB10, PF1 */
    #if (RTE_I2C2_SCL_PORT_ID != 2 && RTE_I2C2_SCL_PORT_ID != 0)
      #error "Only PB10 and PF1 can be configured as I2C2 SCL on selected device!"
    #endif
  #endif

  #if defined (STM32F413xx) || defined (STM32F423xx)
    /* SDA available on pins: PB3, PB9, PB11, PF0 */
    #if (RTE_I2C2_SDA_PORT_ID == 1)
      #error "Only PB3, PB9, PB11 and PF0 can be configured as I2C2 SDA on selected device!"
    #endif
    /* SCL available on pin:  PF1, PB10 */
    #if (RTE_I2C2_SCL_PORT_ID == 1)
      #error "Only PB10 and PF1 can be configured as I2C2 SCL on selected device!"
    #endif
  #endif

  #define MX_I2C2

  #if (RTE_I2C2_RX_DMA == 1)
    #define MX_I2C2_RX_DMA_Instance DMAx_STREAMy(RTE_I2C2_RX_DMA_NUMBER, RTE_I2C2_RX_DMA_STREAM)
    #define MX_I2C2_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_I2C2_RX_DMA_NUMBER, RTE_I2C2_RX_DMA_STREAM)
    #define MX_I2C2_RX_DMA_Channel  DMA_CHANNEL_x(RTE_I2C2_RX_DMA_CHANNEL)
    #define MX_I2C2_RX_DMA_Priority DMA_PRIORITY(RTE_I2C2_RX_DMA_PRIORITY)

    #define I2C2_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_I2C2_RX_DMA_NUMBER, RTE_I2C2_RX_DMA_STREAM)
  #endif
  #if (RTE_I2C2_TX_DMA == 1)
    #define MX_I2C2_TX_DMA_Instance DMAx_STREAMy(RTE_I2C2_TX_DMA_NUMBER, RTE_I2C2_TX_DMA_STREAM)
    #define MX_I2C2_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_I2C2_TX_DMA_NUMBER, RTE_I2C2_TX_DMA_STREAM)
    #define MX_I2C2_TX_DMA_Channel  DMA_CHANNEL_x(RTE_I2C2_TX_DMA_CHANNEL)
    #define MX_I2C2_TX_DMA_Priority DMA_PRIORITY(RTE_I2C2_TX_DMA_PRIORITY)

    #define I2C2_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_I2C2_TX_DMA_NUMBER, RTE_I2C2_TX_DMA_STREAM)
  #endif

  #define MX_I2C2_SCL_GPIOx         RTE_I2C2_SCL_PORT
  #define MX_I2C2_SCL_GPIO_Pin      (1U << RTE_I2C2_SCL_BIT)
  #define MX_I2C2_SCL_GPIO_PuPdOD   GPIO_NOPULL
  #define MX_I2C2_SCL_GPIO_AF       GPIO_AF4_I2C2

  #define MX_I2C2_SDA_GPIOx         RTE_I2C2_SDA_PORT
  #define MX_I2C2_SDA_GPIO_Pin      (1U << RTE_I2C2_SDA_BIT)
  #define MX_I2C2_SDA_GPIO_PuPdOD   GPIO_NOPULL

  #if ((defined (STM32F413xx) || defined (STM32F423xx)) && \
       ((RTE_I2C2_SDA_PORT_ID == 3) || (RTE_I2C2_SDA_PORT_ID == 4)))
  #define MX_I2C2_SDA_GPIO_AF       GPIO_AF9_I2C2
  #else
  #define MX_I2C2_SDA_GPIO_AF       GPIO_AF4_I2C2
#endif

#endif

/* I2C3 configuration definitions */
#if defined (RTE_I2C3) && (RTE_I2C3 == 1)

  #if !defined(I2C3)
    #error "I2C3 not available for selected device!"
  #endif

  #if (((RTE_I2C3_RX_DMA != 0) && (RTE_I2C3_TX_DMA == 0)) || \
       ((RTE_I2C3_RX_DMA == 0) && (RTE_I2C3_TX_DMA != 0)))
    #error "I2C3 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h!"
  #endif


  #if (!defined (STM32F401xC)) && (!defined (STM32F401xE)) && (!defined (STM32F411xE)) && (!defined (STM32F446xx))
    /* SDA available on pins: PC9, PH8 */
    #if (RTE_I2C3_SDA_PORT_ID != 1 && RTE_I2C3_SDA_PORT_ID != 0)
      #error "Only PC9 and PH8 can be configured as I2C3 SDA on selected device!"
    #endif
  #endif
  
  #if defined (STM32F411xE)
    /* SDA available on pins: PB4, PB8, PC9 */
    #if (RTE_I2C3_SDA_PORT_ID != 2 && RTE_I2C3_SDA_PORT_ID != 3 && RTE_I2C3_SDA_PORT_ID != 1)
      #error "Only PB4, PB8 and PC9 can be configured as I2C3 SDA on selected device!"
    #endif
  #endif

  #if defined (STM32F401xC) || defined (STM32F401xE) || defined (STM32F446xx)
    /* SDA available on pins: PB4, PC9 */
    #if (RTE_I2C3_SDA_PORT_ID != 2 && RTE_I2C3_SDA_PORT_ID != 1)
      #error "Only PB4 and PC9 can be configured as I2C3 SDA on selected device!"
    #endif
  #endif

  #if defined (STM32F401xC) || defined (STM32F401xE) || defined (STM32F411xE) || defined (STM32F446xx)
    /* SCL available on pin:  PA8 */
    #if (RTE_I2C3_SCL_PORT_ID != 1)
      #error "Only PA8 can be configured as I2C3 SCL on selected device!"
    #endif
  #endif

  #if defined (STM32F413xx) || defined (STM32F423xx)
    /* SDA available on pins: PB3, PB9, PB11, PF0 */
    #if (RTE_I2C3_SDA_PORT_ID == 0)
      #error "Only PB4, PB8, PC9 can be configured as I2C3 SDA on selected device!"
    #endif
    /* SCL available on pin:  PA8 */
    #if (RTE_I2C3_SCL_PORT_ID != 1)
      #error "Only PA8 can be configured as I2C3 SCL on selected device!"
    #endif
  #endif

  #define MX_I2C3

  #if (RTE_I2C3_RX_DMA == 1)
    #define MX_I2C3_RX_DMA_Instance DMAx_STREAMy(RTE_I2C3_RX_DMA_NUMBER, RTE_I2C3_RX_DMA_STREAM)
    #define MX_I2C3_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_I2C3_RX_DMA_NUMBER, RTE_I2C3_RX_DMA_STREAM)
    #define MX_I2C3_RX_DMA_Channel  DMA_CHANNEL_x(RTE_I2C3_RX_DMA_CHANNEL)
    #define MX_I2C3_RX_DMA_Priority DMA_PRIORITY(RTE_I2C3_RX_DMA_PRIORITY)

    #define I2C3_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_I2C3_RX_DMA_NUMBER, RTE_I2C3_RX_DMA_STREAM)
  #endif
  #if (RTE_I2C3_TX_DMA == 1)
    #define MX_I2C3_TX_DMA_Instance DMAx_STREAMy(RTE_I2C3_TX_DMA_NUMBER, RTE_I2C3_TX_DMA_STREAM)
    #define MX_I2C3_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_I2C3_TX_DMA_NUMBER, RTE_I2C3_TX_DMA_STREAM)
    #define MX_I2C3_TX_DMA_Channel  DMA_CHANNEL_x(RTE_I2C3_TX_DMA_CHANNEL)
    #define MX_I2C3_TX_DMA_Priority DMA_PRIORITY(RTE_I2C3_TX_DMA_PRIORITY)

    #define I2C3_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_I2C3_TX_DMA_NUMBER, RTE_I2C3_TX_DMA_STREAM)
  #endif

  #define MX_I2C3_SCL_GPIOx         RTE_I2C3_SCL_PORT
  #define MX_I2C3_SCL_GPIO_Pin      (1U << RTE_I2C3_SCL_BIT)
  #define MX_I2C3_SCL_GPIO_PuPdOD   GPIO_NOPULL
  #define MX_I2C3_SCL_GPIO_AF       GPIO_AF4_I2C3

  #define MX_I2C3_SDA_GPIOx         RTE_I2C3_SDA_PORT
  #define MX_I2C3_SDA_GPIO_Pin      (1U << RTE_I2C3_SDA_BIT)
  #define MX_I2C3_SDA_GPIO_PuPdOD   GPIO_NOPULL

  #if ((defined (STM32F413xx) || defined (STM32F423xx)) && \
       ((RTE_I2C3_SDA_PORT_ID == 2) || (RTE_I2C3_SDA_PORT_ID == 3)))
  #define MX_I2C3_SDA_GPIO_AF       GPIO_AF9_I2C3
  #else
  #define MX_I2C3_SDA_GPIO_AF       GPIO_AF4_I2C3
  #endif

#endif

#endif /* RTE_DEVICE_FRAMEWORK_CLASSIC */

#if defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
  #if ((defined(RTE_Drivers_I2C1) || \
        defined(RTE_Drivers_I2C2) || \
        defined(RTE_Drivers_I2C3))   \
        && (!defined (MX_I2C1))      \
        && (!defined (MX_I2C2))      \
        && (!defined (MX_I2C3)))
    #error "I2C not configured in STM32CubeMX!"
  #endif

  /* I2C1 configuration definitions */
  #if defined (MX_I2C1)
    #if ((defined (MX_I2C1_TX_DMA_Instance) && (!defined (MX_I2C1_RX_DMA_Instance))) || \
         (defined (MX_I2C1_RX_DMA_Instance) && (!defined (MX_I2C1_TX_DMA_Instance))))
      #error "I2C1 driver in DMA mode requires Rx and Tx DMA channel configured in STM32CubeMX!"
    #endif
  #endif

  /* I2C2 configuration definitions */
  #if defined (MX_I2C2)
    #if ((defined (MX_I2C2_TX_DMA_Instance) && (!defined (MX_I2C2_RX_DMA_Instance))) || \
         (defined (MX_I2C2_RX_DMA_Instance) && (!defined (MX_I2C2_TX_DMA_Instance))))
      #error "I2C2 driver in DMA mode requires Rx and Tx DMA channel configured in STM32CubeMX!"
    #endif
  #endif

  /* I2C3 configuration definitions */
  #if defined (MX_I2C3)
    #if ((defined (MX_I2C3_TX_DMA_Instance) && (!defined (MX_I2C3_RX_DMA_Instance))) || \
         (defined (MX_I2C3_RX_DMA_Instance) && (!defined (MX_I2C3_TX_DMA_Instance))))
      #error "I2C3 driver in DMA mode requires Rx and Tx DMA channel configured in STM32CubeMX!"
    #endif
  #endif

#endif /* RTE_DEVICE_FRAMEWORK_CUBE_MX */

/* Bus Clear clock period definition */
#define I2C_BUS_CLEAR_CLOCK_PERIOD   2        // I2C bus clock period in ms

/* Current driver status flag definition */
#define I2C_INIT            ((uint8_t)0x01)   // I2C initialized
#define I2C_POWER           ((uint8_t)0x02)   // I2C powered on
#define I2C_SETUP           ((uint8_t)0x04)   // I2C configured
#define I2C_DMA_XFER        ((uint8_t)0x08)   // DMA transfer in use

#define I2C_MODE_SLAVE      ((uint8_t)0x00)   // Mode: slave
#define I2C_MODE_MASTER     ((uint8_t)0x01)   // Mode: master

#define I2C_DIR_TX          ((uint8_t)0x00)   // Direction: transmitter
#define I2C_DIR_RX          ((uint8_t)0x01)   // Direction: receiver

/* Transfer state definitions (slave) */
#define XFER_STATE_IDLE      (0U)
#define XFER_STATE_ADDR      (1U)
#define XFER_STATE_SLAVE_RX  (2U)
#define XFER_STATE_SLAVE_TX  (3U)

/* Transfer status flags definitions */
#define XFER_CTRL_XSET      (0x0001UL)        // Transfer is set
#define XFER_CTRL_XPENDING  (0x0002UL)        // Transfer pending
#define XFER_CTRL_XDONE     (0x0008UL)        // Transfer done (all data transferred)
#define XFER_CTRL_RSTART    (0x0010UL)        // Generate repeated start and readdress
#define XFER_CTRL_RDIR      (0x0020UL)        // Re-send direction bit (10-bit addressing)
#define XFER_CTRL_ADDR_DONE (0x0040UL)        // Addressing done
#define XFER_CTRL_DMA_DONE  (0x0080UL)        // DMA transfer done
#define XFER_CTRL_WAIT_BTF  (0x0100UL)        // Wait for byte transfer finished


/* DMA Event definitions */
#define DMA_COMPLETED             0U
#define DMA_ERROR                 1U

/* DMA Callback functions */
typedef void (*DMA_Callback_t) (DMA_HandleTypeDef *hdma);

/* DMA Information definitions */
typedef struct _I2C_DMA {
  DMA_HandleTypeDef    *h;
  DMA_Callback_t        cb_complete;
  DMA_Callback_t        cb_error;
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
  DMA_Stream_TypeDef   *stream;               // Stream register interface
  IRQn_Type             irq_num;              // Stream IRQ number
  uint8_t               reserved[3];          // Reserved
  uint32_t              channel;              // Channel number
  uint32_t              priority;             // Stream priority
#endif
} const I2C_DMA;


/* I2C Input/Output Configuration */
typedef const struct _I2C_IO {
  GPIO_TypeDef         *scl_port;             // SCL IO Port
  GPIO_TypeDef         *sda_port;             // SDA IO Port
  uint16_t              scl_pin;              // SCL IO Pin
  uint16_t              sda_pin;              // SDA IO Pin
  uint32_t              scl_pull;             // SCL IO Pin Pull Definition
  uint32_t              sda_pull;             // SDA IO Pin Pull Definition
  uint32_t              scl_af;
  uint32_t              sda_af;
} I2C_IO;


/* I2C Transfer Information (Run-Time) */
typedef struct _I2C_TRANSFER_INFO {
  uint8_t              *data;                 // Data pointer
  uint32_t              num;                  // Number of data to transfer
  uint32_t              cnt;                  // Data transfer counter
} I2C_TRANSFER_INFO;


/* I2C Information (Run-Time) */
typedef struct _I2C_INFO {
  ARM_I2C_SignalEvent_t cb_event;             // Event Callback
  ARM_I2C_STATUS        status;               // Status flags
  I2C_TRANSFER_INFO     rx;                   // Rx transfer information
  I2C_TRANSFER_INFO     tx;                   // Tx transfer information
  uint16_t              addr;                 // Transfer address
  uint8_t               flags;                // Current I2C state flags
  uint8_t               state;                // Slave state (IRQ)
  uint32_t              ctrl;                 // Transfer control (current)
} I2C_INFO;


/* I2C Resources definition */
typedef struct {
#if defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
  I2C_HandleTypeDef    *h;                    // I2C handle
#endif
  I2C_TypeDef          *reg;                  // I2C peripheral register interface
  I2C_DMA              *dma_rx;               // I2C DMA Configuration
  I2C_DMA              *dma_tx;               // I2C DMA Configuration
  I2C_IO                io;                   // I2C Input/Output pins
  IRQn_Type             ev_irq_num;           // I2C Event IRQ Number
  IRQn_Type             er_irq_num;           // I2C Error IRQ Number
  uint16_t              reserved;             // Reserved
  I2C_INFO             *info;                 // Run-Time information
} const I2C_RESOURCES;

// Global functions and variables exported by driver .c module
#ifdef MX_I2C1
extern void I2C1_EV_IRQHandler  (void);
extern void I2C1_ER_IRQHandler  (void);
extern void I2C1_RX_DMA_Handler (void);
extern void I2C1_TX_DMA_Handler (void);

extern ARM_DRIVER_I2C Driver_I2C1;
#endif

#ifdef MX_I2C2
extern void I2C2_EV_IRQHandler  (void);
extern void I2C2_ER_IRQHandler  (void);
extern void I2C2_RX_DMA_Handler (void);
extern void I2C2_TX_DMA_Handler (void);

extern ARM_DRIVER_I2C Driver_I2C2;
#endif

#ifdef MX_I2C3
extern void I2C3_EV_IRQHandler  (void);
extern void I2C3_ER_IRQHandler  (void);
extern void I2C3_RX_DMA_Handler (void);
extern void I2C3_TX_DMA_Handler (void);

extern ARM_DRIVER_I2C Driver_I2C3;
#endif

#endif /* __I2C_STM32F7XX_H */
