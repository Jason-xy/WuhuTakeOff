/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2020 Arm Limited (or its affiliates). All 
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
 * $Date:        7. April 2020
 * $Revision:    V2.13
 *
 * Project:      SPI Driver definitions for ST STM32F4xx
 * -------------------------------------------------------------------------- */

#ifndef __SPI_STM32F4XX_H
#define __SPI_STM32F4XX_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "Driver_SPI.h"
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
  #if ((defined(RTE_Drivers_SPI1) || \
        defined(RTE_Drivers_SPI2) || \
        defined(RTE_Drivers_SPI3) || \
        defined(RTE_Drivers_SPI4) || \
        defined(RTE_Drivers_SPI5) || \
        defined(RTE_Drivers_SPI6))   \
       && (RTE_SPI1 == 0)            \
       && (RTE_SPI2 == 0)            \
       && (RTE_SPI3 == 0)            \
       && (RTE_SPI4 == 0)            \
       && (RTE_SPI5 == 0)            \
       && (RTE_SPI6 == 0))
    #error "SPI not configured in RTE_Device.h!"
  #endif

// RTE macros
#define _DMA_CHANNEL_x(x)               DMA_CHANNEL_##x
#define  DMA_CHANNEL_x(x)              _DMA_CHANNEL_x(x)

#define  DMA_PRIORITY(x)              ((x == 0) ? DMA_PRIORITY_LOW    : \
                                       (x == 1) ? DMA_PRIORITY_MEDIUM : \
                                       (x == 2) ? DMA_PRIORITY_HIGH   : \
                                                  DMA_PRIORITY_VERY_HIGH)

#define _DMAx_STREAMy(x, y)             DMA##x##_Stream##y
#define  DMAx_STREAMy(x, y)            _DMAx_STREAMy(x, y)

#define _DMAx_STREAMy_IRQ(x, y)         DMA##x##_Stream##y##_IRQHandler
#define  DMAx_STREAMy_IRQ(x, y)        _DMAx_STREAMy_IRQ(x, y)

#define _DMAx_STREAMy_IRQn(x, y)        DMA##x##_Stream##y##_IRQn
#define  DMAx_STREAMy_IRQn(x, y)       _DMAx_STREAMy_IRQn(x, y)

// SPI1 configuration definitions
#if (RTE_SPI1 == 1)
  #define MX_SPI1

  #if (RTE_SPI1_RX_DMA == 1)
    #define MX_SPI1_RX_DMA_Instance DMAx_STREAMy(RTE_SPI1_RX_DMA_NUMBER, RTE_SPI1_RX_DMA_STREAM)
    #define MX_SPI1_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_SPI1_RX_DMA_NUMBER, RTE_SPI1_RX_DMA_STREAM)
    #define MX_SPI1_RX_DMA_Channel  DMA_CHANNEL_x(RTE_SPI1_RX_DMA_CHANNEL)
    #define MX_SPI1_RX_DMA_Priority DMA_PRIORITY(RTE_SPI1_RX_DMA_PRIORITY)

    #define SPI1_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_SPI1_RX_DMA_NUMBER, RTE_SPI1_RX_DMA_STREAM)
  #endif
  #if (RTE_SPI1_TX_DMA == 1)
    #define MX_SPI1_TX_DMA_Instance DMAx_STREAMy(RTE_SPI1_TX_DMA_NUMBER, RTE_SPI1_TX_DMA_STREAM)
    #define MX_SPI1_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_SPI1_TX_DMA_NUMBER, RTE_SPI1_TX_DMA_STREAM)
    #define MX_SPI1_TX_DMA_Channel  DMA_CHANNEL_x(RTE_SPI1_TX_DMA_CHANNEL)
    #define MX_SPI1_TX_DMA_Priority DMA_PRIORITY(RTE_SPI1_TX_DMA_PRIORITY)

    #define SPI1_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_SPI1_TX_DMA_NUMBER, RTE_SPI1_TX_DMA_STREAM)
  #endif

  #ifndef SPI1_GPIO_SPEED_FREQ
    #define SPI1_GPIO_SPEED_FREQ  GPIO_SPEED_FREQ_HIGH
  #endif

  #if (RTE_SPI1_MISO == 1)
    #if defined (STM32F410Tx)
      // SPI1 MISO available on pin: PB4
      #if (RTE_SPI1_MISO_PORT_ID != 2)
        #error "Only PB4 can be configured as SPI1 MISO on selected device!"
      #endif
    #endif

    #define MX_SPI1_MISO_Pin        1U
    #define MX_SPI1_MISO_GPIOx      RTE_SPI1_MISO_PORT
    #define MX_SPI1_MISO_GPIO_Pin   (1U << RTE_SPI1_MISO_BIT)
    #define MX_SPI1_MISO_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI1_MISO_GPIO_Speed SPI1_GPIO_SPEED_FREQ
    #define MX_SPI1_MISO_GPIO_AF    GPIO_AF5_SPI1
  #endif

  #if (RTE_SPI1_MOSI == 1)
    #if defined (STM32F410Tx)
      // SPI1 MOSI available on pin: PB5
      #if (RTE_SPI1_MOSI_PORT_ID != 2)
        #error "Only PB5 can be configured as SPI1 MOSI on selected device!"
      #endif
    #endif

    #define MX_SPI1_MOSI_Pin        1U
    #define MX_SPI1_MOSI_GPIOx      RTE_SPI1_MOSI_PORT
    #define MX_SPI1_MOSI_GPIO_Pin  (1U << RTE_SPI1_MOSI_BIT)
    #define MX_SPI1_MOSI_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI1_MOSI_GPIO_Speed SPI1_GPIO_SPEED_FREQ
    #define MX_SPI1_MOSI_GPIO_AF    GPIO_AF5_SPI1
  #endif

  #define MX_SPI1_SCK_Pin         1U
  #define MX_SPI1_SCK_GPIOx       RTE_SPI1_SCL_PORT
  #define MX_SPI1_SCK_GPIO_Pin    (1U << RTE_SPI1_SCL_BIT)
  #define MX_SPI1_SCK_GPIO_PuPd   GPIO_NOPULL
  #define MX_SPI1_SCK_GPIO_Speed  SPI1_GPIO_SPEED_FREQ
  #define MX_SPI1_SCK_GPIO_AF     GPIO_AF5_SPI1

  #if (RTE_SPI1_NSS_PIN == 1)
    #if defined (STM32F410Tx)
      // SPI1 NSS available on pin: PA15
      #if ((RTE_SPI1_NSS_PORT_ID != 0) && (RTE_SPI1_NSS_PORT_ID != 2)
        #error "Only PA15 can be configured as SPI1 NSS on selected device!"
      #endif
    #endif

    #define MX_SPI1_NSS_Pin        1U
    #define MX_SPI1_NSS_GPIOx      RTE_SPI1_NSS_PORT
    #define MX_SPI1_NSS_GPIO_Pin  (1U << RTE_SPI1_NSS_BIT)
    #define MX_SPI1_NSS_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI1_NSS_GPIO_Speed SPI1_GPIO_SPEED_FREQ
    #define MX_SPI1_NSS_GPIO_AF    GPIO_AF5_SPI1
  #endif
