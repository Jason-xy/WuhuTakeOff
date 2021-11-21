/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2019 Arm Limited (or its affiliates). All 
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
 * $Date:        20. November 2019
 * $Revision:    V1.7
 *
 * Driver:       Driver_CAN1/2
 * Configured:   via RTE_Device.h configuration file
 * Project:      CAN Driver Header for ST STM32F4xx
 * -------------------------------------------------------------------------- */

#ifndef __CAN_STM32F4XX_H
#define __CAN_STM32F4XX_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "cmsis_compiler.h"

#include "Driver_CAN.h"
#include "stm32f4xx_hal.h"

#include "RTE_Components.h"
#if    (defined(RTE_DEVICE_FRAMEWORK_CLASSIC))
#include "RTE_Device.h"
#elif  (defined(RTE_DEVICE_FRAMEWORK_CUBE_MX))
#include "MX_Device.h"
#else
#error "::Device:STM32Cube Framework: not selected in RTE"
#endif

#if    (defined(RTE_DEVICE_FRAMEWORK_CLASSIC))

#if   ((defined(RTE_Drivers_CAN1) ||  defined(RTE_Drivers_CAN2) ||  defined(RTE_Drivers_CAN3)) && \
      ((RTE_CAN1 == 0)            && (RTE_CAN2 == 0)            && (RTE_CAN3 == 0)))
#error "No CAN configured in RTE_Device.h!"
#endif

#if    (RTE_CAN1 != 0)

  #if defined (STM32F413xx) || defined (STM32F423xx)
    /* CAN1_RX available on pins: PA11, PB8, PD0, PG0 */
    #if (RTE_CAN1_RX_PORT_ID == 3)
      #error "Only PA11, PB8, PD0, PG0 can be configured as CAN1 RX on selected device!"
    #endif
    /* CAN1_TX available on pins: PA12, PB9, PD1, PG1 */
    #if (RTE_CAN1_TX_PORT_ID == 3)
      #error "Only PA12, PB9, PD1, PG1 can be configured as CAN1 TX on selected device!"
    #endif
  #endif

#define MX_CAN1                         1

/* Pin CAN1_RX */
#define MX_CAN1_RX_Pin                  1
#define MX_CAN1_RX_GPIO_Speed           GPIO_SPEED_HIGH
#define MX_CAN1_RX_GPIOx                RTE_CAN1_RX_PORT
#define MX_CAN1_RX_GPIO_PuPd            GPIO_NOPULL
#define MX_CAN1_RX_GPIO_Pin             RTE_CAN1_RX_BIT
#if (RTE_CAN1_RX_PORT_ID == 1) && \
    (defined (STM32F412Cx) || defined (STM32F412Rx) || defined (STM32F412Vx) || defined (STM32F412Zx) || \
     defined (STM32F413xx) || defined (STM32F423xx) )
  #define MX_CAN1_RX_GPIO_AF            GPIO_AF8_CAN1
#else
  #define MX_CAN1_RX_GPIO_AF            GPIO_AF9_CAN1
#endif
#define MX_CAN1_RX_GPIO_Mode            GPIO_MODE_AF_PP

/* Pin CAN1_TX */
#define MX_CAN1_TX_Pin                  1
#define MX_CAN1_TX_GPIO_Speed           GPIO_SPEED_HIGH
#define MX_CAN1_TX_GPIOx                RTE_CAN1_TX_PORT
#define MX_CAN1_TX_GPIO_PuPd            GPIO_NOPULL
#define MX_CAN1_TX_GPIO_Pin             RTE_CAN1_TX_BIT
#if (RTE_CAN1_TX_PORT_ID == 1) && \
    (defined (STM32F412Cx) || defined (STM32F412Rx) || defined (STM32F412Vx) || defined (STM32F412Zx) || \
     defined (STM32F413xx) || defined (STM32F423xx) )
  #define MX_CAN1_TX_GPIO_AF            GPIO_AF8_CAN1
#else
  #define MX_CAN1_TX_GPIO_AF            GPIO_AF9_CAN1
#endif
#define MX_CAN1_TX_GPIO_Mode            GPIO_MODE_AF_PP
#endif

#if    (RTE_CAN2 != 0)
#define MX_CAN2                         1