#endif

// SPI2 configuration definitions
#if (RTE_SPI2 == 1)

  #ifndef SPI2
    #error "SPI2 not available for selected device!"
  #endif

  #define MX_SPI2

  #if (RTE_SPI2_RX_DMA == 1)
    #define MX_SPI2_RX_DMA_Instance DMAx_STREAMy(RTE_SPI2_RX_DMA_NUMBER, RTE_SPI2_RX_DMA_STREAM)
    #define MX_SPI2_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_SPI2_RX_DMA_NUMBER, RTE_SPI2_RX_DMA_STREAM)
    #define MX_SPI2_RX_DMA_Channel  DMA_CHANNEL_x(RTE_SPI2_RX_DMA_CHANNEL)
    #define MX_SPI2_RX_DMA_Priority DMA_PRIORITY(RTE_SPI2_RX_DMA_PRIORITY)

    #define SPI2_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_SPI2_RX_DMA_NUMBER, RTE_SPI2_RX_DMA_STREAM)
  #endif
  #if (RTE_SPI2_TX_DMA == 1)
    #define MX_SPI2_TX_DMA_Instance DMAx_STREAMy(RTE_SPI2_TX_DMA_NUMBER, RTE_SPI2_TX_DMA_STREAM)
    #define MX_SPI2_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_SPI2_TX_DMA_NUMBER, RTE_SPI2_TX_DMA_STREAM)
    #define MX_SPI2_TX_DMA_Channel  DMA_CHANNEL_x(RTE_SPI2_TX_DMA_CHANNEL)
    #define MX_SPI2_TX_DMA_Priority DMA_PRIORITY(RTE_SPI2_TX_DMA_PRIORITY)

    #define SPI2_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_SPI2_TX_DMA_NUMBER, RTE_SPI2_TX_DMA_STREAM)
  #endif

  #ifndef SPI2_GPIO_SPEED_FREQ
    #define SPI2_GPIO_SPEED_FREQ  GPIO_SPEED_FREQ_HIGH
  #endif

  #if (RTE_SPI2_MISO == 1)
    #if defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F411xE) || defined(STM32F446xx)
      // PI2 as SPI2 MISO not available on STM32F401xx, STM32F411xx and STM32F446xx
      #if (RTE_SPI2_MISO_PORT_ID == 3)
        #error "PI2 can not be configured as SPI2 MISO on selected device!"
      #endif
    #elif defined (STM32F410Cx)
      // SPI2 MISO available on pin: PB14
      #if (RTE_SPI2_MISO_PORT_ID != 1)
        #error "Only PB14 can be configured as SPI2 MISO on selected device!"
      #endif
    #elif defined (STM32F410Rx)
      // SPI2 MISO available on pins: PB14, PC2
      #if ((RTE_SPI2_MISO_PORT_ID != 1) && (RTE_SPI2_MISO_PORT_ID != 2))
        #error "Only PB14 and PC2 can be configured as SPI2 MISO on selected device!"
      #endif
    #elif defined (STM32F413xx) || defined (STM32F423xx)
      // SPI2 MISO available on pins: PB14, PC2, PA12
      #if ((RTE_SPI2_MISO_PORT_ID != 1) && (RTE_SPI2_MISO_PORT_ID != 2) && (RTE_SPI2_MISO_PORT_ID != 4))
        #error "Only PB14, PC2, PA12 can be configured as SPI2 MISO on selected device!"
      #endif
    #endif

    #define MX_SPI2_MISO_Pin        1U
    #define MX_SPI2_MISO_GPIOx      RTE_SPI2_MISO_PORT
    #define MX_SPI2_MISO_GPIO_Pin   (1U << RTE_SPI2_MISO_BIT)
    #define MX_SPI2_MISO_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI2_MISO_GPIO_Speed SPI2_GPIO_SPEED_FREQ
    #define MX_SPI2_MISO_GPIO_AF    GPIO_AF5_SPI2
  #endif

  #if (RTE_SPI2_MOSI == 1)
    #if defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F411xE) || defined(STM32F446xx)
      // PI3 as SPI2 MOSI not available on STM32F401xx, STM32F411xx and STM32F446xx
      #if (RTE_SPI2_MOSI_PORT_ID == 3)
        #error "PI3 can not be configured as SPI2 MOSI on selected device!"
      #endif
    #elif defined (STM32F410Cx)
      // SPI2 MOSI available on pin: PB15
      #if (RTE_SPI2_MOSI_PORT_ID != 1)
        #error "Only PB15 can be configured as SPI2 MOSI on selected device!"
      #endif
    #elif defined (STM32F410Rx)
      // SPI2 MOSI available on pins: PB15, PC3
      #if ((RTE_SPI2_MOSI_PORT_ID != 1) && (RTE_SPI2_MOSI_PORT_ID != 2))
        #error "Only PB15 and PC3 can be configured as SPI2 MOSI on selected device!"
      #endif
    #elif defined (STM32F413xx) || defined (STM32F423xx)
      // SPI2 MOSI available on pins: PB15, PC3, PA10
      #if ((RTE_SPI2_MOSI_PORT_ID != 1) && (RTE_SPI2_MOSI_PORT_ID != 2) && (RTE_SPI2_MOSI_PORT_ID != 4))
        #error "Only PB15, PC3, PA10 can be configured as SPI2 MOSI on selected device!"
      #endif
    #endif

    #define MX_SPI2_MOSI_Pin        1U
    #define MX_SPI2_MOSI_GPIOx      RTE_SPI2_MOSI_PORT
    #define MX_SPI2_MOSI_GPIO_Pin   (1U << RTE_SPI2_MOSI_BIT)
    #define MX_SPI2_MOSI_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI2_MOSI_GPIO_Speed SPI2_GPIO_SPEED_FREQ
    #define MX_SPI2_MOSI_GPIO_AF    GPIO_AF5_SPI2
  #endif

  #if defined (STM32F410Cx)
    // SPI2 SCK available on pins: PB10, PB13
    #if (RTE_SPI2_SCL_PORT_ID > 1)
      #error "Only PB10 and PB13 can be configured as SPI2 SCK on selected device!"
    #endif
  #elif defined (STM32F410Rx)
    // SPI2 SCK available on pins: PB10, PB13, PC7
    #if (RTE_SPI2_SCL_PORT_ID > 2)
      #error "Only PB10, PB13 and PC7 can be configured as SPI2 SCK on selected device!"
    #endif
  #elif !defined(STM32F411xE) && !defined(STM32F446xx)
    // PC7 as SPI2 SCK only available on STM32F411xx and STM32F446xx
    #if (RTE_SPI2_SCL_PORT_ID == 2)
      #error "PC7 can not be configured as SPI2 SCK on selected device!"
    #endif
  #elif defined(STM32F405xx) || defined(STM32F407xx) || defined(STM32F415xx) || defined(STM32F417xx)
    // PD3 as SPI2 SCK not available on STM32F405xx, STM32F407xx, STM32F415xx and STM32F417xx
    #if (RTE_SPI2_SCL_PORT_ID == 3)
      #error "PD3 can not be configured as SPI2 SCK on selected device!"
    #endif
  #elif defined(STM32F446xx)
    // PI1 as SPI2 SCK not available on STM32F446xx
    #if (RTE_SPI2_SCL_PORT_ID == 1)
      #error "PI1 can not be configured as SPI2 SCK on selected device!"
    #endif
  #elif defined (STM32F413xx) || defined (STM32F423xx)
    // PI1 as SPI2 SCK not available on STM32F413xx, STM32F423xx
    #if (RTE_SPI2_SCL_PORT_ID == 4)
      #error "PI1 can not be configured as SPI2 SCK on selected device!"
    #endif
  #endif

  #define MX_SPI2_SCK_Pin        1U
  #define MX_SPI2_SCK_GPIOx      RTE_SPI2_SCL_PORT
  #define MX_SPI2_SCK_GPIO_Pin   (1U << RTE_SPI2_SCL_BIT)
  #define MX_SPI2_SCK_GPIO_PuPd  GPIO_NOPULL
  #define MX_SPI2_SCK_GPIO_Speed SPI2_GPIO_SPEED_FREQ
  #define MX_SPI2_SCK_GPIO_AF    GPIO_AF5_SPI2

  #if (RTE_SPI2_NSS_PIN == 1)
    #if defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F411xE) || defined(STM32F446xx)
      // PI0 as SPI2 NSS not available on STM32F401xx, STM32F411xx and STM32F446xx
      #if (RTE_SPI2_NSS_PORT_ID == 3)
        #error "PI0 can not be configured as SPI2 NSS on selected device!"
      #endif
    #elif defined (STM32F410Cx) || defined (STM32F410Rx)
      // SPI2 NSS available on pins: PB9, PB12
      #if (RTE_SPI2_NSS_PORT_ID > 2)
        #error "Only PB9 and PB12 can be configured as SPI2 NSS on selected device!"
      #endif
    #elif defined (STM32F413xx) || defined (STM32F423xx)
      // PI0 as SPI2 NSS not available on STM32F413xx, STM32F423xx
      #if (RTE_SPI2_NSS_PORT_ID == 3)
        #error "PI0 can not be configured as SPI2 NSS on selected device!"
      #endif
    #endif

    #define MX_SPI2_NSS_Pin        1U
    #define MX_SPI2_NSS_GPIOx      RTE_SPI2_NSS_PORT
    #define MX_SPI2_NSS_GPIO_Pin  (1U << RTE_SPI2_NSS_BIT)
    #define MX_SPI2_NSS_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI2_NSS_GPIO_Speed SPI2_GPIO_SPEED_FREQ
    #define MX_SPI2_NSS_GPIO_AF    GPIO_AF5_SPI2
  #endif
#endif

// SPI3 configuration definitions
#if (RTE_SPI3 == 1)

  #ifndef SPI3
    #error "SPI3 not available for selected device!"
  #endif

  #define MX_SPI3

  #if (RTE_SPI3_RX_DMA == 1)
    #define MX_SPI3_RX_DMA_Instance DMAx_STREAMy(RTE_SPI3_RX_DMA_NUMBER, RTE_SPI3_RX_DMA_STREAM)
    #define MX_SPI3_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_SPI3_RX_DMA_NUMBER, RTE_SPI3_RX_DMA_STREAM)
    #define MX_SPI3_RX_DMA_Channel  DMA_CHANNEL_x(RTE_SPI3_RX_DMA_CHANNEL)
    #define MX_SPI3_RX_DMA_Priority DMA_PRIORITY(RTE_SPI3_RX_DMA_PRIORITY)

    #define SPI3_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_SPI3_RX_DMA_NUMBER, RTE_SPI3_RX_DMA_STREAM)
  #endif
  #if (RTE_SPI3_TX_DMA == 1)
    #define MX_SPI3_TX_DMA_Instance DMAx_STREAMy(RTE_SPI3_TX_DMA_NUMBER, RTE_SPI3_TX_DMA_STREAM)
    #define MX_SPI3_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_SPI3_TX_DMA_NUMBER, RTE_SPI3_TX_DMA_STREAM)
    #define MX_SPI3_TX_DMA_Channel  DMA_CHANNEL_x(RTE_SPI3_TX_DMA_CHANNEL)
    #define MX_SPI3_TX_DMA_Priority DMA_PRIORITY(RTE_SPI3_TX_DMA_PRIORITY)

    #define SPI3_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_SPI3_TX_DMA_NUMBER, RTE_SPI3_TX_DMA_STREAM)
  #endif

  #ifndef SPI3_GPIO_SPEED_FREQ
    #define SPI3_GPIO_SPEED_FREQ  GPIO_SPEED_FREQ_HIGH
  #endif

  #if (RTE_SPI3_MISO == 1)
    #define MX_SPI3_MISO_Pin        1U
    #define MX_SPI3_MISO_GPIOx      RTE_SPI3_MISO_PORT
    #define MX_SPI3_MISO_GPIO_Pin   (1U << RTE_SPI3_MISO_BIT)
    #define MX_SPI3_MISO_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI3_MISO_GPIO_Speed SPI3_GPIO_SPEED_FREQ
    #define MX_SPI3_MISO_GPIO_AF    GPIO_AF6_SPI3
  #endif

  #if (RTE_SPI3_MOSI == 1)
    #if defined(STM32F405xx) || defined(STM32F407xx) || \
        defined(STM32F415xx) || defined(STM32F417xx)
      // PD6 as SPI3 MOSI not available on STM32F405xx, STM32F407xx, STM32F415xx and STM32F417xx
      #if (RTE_SPI3_MOSI_PORT_ID == 3)
        #error "PD6 can not be configured as SPI3 MOSI on selected device!"
      #endif
    #endif
    #define MX_SPI3_MOSI_Pin        1U
    #define MX_SPI3_MOSI_GPIOx      RTE_SPI3_MOSI_PORT
    #define MX_SPI3_MOSI_GPIO_Pin   (1U << RTE_SPI3_MOSI_BIT)
    #define MX_SPI3_MOSI_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI3_MOSI_GPIO_Speed SPI3_GPIO_SPEED_FREQ

    #if (defined (STM32F413xx) || defined (STM32F423xx)) && (RTE_SPI3_MOSI_PORT_ID == 3)
    #define MX_SPI3_MOSI_GPIO_AF    GPIO_AF5_SPI3
    #else
    #define MX_SPI3_MOSI_GPIO_AF    GPIO_AF6_SPI3
    #endif
  #endif

  #if !defined(STM32F411xE) && !defined(STM32F412Cx) && !defined(STM32F412Rx) && !defined(STM32F412Vx) && !defined(STM32F412Zx) && !defined(STM32F413xx) && !defined(STM32F423xx)
    // PB12 as SPI3 SCK not available on STM32F411xx, STM32F412xx, STM32F413xx and STM32F423xx
    #if (RTE_SPI3_SCL_PORT_ID == 1)
      #error "PB12 can not be configured as SPI3 SCK on selected device!"
    #endif
  #endif
  #define MX_SPI3_SCK_Pin        1U
  #define MX_SPI3_SCK_GPIOx      RTE_SPI3_SCL_PORT
  #define MX_SPI3_SCK_GPIO_Pin  (1U << RTE_SPI3_SCL_BIT)
  #define MX_SPI3_SCK_GPIO_PuPd  GPIO_NOPULL
  #define MX_SPI3_SCK_GPIO_Speed SPI3_GPIO_SPEED_FREQ

  #if (defined (STM32F413xx) || defined (STM32F423xx)) && (RTE_SPI3_SCL_PORT_ID == 1)
  #define MX_SPI3_SCK_GPIO_AF    GPIO_AF7_SPI3
  #else
  #define MX_SPI3_SCK_GPIO_AF    GPIO_AF6_SPI3
  #endif

  #if (RTE_SPI3_NSS_PIN == 1)
    #define MX_SPI3_NSS_Pin         1U
    #define MX_SPI3_NSS_GPIOx       RTE_SPI3_NSS_PORT
    #define MX_SPI3_NSS_GPIO_Pin   (1U << RTE_SPI3_NSS_BIT)
    #define MX_SPI3_NSS_GPIO_PuPd   GPIO_NOPULL
    #define MX_SPI3_NSS_GPIO_Speed  SPI3_GPIO_SPEED_FREQ
    #define MX_SPI3_NSS_GPIO_AF     GPIO_AF6_SPI3
  #endif