/* Pin CAN2_RX */
#define MX_CAN2_RX_Pin                  1
#define MX_CAN2_RX_GPIO_Speed           GPIO_SPEED_HIGH
#define MX_CAN2_RX_GPIOx                RTE_CAN2_RX_PORT
#define MX_CAN2_RX_GPIO_PuPd            GPIO_NOPULL
#define MX_CAN2_RX_GPIO_Pin             RTE_CAN2_RX_BIT
#define MX_CAN2_RX_GPIO_AF              GPIO_AF9_CAN2
#define MX_CAN2_RX_GPIO_Mode            GPIO_MODE_AF_PP

/* Pin CAN2_TX */
#define MX_CAN2_TX_Pin                  1
#define MX_CAN2_TX_GPIO_Speed           GPIO_SPEED_HIGH
#define MX_CAN2_TX_GPIOx                RTE_CAN2_TX_PORT
#define MX_CAN2_TX_GPIO_PuPd            GPIO_NOPULL
#define MX_CAN2_TX_GPIO_Pin             RTE_CAN2_TX_BIT
#define MX_CAN2_TX_GPIO_AF              GPIO_AF9_CAN2
#define MX_CAN2_TX_GPIO_Mode            GPIO_MODE_AF_PP
#endif

#if    (RTE_CAN3 != 0)
#define MX_CAN3                         1

/* Pin CAN3_RX */
#define MX_CAN3_RX_Pin                  1
#define MX_CAN3_RX_GPIO_Speed           GPIO_SPEED_HIGH
#define MX_CAN3_RX_GPIOx                RTE_CAN3_RX_PORT
#define MX_CAN3_RX_GPIO_PuPd            GPIO_NOPULL
#define MX_CAN3_RX_GPIO_Pin             RTE_CAN3_RX_BIT
#define MX_CAN3_RX_GPIO_AF              GPIO_AF11_CAN3
#define MX_CAN3_RX_GPIO_Mode            GPIO_MODE_AF_PP

/* Pin CAN3_TX */
#define MX_CAN3_TX_Pin                  1
#define MX_CAN3_TX_GPIO_Speed           GPIO_SPEED_HIGH
#define MX_CAN3_TX_GPIOx                RTE_CAN3_TX_PORT
#define MX_CAN3_TX_GPIO_PuPd            GPIO_NOPULL
#define MX_CAN3_TX_GPIO_Pin             RTE_CAN3_TX_BIT
#define MX_CAN3_TX_GPIO_AF              GPIO_AF11_CAN3
#define MX_CAN3_TX_GPIO_Mode            GPIO_MODE_AF_PP
#endif

#elif  (defined(RTE_DEVICE_FRAMEWORK_CUBE_MX))

#if   ((defined(RTE_Drivers_CAN1) ||   defined(RTE_Drivers_CAN2) ||   defined(RTE_Drivers_CAN3)) && \
      (!defined(MX_CAN1))         && (!defined(MX_CAN2))         && (!defined(MX_CAN3)))
#error "No CAN configured in STM32CubeMX!"
#endif

#endif

#if    !defined(MX_CAN1)
#define MX_CAN1                         (0U)
#endif
#if    !defined(MX_CAN2)
#define MX_CAN2                         (0U)
#endif
#if    !defined(MX_CAN3)
#define MX_CAN3                         (0U)
#endif

#if    (MX_CAN3 == 1U)
#define CAN_CTRL_NUM                    (3U)
#elif  (MX_CAN2 == 1U)
#define CAN_CTRL_NUM                    (2U)
#else
#define CAN_CTRL_NUM                    (1U)
#endif

// Global functions and variables exported by driver .c module
#if (MX_CAN1 == 1U)
extern void CAN1_TX_IRQHandler  (void);
extern void CAN1_RX0_IRQHandler (void);
extern void CAN1_RX1_IRQHandler (void);
extern void CAN1_SCE_IRQHandler (void);

extern ARM_DRIVER_CAN Driver_CAN1;
#endif

#if (MX_CAN2 == 1U)
extern void CAN2_TX_IRQHandler  (void);
extern void CAN2_RX0_IRQHandler (void);
extern void CAN2_RX1_IRQHandler (void);
extern void CAN2_SCE_IRQHandler (void);

extern ARM_DRIVER_CAN Driver_CAN2;
#endif

#if (MX_CAN3 == 1U)
extern void CAN3_TX_IRQHandler  (void);
extern void CAN3_RX0_IRQHandler (void);
extern void CAN3_RX1_IRQHandler (void);
extern void CAN3_SCE_IRQHandler (void);

extern ARM_DRIVER_CAN Driver_CAN3;
#endif

#endif // __CAN_STM32F4XX_H