#endif

// SPI4 configuration definitions
#if (RTE_SPI4 == 1)

  #ifndef SPI4
    #error "SPI4 not available for selected device!"
  #endif
  
  #define MX_SPI4

  #if (RTE_SPI4_RX_DMA == 1)
    #define MX_SPI4_RX_DMA_Instance DMAx_STREAMy(RTE_SPI4_RX_DMA_NUMBER, RTE_SPI4_RX_DMA_STREAM)
    #define MX_SPI4_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_SPI4_RX_DMA_NUMBER, RTE_SPI4_RX_DMA_STREAM)
    #define MX_SPI4_RX_DMA_Channel  DMA_CHANNEL_x(RTE_SPI4_RX_DMA_CHANNEL)
    #define MX_SPI4_RX_DMA_Priority DMA_PRIORITY(RTE_SPI4_RX_DMA_PRIORITY)

    #define SPI4_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_SPI4_RX_DMA_NUMBER, RTE_SPI4_RX_DMA_STREAM)
  #endif
  #if (RTE_SPI4_TX_DMA == 1)
    #define MX_SPI4_TX_DMA_Instance DMAx_STREAMy(RTE_SPI4_TX_DMA_NUMBER, RTE_SPI4_TX_DMA_STREAM)
    #define MX_SPI4_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_SPI4_TX_DMA_NUMBER, RTE_SPI4_TX_DMA_STREAM)
    #define MX_SPI4_TX_DMA_Channel  DMA_CHANNEL_x(RTE_SPI4_TX_DMA_CHANNEL)
    #define MX_SPI4_TX_DMA_Priority DMA_PRIORITY(RTE_SPI4_TX_DMA_PRIORITY)

    #define SPI4_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_SPI4_TX_DMA_NUMBER, RTE_SPI4_TX_DMA_STREAM)
  #endif

  #ifndef SPI4_GPIO_SPEED_FREQ
    #define SPI4_GPIO_SPEED_FREQ  GPIO_SPEED_FREQ_HIGH
  #endif

  #if (RTE_SPI4_MISO == 1)
    #ifndef STM32F411xE
      // PA11 as SPI4 MISO only available on STM32F411xx
      #if (RTE_SPI4_MISO_PORT_ID == 1)
        #error "PA11 can not be configured as SPI4 MISO on selected device!"
      #endif
    #endif
    #define MX_SPI4_MISO_Pin        1U
    #define MX_SPI4_MISO_GPIOx      RTE_SPI4_MISO_PORT
    #define MX_SPI4_MISO_GPIO_Pin   (1U << RTE_SPI4_MISO_BIT)
    #define MX_SPI4_MISO_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI4_MISO_GPIO_Speed SPI4_GPIO_SPEED_FREQ

    #if (defined (STM32F413xx) || defined (STM32F423xx)) && (RTE_SPI4_MISO_PORT_ID == 1)
    #define MX_SPI4_MISO_GPIO_AF   GPIO_AF6_SPI4
    #else
    #define MX_SPI4_MISO_GPIO_AF   GPIO_AF5_SPI4
    #endif
  #endif

  #if (RTE_SPI4_MOSI == 1)
    #ifndef STM32F411xE
      // PA1 as SPI4 MOSI only available on STM32F411xx
      #if (RTE_SPI4_MOSI_PORT_ID == 1)
        #error "PA1 can not be configured as SPI4 MOSI on selected device!"
      #endif
    #endif
    #define MX_SPI4_MOSI_Pin        1U
    #define MX_SPI4_MOSI_GPIOx      RTE_SPI4_MOSI_PORT
    #define MX_SPI4_MOSI_GPIO_Pin   (1U << RTE_SPI4_MOSI_BIT)
    #define MX_SPI4_MOSI_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI4_MOSI_GPIO_Speed SPI4_GPIO_SPEED_FREQ
    #define MX_SPI4_MOSI_GPIO_AF    GPIO_AF5_SPI4
  #endif

  #ifndef STM32F411xE
    // PB13 as SPI4 SCK only available on STM32F411xx
    #if (RTE_SPI4_SCL_PORT_ID == 0)
      #error "PB13 can not be configured as SPI4 SCK on selected device!"
    #endif
  #endif
  #define MX_SPI4_SCK_Pin        1U
  #define MX_SPI4_SCK_GPIOx      RTE_SPI4_SCL_PORT
  #define MX_SPI4_SCK_GPIO_Pin   (1U << RTE_SPI4_SCL_BIT)
  #define MX_SPI4_SCK_GPIO_PuPd  GPIO_NOPULL
  #define MX_SPI4_SCK_GPIO_Speed SPI4_GPIO_SPEED_FREQ

  #if (defined (STM32F413xx) || defined (STM32F423xx)) && (RTE_SPI4_SCL_PORT_ID == 0)
  #define MX_SPI4_SCK_GPIO_AF   GPIO_AF6_SPI4
  #else
  #define MX_SPI4_SCK_GPIO_AF   GPIO_AF5_SPI4
  #endif

  #if (RTE_SPI4_NSS_PIN == 1)
    #ifndef STM32F411xE
      // PB12 as SPI4 NSS only available on STM32F411xx
      #if (RTE_SPI4_NSS_PORT_ID == 1)
        #error "PB12 can not be configured as SPI4 NSS on selected device!"
      #endif
    #endif
    #define MX_SPI4_NSS_Pin        1U
    #define MX_SPI4_NSS_GPIOx      RTE_SPI4_NSS_PORT
    #define MX_SPI4_NSS_GPIO_Pin  (1U << RTE_SPI4_NSS_BIT)
    #define MX_SPI4_NSS_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI4_NSS_GPIO_Speed SPI4_GPIO_SPEED_FREQ

    #if (defined (STM32F413xx) || defined (STM32F423xx)) && (RTE_SPI4_NSS_PORT_ID == 1)
    #define MX_SPI4_NSS_GPIO_AF    GPIO_AF6_SPI4
    #else
    #define MX_SPI4_NSS_GPIO_AF    GPIO_AF5_SPI4
    #endif
  #endif
#endif

// SPI5 configuration definitions
#if (RTE_SPI5 == 1)

  #ifndef SPI5
    #error "SPI5 not available for selected device!"
  #endif

  #define MX_SPI5

  #if (RTE_SPI5_RX_DMA == 1)
    #define MX_SPI5_RX_DMA_Instance DMAx_STREAMy(RTE_SPI5_RX_DMA_NUMBER, RTE_SPI5_RX_DMA_STREAM)
    #define MX_SPI5_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_SPI5_RX_DMA_NUMBER, RTE_SPI5_RX_DMA_STREAM)
    #define MX_SPI5_RX_DMA_Channel  DMA_CHANNEL_x(RTE_SPI5_RX_DMA_CHANNEL)
    #define MX_SPI5_RX_DMA_Priority DMA_PRIORITY(RTE_SPI5_RX_DMA_PRIORITY)

    #define SPI5_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_SPI5_RX_DMA_NUMBER, RTE_SPI5_RX_DMA_STREAM)
  #endif
  #if (RTE_SPI5_TX_DMA == 1)
    #define MX_SPI5_TX_DMA_Instance DMAx_STREAMy(RTE_SPI5_TX_DMA_NUMBER, RTE_SPI5_TX_DMA_STREAM)
    #define MX_SPI5_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_SPI5_TX_DMA_NUMBER, RTE_SPI5_TX_DMA_STREAM)
    #define MX_SPI5_TX_DMA_Channel  DMA_CHANNEL_x(RTE_SPI5_TX_DMA_CHANNEL)
    #define MX_SPI5_TX_DMA_Priority DMA_PRIORITY(RTE_SPI5_TX_DMA_PRIORITY)

    #define SPI5_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_SPI5_TX_DMA_NUMBER, RTE_SPI5_TX_DMA_STREAM)
  #endif

  #ifndef SPI5_GPIO_SPEED_FREQ
    #define SPI5_GPIO_SPEED_FREQ  GPIO_SPEED_FREQ_HIGH
  #endif

  #if (RTE_SPI5_MISO == 1)
    #if defined (STM32F410Cx) || defined (STM32F410Rx)
      // SPI5 MISO available on pin: PA12
      #if (RTE_SPI5_MISO_PORT_ID != 1)
        #error "Only PA12 can be configured as SPI5 MISO on selected device!"
      #endif
    #elif !defined (STM32F411xE)
      // PA12 as SPI5 MISO only available on STM32F411xx
      #if (RTE_SPI5_MISO_PORT_ID == 1)
        #error "PA12 can not be configured as SPI5 MISO on selected device!"
      #endif
 
      // PE5 as SPI5 MISO only available on STM32F411xx
      #if (RTE_SPI5_MISO_PORT_ID == 2)
        #error "PE5 can not be configured as SPI5 MISO on selected device!"
      #endif

      // PE13 as SPI5 MISO only available on STM32F411xx
      #if (RTE_SPI5_MISO_PORT_ID == 3)
        #error "PE13 can not be configured as SPI5 MISO on selected device!"
      #endif
    #else
      // PF8 as SPI5 MISO only available on STM32F427xx, STM32F429xx, STM32F437xx and STM32F439xx
      #if (RTE_SPI5_MISO_PORT_ID == 4)
        #error "PF8 can not be configured as SPI5 MISO on selected device!"
      #endif

      // PH7 as SPI5 MISO only available on STM32F427xx, STM32F429xx, STM32F437xx and STM32F439xx
      #if (RTE_SPI5_MISO_PORT_ID == 5)
        #error "PH7 can not be configured as SPI5 MISO on selected device!"
      #endif
    #endif
    #define MX_SPI5_MISO_Pin        1U
    #define MX_SPI5_MISO_GPIOx      RTE_SPI5_MISO_PORT
    #define MX_SPI5_MISO_GPIO_Pin   (1U << RTE_SPI5_MISO_BIT)
    #define MX_SPI5_MISO_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI5_MISO_GPIO_Speed SPI5_GPIO_SPEED_FREQ

    #if defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) || \
        defined(STM32F413xx) || defined(STM32F423xx)
    #define MX_SPI5_MISO_GPIO_AF   GPIO_AF6_SPI5
    #else
    #define MX_SPI5_MISO_GPIO_AF   GPIO_AF5_SPI5
    #endif
  #endif

  #if (RTE_SPI5_MOSI == 1)
    #if defined (STM32F410Cx) || defined (STM32F410Rx)
      // SPI5 MOSI available on pins: PA10, PB8
      #if ((RTE_SPI5_MOSI_PORT_ID != 1) && (RTE_SPI5_MOSI_PORT_ID != 2))
        #error "Only PA10 and PB8 can be configured as SPI5 MOSI on selected device!"
      #endif
    #elif !defined (STM32F411xE)
      // PA10 as SPI5 MOSI only available on STM32F411xx
      #if (RTE_SPI5_MOSI_PORT_ID == 1)
        #error "PA10 can not be configured as SPI5 MOSI on selected device!"
      #endif
 
      // PB8 as SPI5 MOSI only available on STM32F411xx
      #if (RTE_SPI5_MOSI_PORT_ID == 2)
        #error "PB8 can not be configured as SPI5 MOSI on selected device!"
      #endif

      // PE6 as SPI5 MOSI only available on STM32F411xx
      #if (RTE_SPI5_MOSI_PORT_ID == 3)
        #error "PE6 can not be configured as SPI5 MOSI on selected device!"
      #endif

      // PE14 as SPI5 MOSI only available on STM32F411xx
      #if (RTE_SPI5_MOSI_PORT_ID == 4)
        #error "PE14 can not be configured as SPI5 MOSI on selected device!"
      #endif
    #else
      // PF9 as SPI5 MOSI only available on STM32F427xx, STM32F429xx, STM32F437xx and STM32F439xx
      #if (RTE_SPI5_MOSI_PORT_ID == 5)
        #error "PF9 can not be configured as SPI5 MOSI on selected device!"
      #endif

      // PF11 as SPI5 MOSI only available on STM32F427xx, STM32F429xx, STM32F437xx and STM32F439xx
      #if (RTE_SPI5_MOSI_PORT_ID == 6)
        #error "PF11 can not be configured as SPI5 MOSI on selected device!"
      #endif
    #endif
    #define MX_SPI5_MOSI_Pin        1U
    #define MX_SPI5_MOSI_GPIOx      RTE_SPI5_MOSI_PORT
    #define MX_SPI5_MOSI_GPIO_Pin   (1U << RTE_SPI5_MOSI_BIT)
    #define MX_SPI5_MOSI_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI5_MOSI_GPIO_Speed SPI5_GPIO_SPEED_FREQ

    #if defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) || \
        defined(STM32F413xx) || defined(STM32F423xx)
    #define MX_SPI5_MOSI_GPIO_AF   GPIO_AF6_SPI5
    #else
    #define MX_SPI5_MOSI_GPIO_AF   GPIO_AF5_SPI5
    #endif
  #endif

  #if defined (STM32F410Cx) || defined (STM32F410Rx)
    // SPI5 SCK available on pin: PB0
    #if (RTE_SPI5_SCL_PORT_ID != 0)
      #error "Only PB0 can be configured as SPI5 SCK on selected device!"
    #endif
  #elif !defined (STM32F411xE)
    // PB0 as SPI5 SCK only available on STM32F411xx
    #if (RTE_SPI5_SCL_PORT_ID == 0)
      #error "PB0 can not be configured as SPI5 SCK on selected device!"
    #endif
 
    // PE2 as SPI5 SCK only available on STM32F411xx
    #if (RTE_SPI5_SCL_PORT_ID == 1)
      #error "PE2 can not be configured as SPI5 SCK on selected device!"
    #endif

    // PE12 as SPI5 SCK only available on STM32F411xx
    #if (RTE_SPI5_SCL_PORT_ID == 2)
      #error "PE12 can not be configured as SPI5 SCK on selected device!"
    #endif
  #else
    // PF7 as SPI5 SCK only available on STM32F427xx, STM32F429xx, STM32F437xx and STM32F439xx
    #if (RTE_SPI5_SCL_PORT_ID == 3)
      #error "PF7 can not be configured as SPI5 SCK on selected device!"
    #endif

    // PH6 as SPI5 SCK only available on STM32F427xx, STM32F429xx, STM32F437xx and STM32F439xx
    #if (RTE_SPI5_SCL_PORT_ID == 4)
      #error "PH6 can not be configured as SPI5 SCK on selected device!"
    #endif
  #endif
  #define MX_SPI5_SCK_Pin        1U
  #define MX_SPI5_SCK_GPIOx      RTE_SPI5_SCL_PORT
  #define MX_SPI5_SCK_GPIO_Pin   (1U << RTE_SPI5_SCL_BIT)
  #define MX_SPI5_SCK_GPIO_PuPd  GPIO_NOPULL
  #define MX_SPI5_SCK_GPIO_Speed SPI5_GPIO_SPEED_FREQ

  #if defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) || \
      defined(STM32F413xx) || defined(STM32F423xx)
  #define MX_SPI5_SCK_GPIO_AF   GPIO_AF6_SPI5
  #else
  #define MX_SPI5_SCK_GPIO_AF   GPIO_AF5_SPI5
  #endif


  #if (RTE_SPI5_NSS_PIN == 1)
    #if defined (STM32F410Cx) || defined (STM32F410Rx)
      // SPI5 NSS available on pin: PB1
      #if ((RTE_SPI5_NSS_PORT_ID != 0) && (RTE_SPI5_NSS_PORT_ID != 1))
        #error "Only PB1 can be configured as SPI5 NSS on selected device!"
      #endif
    #elif !defined (STM32F411xE)
      // PB1 as SPI5 NSS only available on STM32F411xx
      #if (RTE_SPI5_NSS_PORT_ID == 0)
        #error "PB1 can not be configured as SPI5 NSS on selected device!"
      #endif
 
      // PE4 as SPI5 NSS only available on STM32F411xx
      #if (RTE_SPI5_NSS_PORT_ID == 1)
        #error "PE4 can not be configured as SPI5 NSS on selected device!"
      #endif

      // PE11 as SPI5 NSS only available on STM32F411xx
      #if (RTE_SPI5_NSS_PORT_ID == 2)
        #error "PE11 can not be configured as SPI5 NSS on selected device!"
      #endif
    #else
      // PF6 as SPI5 NSS only available on STM32F427xx, STM32F429xx, STM32F437xx and STM32F439xx
      #if (RTE_SPI5_NSS_PORT_ID == 3)
        #error "PF6 can not be configured as SPI5 NSS on selected device!"
      #endif

      // PH5 as SPI5 NSS only available on STM32F427xx, STM32F429xx, STM32F437xx and STM32F439xx
      #if (RTE_SPI5_NSS_PORT_ID == 4)
        #error "PH5 can not be configured as SPI5 NSS on selected device!"
      #endif
    #endif
    #define MX_SPI5_NSS_Pin        1U
    #define MX_SPI5_NSS_GPIOx      RTE_SPI5_NSS_PORT
    #define MX_SPI5_NSS_GPIO_Pin  (1U << RTE_SPI5_NSS_BIT)
    #define MX_SPI5_NSS_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI5_NSS_GPIO_Speed SPI5_GPIO_SPEED_FREQ

    #if defined(STM32F413xx) || defined(STM32F423xx)
    #define MX_SPI5_NSS_GPIO_AF    GPIO_AF6_SPI5
    #else
    #define MX_SPI5_NSS_GPIO_AF    GPIO_AF5_SPI5
    #endif
  #endif
#endif

// SPI6 configuration definitions
#if (defined (STM32F413xx) && defined (STM32F423xx)) && defined (SPI6)
  // SPI6 not available
  #undef SPI6
#endif

#if (RTE_SPI6 == 1)

  #ifndef SPI6
    #error "SPI6 not available for selected device!"
  #endif

  #define MX_SPI6

  #if (RTE_SPI6_RX_DMA == 1)
    #define MX_SPI6_RX_DMA_Instance DMAx_STREAMy(RTE_SPI6_RX_DMA_NUMBER, RTE_SPI6_RX_DMA_STREAM)
    #define MX_SPI6_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_SPI6_RX_DMA_NUMBER, RTE_SPI6_RX_DMA_STREAM)
    #define MX_SPI6_RX_DMA_Channel  DMA_CHANNEL_x(RTE_SPI6_RX_DMA_CHANNEL)
    #define MX_SPI6_RX_DMA_Priority DMA_PRIORITY(RTE_SPI6_RX_DMA_PRIORITY)

    #define SPI6_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_SPI6_RX_DMA_NUMBER, RTE_SPI6_RX_DMA_STREAM)
  #endif
  #if (RTE_SPI6_TX_DMA == 1)
    #define MX_SPI6_TX_DMA_Instance DMAx_STREAMy(RTE_SPI6_TX_DMA_NUMBER, RTE_SPI6_TX_DMA_STREAM)
    #define MX_SPI6_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_SPI6_TX_DMA_NUMBER, RTE_SPI6_TX_DMA_STREAM)
    #define MX_SPI6_TX_DMA_Channel  DMA_CHANNEL_x(RTE_SPI6_TX_DMA_CHANNEL)
    #define MX_SPI6_TX_DMA_Priority DMA_PRIORITY(RTE_SPI6_TX_DMA_PRIORITY)

    #define SPI6_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_SPI6_TX_DMA_NUMBER, RTE_SPI6_TX_DMA_STREAM)
  #endif

  #ifndef SPI6_GPIO_SPEED_FREQ
    #define SPI6_GPIO_SPEED_FREQ  GPIO_SPEED_FREQ_HIGH
  #endif

  #define MX_SPI6_SCK_Pin        1U
  #define MX_SPI6_SCK_GPIOx      RTE_SPI6_SCL_PORT
  #define MX_SPI6_SCK_GPIO_Pin   (1U << RTE_SPI6_SCL_BIT)
  #define MX_SPI6_SCK_GPIO_PuPd  GPIO_NOPULL
  #define MX_SPI6_SCK_GPIO_Speed SPI6_GPIO_SPEED_FREQ
  #define MX_SPI6_SCK_GPIO_AF    GPIO_AF5_SPI6

  #if (RTE_SPI6_MISO == 1U)
    #define MX_SPI6_MISO_Pin        1U
    #define MX_SPI6_MISO_GPIOx      RTE_SPI6_MISO_PORT
    #define MX_SPI6_MISO_GPIO_Pin   (1U << RTE_SPI6_MISO_BIT)
    #define MX_SPI6_MISO_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI6_MISO_GPIO_Speed SPI6_GPIO_SPEED_FREQ
    #define MX_SPI6_MISO_GPIO_AF    GPIO_AF5_SPI6
  #endif

  #if (RTE_SPI6_MOSI == 1U)
    #define MX_SPI6_MOSI_Pin        1U
    #define MX_SPI6_MOSI_GPIOx      RTE_SPI6_MOSI_PORT
    #define MX_SPI6_MOSI_GPIO_Pin   (1U << RTE_SPI6_MOSI_BIT)
    #define MX_SPI6_MOSI_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI6_MOSI_GPIO_Speed SPI6_GPIO_SPEED_FREQ
    #define MX_SPI6_MOSI_GPIO_AF    GPIO_AF5_SPI6
  #endif

  #if (RTE_SPI6_NSS_PIN == 1U)
    #define MX_SPI6_NSS_Pin        1U
    #define MX_SPI6_NSS_GPIOx      RTE_SPI6_NSS_PORT
    #define MX_SPI6_NSS_GPIO_Pin  (1U << RTE_SPI6_NSS_BIT)
    #define MX_SPI6_NSS_GPIO_PuPd  GPIO_NOPULL
    #define MX_SPI6_NSS_GPIO_Speed SPI6_GPIO_SPEED_FREQ
    #define MX_SPI6_NSS_GPIO_AF    GPIO_AF5_SPI6
  #endif
#endif

#endif /* RTE_DEVICE_FRAMEWORK_CLASSIC */

#if defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
  #if ((defined(RTE_Drivers_SPI1) || \
        defined(RTE_Drivers_SPI2) || \
        defined(RTE_Drivers_SPI3) || \
        defined(RTE_Drivers_SPI4) || \
        defined(RTE_Drivers_SPI5) || \
        defined(RTE_Drivers_SPI6))   \
        && (!defined (MX_SPI1))      \
        && (!defined (MX_SPI2))      \
        && (!defined (MX_SPI3))      \
        && (!defined (MX_SPI4))      \
        && (!defined (MX_SPI5))      \
        && (!defined (MX_SPI6)))
    #error "SPI not configured in STM32CubeMX!"
  #endif

#endif /* RTE_DEVICE_FRAMEWORK_CUBE_MX */


#ifdef MX_SPI1
#if (defined(MX_SPI1_RX_DMA_Instance) || defined(MX_SPI1_TX_DMA_Instance))
#ifndef MX_SPI1_RX_DMA_Instance
  #error "SPI1 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_SPI1_TX_DMA_Instance
  #error "SPI1 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_SPI2
#if (defined(MX_SPI2_RX_DMA_Instance) || defined(MX_SPI2_TX_DMA_Instance))
#ifndef MX_SPI2_RX_DMA_Instance
  #error "SPI2 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_SPI2_TX_DMA_Instance
  #error "SPI2 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_SPI3
#if (defined(MX_SPI3_RX_DMA_Instance) || defined(MX_SPI3_TX_DMA_Instance))
#ifndef MX_SPI3_RX_DMA_Instance
  #error "SPI3 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_SPI3_TX_DMA_Instance
  #error "SPI3 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_SPI4
#if (defined(MX_SPI4_RX_DMA_Instance) || defined(MX_SPI4_TX_DMA_Instance))
#ifndef MX_SPI4_RX_DMA_Instance
  #error "SPI4 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_SPI4_TX_DMA_Instance
  #error "SPI4 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_SPI5
#if (defined(MX_SPI5_RX_DMA_Instance) || defined(MX_SPI5_TX_DMA_Instance))
#ifndef MX_SPI5_RX_DMA_Instance
  #error "SPI5 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_SPI5_TX_DMA_Instance
  #error "SPI5 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_SPI6
#if (defined(MX_SPI6_RX_DMA_Instance) || defined(MX_SPI6_TX_DMA_Instance))
#ifndef MX_SPI6_RX_DMA_Instance
  #error "SPI6 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_SPI6_TX_DMA_Instance
  #error "SPI6 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif


#if ((defined(MX_SPI1) && defined(MX_SPI1_RX_DMA_Instance)) || \
     (defined(MX_SPI2) && defined(MX_SPI2_RX_DMA_Instance)) || \
     (defined(MX_SPI3) && defined(MX_SPI3_RX_DMA_Instance)) || \
     (defined(MX_SPI4) && defined(MX_SPI4_RX_DMA_Instance)) || \
     (defined(MX_SPI5) && defined(MX_SPI5_RX_DMA_Instance)) || \
     (defined(MX_SPI6) && defined(MX_SPI6_RX_DMA_Instance)))
#define __SPI_DMA_RX
#endif
#if ((defined(MX_SPI1) && defined(MX_SPI1_TX_DMA_Instance)) || \
     (defined(MX_SPI2) && defined(MX_SPI2_TX_DMA_Instance)) || \
     (defined(MX_SPI3) && defined(MX_SPI3_TX_DMA_Instance)) || \
     (defined(MX_SPI4) && defined(MX_SPI4_TX_DMA_Instance)) || \
     (defined(MX_SPI5) && defined(MX_SPI5_TX_DMA_Instance)) || \
     (defined(MX_SPI6) && defined(MX_SPI6_TX_DMA_Instance)))
#define __SPI_DMA_TX
#endif
#if (defined(__SPI_DMA_RX) && defined(__SPI_DMA_TX))
#define __SPI_DMA
#endif


// Current driver status flag definition
#define SPI_INITIALIZED           ((uint8_t)(1U))           // SPI initialized
#define SPI_POWERED               ((uint8_t)(1U << 1))     // SPI powered on
#define SPI_CONFIGURED            ((uint8_t)(1U << 2))     // SPI configured
#define SPI_DATA_LOST             ((uint8_t)(1U << 3))     // SPI data lost occurred
#define SPI_MODE_FAULT            ((uint8_t)(1U << 4))     // SPI mode fault occurred


// DMA Callback functions
typedef void (*DMA_Callback_t) (DMA_HandleTypeDef *hdma);


// SPI DMA
typedef struct _SPI_DMA {
  DMA_HandleTypeDef    *hdma;           // DMA handle
  DMA_Callback_t        cb_complete;    // DMA complete callback
#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
  DMA_Stream_TypeDef   *stream;         // Stream register interface
  uint32_t              channel;        // DMA channel
  uint32_t              priority;       // DMA channel priority
  int8_t                irq_num;        // Stream IRQ number
  uint8_t               reserved[3];    // Reserved
#endif
} SPI_DMA;

// SPI pin
typedef const struct _SPI_PIN {
  GPIO_TypeDef         *port;           // Port
  uint32_t              pin;            // Pin
  uint32_t              af;             // Alternate function
  uint8_t               pupd;           // Pull up/down
  uint8_t               speed;          // Speed
  uint16_t              reserved;       // Reserved
} SPI_PIN;

// SPI Input/Output Configuration
typedef const struct _SPI_IO {
  SPI_PIN              *mosi;           // Pointer to MOSI pin configuration
  SPI_PIN              *miso;           // Pointer to MISO pin configuration
  SPI_PIN              *sck;            // Pointer to SCK pin configuration
  SPI_PIN              *nss;            // Pointer to NSS pin configuration
} SPI_IO;

typedef struct _SPI_STATUS {
  uint8_t busy;                         // Transmitter/Receiver busy flag
  uint8_t data_lost;                    // Data lost: Receive overflow / Transmit underflow (cleared on start of transfer operation)
  uint8_t mode_fault;                   // Mode fault detected; optional (cleared on start of transfer operation)
  uint8_t reserved;                     // Reserved
} SPI_STATUS;

// SPI Information (Run-time)
typedef struct _SPI_INFO {
  ARM_SPI_SignalEvent_t cb_event;       // Event Callback
  SPI_STATUS            status;         // Status flags
  uint32_t              state;          // Current SPI state
  uint32_t              mode;           // Current SPI mode
} SPI_INFO;

// SPI Transfer Information (Run-Time)
typedef struct _SPI_TRANSFER_INFO {
  uint32_t              num;            // Total number of transfers
  uint8_t              *rx_buf;         // Pointer to in data buffer
  uint8_t              *tx_buf;         // Pointer to out data buffer
  uint32_t              rx_cnt;         // Number of data received
  uint32_t              tx_cnt;         // Number of data sent
  uint32_t              dump_val;       // Variable for dumping DMA data
  uint16_t              def_val;        // Default transfer value
  uint16_t              reserved;       // Reserved
} SPI_TRANSFER_INFO;


// SPI Resources definition
typedef struct {
#ifdef RTE_DEVICE_FRAMEWORK_CUBE_MX
  SPI_HandleTypeDef    *h;                    // SPI Handle
#endif
  SPI_TypeDef          *reg;                  // SPI peripheral pointer
  uint32_t            (*periph_clock)(void);  // Peripheral bus clock
  SPI_IO                io;                   // SPI Input/Output pins
  int8_t                irq_num;              // SPI IRQ Number
  uint8_t               reserved[3];          // Reserved
  SPI_DMA              *rx_dma;               // Receive stream register interface
  SPI_DMA              *tx_dma;               // Transmit stream register interface
  SPI_INFO             *info;                 // Run-Time Information
  SPI_TRANSFER_INFO    *xfer;                 // SPI transfer information
} SPI_RESOURCES;

// Global functions and variables exported by driver .c module
#ifdef MX_SPI1
extern void SPI1_IRQHandler     (void);
extern void SPI1_RX_DMA_Handler (void);
extern void SPI1_TX_DMA_Handler (void);

extern ARM_DRIVER_SPI Driver_SPI1;
#endif

#ifdef MX_SPI2
extern void SPI2_IRQHandler     (void);
extern void SPI2_RX_DMA_Handler (void);
extern void SPI2_TX_DMA_Handler (void);

extern ARM_DRIVER_SPI Driver_SPI2;
#endif

#ifdef MX_SPI3
extern void SPI3_IRQHandler     (void);
extern void SPI3_RX_DMA_Handler (void);
extern void SPI3_TX_DMA_Handler (void);

extern ARM_DRIVER_SPI Driver_SPI3;
#endif

#ifdef MX_SPI4
extern void SPI4_IRQHandler     (void);
extern void SPI4_RX_DMA_Handler (void);
extern void SPI4_TX_DMA_Handler (void);

extern ARM_DRIVER_SPI Driver_SPI4;
#endif

#ifdef MX_SPI5
extern void SPI5_IRQHandler     (void);
extern void SPI5_RX_DMA_Handler (void);
extern void SPI5_TX_DMA_Handler (void);

extern ARM_DRIVER_SPI Driver_SPI5;
#endif

#ifdef MX_SPI6
extern void SPI6_IRQHandler     (void);
extern void SPI6_RX_DMA_Handler (void);
extern void SPI6_TX_DMA_Handler (void);

extern ARM_DRIVER_SPI Driver_SPI6;
#endif

#endif /* __SPI_STM32F4XX_H */
