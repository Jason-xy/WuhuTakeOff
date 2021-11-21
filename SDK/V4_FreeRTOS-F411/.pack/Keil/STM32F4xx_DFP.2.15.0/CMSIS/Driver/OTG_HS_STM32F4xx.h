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
 * $Revision:    V1.12
 *
 * Project:      OTG High-Speed Driver Header for ST STM32F4xx
 * -------------------------------------------------------------------------- */

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "RTE_Components.h"

#include "cmsis_compiler.h"

#include "Driver_USBH.h"
#include "Driver_USBD.h"

#ifdef    RTE_DEVICE_FRAMEWORK_CLASSIC
#include "RTE_Device.h"

// Do basic RTE configuration check
#if    (RTE_USB_OTG_HS == 1)
#if   ((RTE_USB_OTG_HS_HOST == 1) && (RTE_USB_OTG_HS_PHY == 0) && (RTE_OTG_HS_VBUS_PIN == 0))
#error  Configure VBUS Power On/Off pin for USB OTG High-speed in RTE_Device.h!
#endif
#else
#error  Enable USB OTG High-speed in RTE_Device.h!
#endif

// If Framework Classic is used create MX defines from RTE defines

#ifdef RTE_Drivers_USBD1
#define MX_USB_OTG_HS_DEVICE                    1
#endif
#ifdef RTE_Drivers_USBH1
#define MX_USB_OTG_HS_HOST                      1
#endif

#define MX_USB_OTG_HS_GPIO_PIN_(n)              GPIO_PIN_##n
#define MX_USB_OTG_HS_GPIO_PIN(n)               MX_USB_OTG_HS_GPIO_PIN_(n)

#if    (RTE_USB_OTG_HS_PHY == 0)                // Internal Full-speed PHY
#define MX_USB_OTG_HS_INT_FS_PHY               (1U)
#elif  (RTE_USB_OTG_HS_PHY == 1)                // External ULPI High-speed PHY
#define MX_USB_OTG_HS_EXT_HS_PHY               (1U)
#endif

#ifndef MX_USB_OTG_HS_INT_FS_PHY
#define MX_USB_OTG_HS_INT_FS_PHY               (0U)
#endif
#ifndef MX_USB_OTG_HS_EXT_HS_PHY
#define MX_USB_OTG_HS_EXT_HS_PHY               (0U)
#endif


#if     (RTE_USB_OTG_HS_PHY == 1)               // External ULPI High-speed PHY

// Pin USB_OTG_HS_ULPI_DIR : PI11 or PC2
#define MX_USB_OTG_HS_ULPI_DIR_Pin              PI11_PC2
#define MX_USB_OTG_HS_ULPI_DIR_GPIO_AF          GPIO_AF10_OTG_HS
#define MX_USB_OTG_HS_ULPI_DIR_GPIO_Speed       GPIO_SPEED_HIGH
#define MX_USB_OTG_HS_ULPI_DIR_GPIO_PuPd        GPIO_NOPULL
#define MX_USB_OTG_HS_ULPI_DIR_GPIOx            RTE_USB_OTG_HS_ULPI_DIR_PORT
#define MX_USB_OTG_HS_ULPI_DIR_GPIO_Mode        GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ULPI_DIR_GPIO_Pin         MX_USB_OTG_HS_GPIO_PIN(RTE_USB_OTG_HS_ULPI_DIR_PIN)

// Pin USB_OTG_HS_ULPI_CK : PA5
#define MX_USB_OTG_HS_ULPI_CK_Pin               PA5
#define MX_USB_OTG_HS_ULPI_CK_GPIO_AF           GPIO_AF10_OTG_HS
#define MX_USB_OTG_HS_ULPI_CK_GPIO_Speed        GPIO_SPEED_HIGH
#define MX_USB_OTG_HS_ULPI_CK_GPIO_PuPd         GPIO_NOPULL
#define MX_USB_OTG_HS_ULPI_CK_GPIOx             RTE_USB_OTG_HS_ULPI_CK_PORT
#define MX_USB_OTG_HS_ULPI_CK_GPIO_Mode         GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ULPI_CK_GPIO_Pin          MX_USB_OTG_HS_GPIO_PIN(RTE_USB_OTG_HS_ULPI_CK_PIN)

// Pin USB_OTG_HS_ULPI_NXT : PC3 or PH4
#define MX_USB_OTG_HS_ULPI_NXT_Pin              PC3_PH4
#define MX_USB_OTG_HS_ULPI_NXT_GPIO_AF          GPIO_AF10_OTG_HS
#define MX_USB_OTG_HS_ULPI_NXT_GPIO_Speed       GPIO_SPEED_HIGH
#define MX_USB_OTG_HS_ULPI_NXT_GPIO_PuPd        GPIO_NOPULL
#define MX_USB_OTG_HS_ULPI_NXT_GPIOx            RTE_USB_OTG_HS_ULPI_NXT_PORT
#define MX_USB_OTG_HS_ULPI_NXT_GPIO_Mode        GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ULPI_NXT_GPIO_Pin         MX_USB_OTG_HS_GPIO_PIN(RTE_USB_OTG_HS_ULPI_NXT_PIN)

// Pin USB_OTG_HS_ULPI_D0 : PA3
#define MX_USB_OTG_HS_ULPI_D0_Pin               PA3
#define MX_USB_OTG_HS_ULPI_D0_GPIO_AF           GPIO_AF10_OTG_HS
#define MX_USB_OTG_HS_ULPI_D0_GPIO_Speed        GPIO_SPEED_HIGH
#define MX_USB_OTG_HS_ULPI_D0_GPIO_PuPd         GPIO_NOPULL
#define MX_USB_OTG_HS_ULPI_D0_GPIOx             RTE_USB_OTG_HS_ULPI_D0_PORT
#define MX_USB_OTG_HS_ULPI_D0_GPIO_Mode         GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ULPI_D0_GPIO_Pin          MX_USB_OTG_HS_GPIO_PIN(RTE_USB_OTG_HS_ULPI_D0_PIN)

// Pin USB_OTG_HS_ULPI_D3 : PB10
#define MX_USB_OTG_HS_ULPI_D3_Pin               PB10
#define MX_USB_OTG_HS_ULPI_D3_GPIO_AF           GPIO_AF10_OTG_HS
#define MX_USB_OTG_HS_ULPI_D3_GPIO_Speed        GPIO_SPEED_HIGH
#define MX_USB_OTG_HS_ULPI_D3_GPIO_PuPd         GPIO_NOPULL
#define MX_USB_OTG_HS_ULPI_D3_GPIOx             RTE_USB_OTG_HS_ULPI_D3_PORT
#define MX_USB_OTG_HS_ULPI_D3_GPIO_Mode         GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ULPI_D3_GPIO_Pin          MX_USB_OTG_HS_GPIO_PIN(RTE_USB_OTG_HS_ULPI_D3_PIN)

// Pin USB_OTG_HS_ULPI_D4 : PB11
#define MX_USB_OTG_HS_ULPI_D4_Pin               PB11
#define MX_USB_OTG_HS_ULPI_D4_GPIO_AF           GPIO_AF10_OTG_HS
#define MX_USB_OTG_HS_ULPI_D4_GPIO_Speed        GPIO_SPEED_HIGH
#define MX_USB_OTG_HS_ULPI_D4_GPIO_PuPd         GPIO_NOPULL
#define MX_USB_OTG_HS_ULPI_D4_GPIOx             RTE_USB_OTG_HS_ULPI_D4_PORT
#define MX_USB_OTG_HS_ULPI_D4_GPIO_Mode         GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ULPI_D4_GPIO_Pin          MX_USB_OTG_HS_GPIO_PIN(RTE_USB_OTG_HS_ULPI_D4_PIN)

// Pin USB_OTG_HS_ULPI_D1 : PB0
#define MX_USB_OTG_HS_ULPI_D1_Pin               PB0
#define MX_USB_OTG_HS_ULPI_D1_GPIO_AF           GPIO_AF10_OTG_HS
#define MX_USB_OTG_HS_ULPI_D1_GPIO_Speed        GPIO_SPEED_HIGH
#define MX_USB_OTG_HS_ULPI_D1_GPIO_PuPd         GPIO_NOPULL
#define MX_USB_OTG_HS_ULPI_D1_GPIOx             RTE_USB_OTG_HS_ULPI_D1_PORT
#define MX_USB_OTG_HS_ULPI_D1_GPIO_Mode         GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ULPI_D1_GPIO_Pin          MX_USB_OTG_HS_GPIO_PIN(RTE_USB_OTG_HS_ULPI_D1_PIN)

// Pin USB_OTG_HS_ULPI_D2 : PB1
#define MX_USB_OTG_HS_ULPI_D2_Pin               PB1
#define MX_USB_OTG_HS_ULPI_D2_GPIO_AF           GPIO_AF10_OTG_HS
#define MX_USB_OTG_HS_ULPI_D2_GPIO_Speed        GPIO_SPEED_HIGH
#define MX_USB_OTG_HS_ULPI_D2_GPIO_PuPd         GPIO_NOPULL
#define MX_USB_OTG_HS_ULPI_D2_GPIOx             RTE_USB_OTG_HS_ULPI_D2_PORT
#define MX_USB_OTG_HS_ULPI_D2_GPIO_Mode         GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ULPI_D2_GPIO_Pin          MX_USB_OTG_HS_GPIO_PIN(RTE_USB_OTG_HS_ULPI_D2_PIN)

// Pin USB_OTG_HS_ULPI_STP : PC0
#define MX_USB_OTG_HS_ULPI_STP_Pin              PC0
#define MX_USB_OTG_HS_ULPI_STP_GPIO_AF          GPIO_AF10_OTG_HS
#define MX_USB_OTG_HS_ULPI_STP_GPIO_Speed       GPIO_SPEED_HIGH
#define MX_USB_OTG_HS_ULPI_STP_GPIO_PuPd        GPIO_NOPULL
#define MX_USB_OTG_HS_ULPI_STP_GPIOx            RTE_USB_OTG_HS_ULPI_STP_PORT
#define MX_USB_OTG_HS_ULPI_STP_GPIO_Mode        GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ULPI_STP_GPIO_Pin         MX_USB_OTG_HS_GPIO_PIN(RTE_USB_OTG_HS_ULPI_STP_PIN)

// Pin USB_OTG_HS_ULPI_D7 : PB5
#define MX_USB_OTG_HS_ULPI_D7_Pin               PB5
#define MX_USB_OTG_HS_ULPI_D7_GPIO_AF           GPIO_AF10_OTG_HS
#define MX_USB_OTG_HS_ULPI_D7_GPIO_Speed        GPIO_SPEED_HIGH
#define MX_USB_OTG_HS_ULPI_D7_GPIO_PuPd         GPIO_NOPULL
#define MX_USB_OTG_HS_ULPI_D7_GPIOx             RTE_USB_OTG_HS_ULPI_D7_PORT
#define MX_USB_OTG_HS_ULPI_D7_GPIO_Mode         GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ULPI_D7_GPIO_Pin          MX_USB_OTG_HS_GPIO_PIN(RTE_USB_OTG_HS_ULPI_D7_PIN)

// Pin USB_OTG_HS_ULPI_D5 : PB12
#define MX_USB_OTG_HS_ULPI_D5_Pin               PB12
#define MX_USB_OTG_HS_ULPI_D5_GPIO_AF           GPIO_AF10_OTG_HS
#define MX_USB_OTG_HS_ULPI_D5_GPIO_Speed        GPIO_SPEED_HIGH
#define MX_USB_OTG_HS_ULPI_D5_GPIO_PuPd         GPIO_NOPULL
#define MX_USB_OTG_HS_ULPI_D5_GPIOx             RTE_USB_OTG_HS_ULPI_D5_PORT
#define MX_USB_OTG_HS_ULPI_D5_GPIO_Mode         GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ULPI_D5_GPIO_Pin          MX_USB_OTG_HS_GPIO_PIN(RTE_USB_OTG_HS_ULPI_D5_PIN)

// Pin USB_OTG_HS_ULPI_D6 : PB13
#define MX_USB_OTG_HS_ULPI_D6_Pin               PB13
#define MX_USB_OTG_HS_ULPI_D6_GPIO_AF           GPIO_AF10_OTG_HS
#define MX_USB_OTG_HS_ULPI_D6_GPIO_Speed        GPIO_SPEED_HIGH
#define MX_USB_OTG_HS_ULPI_D6_GPIO_PuPd         GPIO_NOPULL
#define MX_USB_OTG_HS_ULPI_D6_GPIOx             RTE_USB_OTG_HS_ULPI_D6_PORT
#define MX_USB_OTG_HS_ULPI_D6_GPIO_Mode         GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ULPI_D6_GPIO_Pin          MX_USB_OTG_HS_GPIO_PIN(RTE_USB_OTG_HS_ULPI_D6_PIN)

#else                                           // On-chip Full-speed PHY

// Pin USB_OTG_HS_ID : PB12
#define MX_USB_OTG_HS_ID_Pin                    PB12
#define MX_USB_OTG_HS_ID_GPIO_AF                GPIO_AF12_OTG_HS_FS
#define MX_USB_OTG_HS_ID_GPIO_Speed             GPIO_SPEED_LOW
#define MX_USB_OTG_HS_ID_GPIO_PuPd              GPIO_NOPULL
#define MX_USB_OTG_HS_ID_GPIOx                  GPIOB
#define MX_USB_OTG_HS_ID_GPIO_Mode              GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ID_GPIO_Pin               GPIO_PIN_12

// Pin USB_OTG_HS_DM : PB14
#define MX_USB_OTG_HS_DM_Pin                    PB14
#define MX_USB_OTG_HS_DM_GPIO_AF                GPIO_AF12_OTG_HS_FS
#define MX_USB_OTG_HS_DM_GPIO_Speed             GPIO_SPEED_LOW
#define MX_USB_OTG_HS_DM_GPIO_PuPd              GPIO_NOPULL
#define MX_USB_OTG_HS_DM_GPIOx                  GPIOB
#define MX_USB_OTG_HS_DM_GPIO_Mode              GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_DM_GPIO_Pin               GPIO_PIN_14

// Pin USB_OTG_HS_DP : PB15
#define MX_USB_OTG_HS_DP_Pin                    PB15
#define MX_USB_OTG_HS_DP_GPIO_AF                GPIO_AF12_OTG_HS_FS
#define MX_USB_OTG_HS_DP_GPIO_Speed             GPIO_SPEED_LOW
#define MX_USB_OTG_HS_DP_GPIO_PuPd              GPIO_NOPULL
#define MX_USB_OTG_HS_DP_GPIOx                  GPIOB
#define MX_USB_OTG_HS_DP_GPIO_Mode              GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_DP_GPIO_Pin               GPIO_PIN_15

#if    ((RTE_USB_OTG_HS_DEVICE == 1) && (RTE_OTG_HS_VBUS_SENSING_PIN == 1))
// Pin USB_OTG_HS_VBUS : PB13
#define MX_USB_OTG_HS_VBUS_Pin                  PB13
#define MX_USB_OTG_HS_VBUS_GPIO_PuPd            GPIO_NOPULL
#define MX_USB_OTG_HS_VBUS_GPIOx                GPIOB
#define MX_USB_OTG_HS_VBUS_GPIO_Mode            GPIO_MODE_INPUT
#define MX_USB_OTG_HS_VBUS_GPIO_Pin             GPIO_PIN_13
#endif

#endif

#if    ((RTE_USB_OTG_HS_HOST == 1) && (RTE_OTG_HS_VBUS_PIN == 1))
// Pin USB_OTG_HS_VBUS_Power : GPIO pin
#define MX_USB_OTG_HS_VBUS_Power_Pin            1
#define MX_USB_OTG_HS_VBUS_Power_GPIO_PuPd      GPIO_NOPULL
#define MX_USB_OTG_HS_VBUS_Power_GPIOx          RTE_OTG_HS_VBUS_PORT
#define MX_USB_OTG_HS_VBUS_Power_GPIO_Mode      GPIO_MODE_OUTPUT_PP
#define MX_USB_OTG_HS_VBUS_Power_GPIO_Pin       MX_USB_OTG_HS_GPIO_PIN(RTE_OTG_HS_VBUS_BIT)
#ifndef    USB_OTG_HS_VBUS_Power_Pin_Active
#define    USB_OTG_HS_VBUS_Power_Pin_Active     RTE_OTG_HS_VBUS_ACTIVE
#endif
#endif

#if    ((RTE_USB_OTG_HS_HOST == 1) && (RTE_OTG_HS_OC_PIN == 1))
// Pin USB_OTG_HS_Overcurrent : GPIO pin
#define MX_USB_OTG_HS_Overcurrent_Pin           1
#define MX_USB_OTG_HS_Overcurrent_GPIO_PuPd     GPIO_NOPULL
#define MX_USB_OTG_HS_Overcurrent_GPIOx         RTE_OTG_HS_OC_PORT
#define MX_USB_OTG_HS_Overcurrent_GPIO_Mode     GPIO_MODE_INPUT
#define MX_USB_OTG_HS_Overcurrent_GPIO_Pin      MX_USB_OTG_HS_GPIO_PIN(RTE_OTG_HS_OC_BIT)
#ifndef    USB_OTG_HS_Overcurrent_Pin_Active
#define    USB_OTG_HS_Overcurrent_Pin_Active    RTE_OTG_HS_OC_ACTIVE
#endif
#endif

// USB_OTG_HS DMA usage
#ifndef    RTE_OTG_HS_DMA
#define    RTE_OTG_HS_DMA                       0
#endif
#ifndef    USB_OTG_HS_DMA
#define    USB_OTG_HS_DMA                       RTE_OTG_HS_DMA
#endif

#elif  (defined(RTE_DEVICE_FRAMEWORK_CUBE_MX))  // #ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
#include "MX_Device.h"

#define CREATE_MX_SYMBOL(mx_def,ext)            MX_##mx_def##_##ext
#define MX_VAL(mx_def,ext)                      CREATE_MX_SYMBOL(mx_def,ext)

// Do basic CubeMX configuration check
#ifndef MX_USB_OTG_HS
#error  Enable USB_OTG_HS in STM32CubeMX!
#endif

#ifdef  MX_USB_OTG_HS_ULPI_D7_GPIO_Speed        // External ULPI High-speed PHY
#define MX_USB_OTG_HS_EXT_HS_PHY               (1U)
#else                                           // Internal Full-speed PHY
#define MX_USB_OTG_HS_INT_FS_PHY               (1U)
#endif

#ifndef MX_USB_OTG_HS_INT_FS_PHY
#define MX_USB_OTG_HS_INT_FS_PHY               (0U)
#endif
#ifndef MX_USB_OTG_HS_EXT_HS_PHY
#define MX_USB_OTG_HS_EXT_HS_PHY               (0U)
#endif

#if     defined(MX_USB_OTG_HS_VBUS_Power)
// Pin USB_OTG_HS_VBUS_Power : GPIO pin
#define MX_USB_OTG_HS_VBUS_Power_Pin            1
#define MX_USB_OTG_HS_VBUS_Power_GPIO_PuPd      MX_VAL(MX_USB_OTG_HS_VBUS_Power, GPIO_PuPd)
#define MX_USB_OTG_HS_VBUS_Power_GPIOx          MX_VAL(MX_USB_OTG_HS_VBUS_Power, GPIOx)
#define MX_USB_OTG_HS_VBUS_Power_GPIO_Mode      MX_VAL(MX_USB_OTG_HS_VBUS_Power, GPIO_ModeDefaultOutputPP)
#define MX_USB_OTG_HS_VBUS_Power_GPIO_Pin       MX_VAL(MX_USB_OTG_HS_VBUS_Power, GPIO_Pin)
#ifndef    USB_OTG_HS_VBUS_Power_Pin_Active
#define    USB_OTG_HS_VBUS_Power_Pin_Active     0
#endif
#endif

#if     defined(MX_USB_OTG_HS_Overcurrent)
// Pin USB_OTG_HS_Overcurrent : GPIO pin
#define MX_USB_OTG_HS_Overcurrent_Pin           1
#define MX_USB_OTG_HS_Overcurrent_GPIO_PuPd     MX_VAL(MX_USB_OTG_HS_Overcurrent, GPIO_PuPd)
#define MX_USB_OTG_HS_Overcurrent_GPIOx         MX_VAL(MX_USB_OTG_HS_Overcurrent, GPIOx)
#define MX_USB_OTG_HS_Overcurrent_GPIO_Mode     MX_VAL(MX_USB_OTG_HS_Overcurrent, Mode)
#define MX_USB_OTG_HS_Overcurrent_GPIO_Pin      MX_VAL(MX_USB_OTG_HS_Overcurrent, GPIO_Pin)
#ifndef    USB_OTG_HS_Overcurrent_Pin_Active
#define    USB_OTG_HS_Overcurrent_Pin_Active    0
#endif
#endif

#ifndef    USB_OTG_HS_DMA
#define    USB_OTG_HS_DMA                       0
#endif

#endif                                          // #ifdef RTE_DEVICE_FRAMEWORK_CLASSIC

#if   (defined (__CC_ARM))
  #pragma push
  #pragma anon_unions
#elif (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
#elif (defined (__GNUC__))
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wc99-c11-compat"
#endif

// OTG_HS
typedef struct
{
  __IO uint32_t GOTGCTL;
  __IO uint32_t GOTGINT;
  __IO uint32_t GAHBCFG;
  __IO uint32_t GUSBCFG;
  __IO uint32_t GRSTCTL;
  __IO uint32_t GINTSTS;
  __IO uint32_t GINTMSK;
  __IO uint32_t GRXSTSR;
  __IO uint32_t GRXSTSP;
  __IO uint32_t GRXFSIZ;
  union {
    __IO uint32_t HNPTXFSIZ;
    __IO uint32_t DIEPTXF0;
  };
  __IO uint32_t HNPTXSTS;
  __IO uint32_t GI2CCTL;
       uint32_t RESERVED0;
  __IO uint32_t GCCFG;
  __IO uint32_t CID;
       uint32_t RESERVED1[48];
  __IO uint32_t HPTXFSIZ;
  __IO uint32_t DIEPTXF1;
  __IO uint32_t DIEPTXF2;
  __IO uint32_t DIEPTXF3;
  __IO uint32_t DIEPTXF4;
  __IO uint32_t DIEPTXF5;
  __IO uint32_t DIEPTXF6;
  __IO uint32_t DIEPTXF7;
  __IO uint32_t DIEPTXF8;
  __IO uint32_t DIEPTXF9;
  __IO uint32_t DIEPTXF10;
  __IO uint32_t DIEPTXF11;
  __IO uint32_t DIEPTXF12;
  __IO uint32_t DIEPTXF13;
  __IO uint32_t DIEPTXF14;
  __IO uint32_t DIEPTXF15;
       uint32_t RESERVED2[176];
  __IO uint32_t HCFG;
  __IO uint32_t HFIR;
  __IO uint32_t HFNUM;
       uint32_t RESERVED3;
  __IO uint32_t HPTXSTS;
  __IO uint32_t HAINT;
  __IO uint32_t HAINTMSK;
       uint32_t RESERVED4[9];
  __IO uint32_t HPRT;
       uint32_t RESERVED5[47];
  __IO uint32_t HCCHAR0;
  __IO uint32_t HCSPLT0;
  __IO uint32_t HCINT0;
  __IO uint32_t HCINTMSK0;
  __IO uint32_t HCTSIZ0;
  __IO uint32_t HCDMA0;
       uint32_t RESERVED6[2];
  __IO uint32_t HCCHAR1;
  __IO uint32_t HCSPLT1;
  __IO uint32_t HCINT1;
  __IO uint32_t HCINTMSK1;
  __IO uint32_t HCTSIZ1;
  __IO uint32_t HCDMA1;
       uint32_t RESERVED7[2];
  __IO uint32_t HCCHAR2;
  __IO uint32_t HCSPLT2;
  __IO uint32_t HCINT2;
  __IO uint32_t HCINTMSK2;
  __IO uint32_t HCTSIZ2;
  __IO uint32_t HCDMA2;
       uint32_t RESERVED8[2];
  __IO uint32_t HCCHAR3;
  __IO uint32_t HCSPLT3;
  __IO uint32_t HCINT3;
  __IO uint32_t HCINTMSK3;
  __IO uint32_t HCTSIZ3;
  __IO uint32_t HCDMA3;
       uint32_t RESERVED9[2];
  __IO uint32_t HCCHAR4;
  __IO uint32_t HCSPLT4;
  __IO uint32_t HCINT4;
  __IO uint32_t HCINTMSK4;
  __IO uint32_t HCTSIZ4;
  __IO uint32_t HCDMA4;
       uint32_t RESERVED10[2];
  __IO uint32_t HCCHAR5;
  __IO uint32_t HCSPLT5;
  __IO uint32_t HCINT5;
  __IO uint32_t HCINTMSK5;
  __IO uint32_t HCTSIZ5;
  __IO uint32_t HCDMA5;
       uint32_t RESERVED11[2];
  __IO uint32_t HCCHAR6;
  __IO uint32_t HCSPLT6;
  __IO uint32_t HCINT6;
  __IO uint32_t HCINTMSK6;
  __IO uint32_t HCTSIZ6;
  __IO uint32_t HCDMA6;
       uint32_t RESERVED12[2];
  __IO uint32_t HCCHAR7;
  __IO uint32_t HCSPLT7;
  __IO uint32_t HCINT7;
  __IO uint32_t HCINTMSK7;
  __IO uint32_t HCTSIZ7;
  __IO uint32_t HCDMA7;
       uint32_t RESERVED13[2];
  __IO uint32_t HCCHAR8;
  __IO uint32_t HCSPLT8;
  __IO uint32_t HCINT8;
  __IO uint32_t HCINTMSK8;
  __IO uint32_t HCTSIZ8;
  __IO uint32_t HCDMA8;
       uint32_t RESERVED14[2];
  __IO uint32_t HCCHAR9;
  __IO uint32_t HCSPLT9;
  __IO uint32_t HCINT9;
  __IO uint32_t HCINTMSK9;
  __IO uint32_t HCTSIZ9;
  __IO uint32_t HCDMA9;
       uint32_t RESERVED15[2];
  __IO uint32_t HCCHAR10;
  __IO uint32_t HCSPLT10;
  __IO uint32_t HCINT10;
  __IO uint32_t HCINTMSK10;
  __IO uint32_t HCTSIZ10;
  __IO uint32_t HCDMA10;
       uint32_t RESERVED16[2];
  __IO uint32_t HCCHAR11;
  __IO uint32_t HCSPLT11;
  __IO uint32_t HCINT11;
  __IO uint32_t HCINTMSK11;
  __IO uint32_t HCTSIZ11;
  __IO uint32_t HCDMA11;
       uint32_t RESERVED17[2];
  __IO uint32_t HCCHAR12;
  __IO uint32_t HCSPLT12;
  __IO uint32_t HCINT12;
  __IO uint32_t HCINTMSK12;
  __IO uint32_t HCTSIZ12;
  __IO uint32_t HCDMA12;
       uint32_t RESERVED18[2];
  __IO uint32_t HCCHAR13;
  __IO uint32_t HCSPLT13;
  __IO uint32_t HCINT13;
  __IO uint32_t HCINTMSK13;
  __IO uint32_t HCTSIZ13;
  __IO uint32_t HCDMA13;
       uint32_t RESERVED19[2];
  __IO uint32_t HCCHAR14;
  __IO uint32_t HCSPLT14;
  __IO uint32_t HCINT14;
  __IO uint32_t HCINTMSK14;
  __IO uint32_t HCTSIZ14;
  __IO uint32_t HCDMA14;
       uint32_t RESERVED20[2];
  __IO uint32_t HCCHAR15;
  __IO uint32_t HCSPLT15;
  __IO uint32_t HCINT15;
  __IO uint32_t HCINTMSK15;
  __IO uint32_t HCTSIZ15;
  __IO uint32_t HCDMA15;
       uint32_t RESERVED21[2];
       uint32_t RESERVED22[64];
  __IO uint32_t DCFG;
  __IO uint32_t DCTL;
  __IO uint32_t DSTS;
       uint32_t RESERVED23;
  __IO uint32_t DIEPMSK;
  __IO uint32_t DOEPMSK;
  __IO uint32_t DAINT;
  __IO uint32_t DAINTMSK;
       uint32_t RESERVED24[2];
  __IO uint32_t DVBUSDIS;
  __IO uint32_t DVBUSPULSE;
  __IO uint32_t DTHRCTL;
  __IO uint32_t DIEPEMPMSK;
  __IO uint32_t DEACHINT;
  __IO uint32_t DEACHINTMSK;
       uint32_t RESERVED26[48];
  __IO uint32_t DIEPCTL0;
       uint32_t RESERVED28;
  __IO uint32_t DIEPINT0;
       uint32_t RESERVED29;
  __IO uint32_t DIEPTSIZ0;
  __IO uint32_t DIEPDMA0;
  __IO uint32_t DTXFSTS0;
       uint32_t RESERVED30;
  __IO uint32_t DIEPCTL1;
       uint32_t RESERVED31;
  __IO uint32_t DIEPINT1;
       uint32_t RESERVED32;
  __IO uint32_t DIEPTSIZ1;
  __IO uint32_t DIEPDMA1;
  __IO uint32_t DTXFSTS1;
       uint32_t RESERVED33;
  __IO uint32_t DIEPCTL2;
       uint32_t RESERVED34;
  __IO uint32_t DIEPINT2;
       uint32_t RESERVED35;
  __IO uint32_t DIEPTSIZ2;
  __IO uint32_t DIEPDMA2;
  __IO uint32_t DTXFSTS2;
       uint32_t RESERVED36;
  __IO uint32_t DIEPCTL3;
       uint32_t RESERVED37;
  __IO uint32_t DIEPINT3;
       uint32_t RESERVED38;
  __IO uint32_t DIEPTSIZ3;
  __IO uint32_t DIEPDMA3;
  __IO uint32_t DTXFSTS3;
       uint32_t RESERVED39;
  __IO uint32_t DIEPCTL4;
       uint32_t RESERVED40;
  __IO uint32_t DIEPINT4;
       uint32_t RESERVED41;
  __IO uint32_t DIEPTSIZ4;
  __IO uint32_t DIEPDMA4;
  __IO uint32_t DTXFSTS4;
       uint32_t RESERVED42;
  __IO uint32_t DIEPCTL5;
       uint32_t RESERVED43;
  __IO uint32_t DIEPINT5;
       uint32_t RESERVED44;
  __IO uint32_t DIEPTSIZ5;
  __IO uint32_t DIEPDMA5;
  __IO uint32_t DTXFSTS5;
       uint32_t RESERVED45;
  __IO uint32_t DIEPCTL6;
       uint32_t RESERVED46;
  __IO uint32_t DIEPINT6;
       uint32_t RESERVED47;
  __IO uint32_t DIEPTSIZ6;
  __IO uint32_t DIEPDMA6;
  __IO uint32_t DTXFSTS6;
       uint32_t RESERVED48;
  __IO uint32_t DIEPCTL7;
       uint32_t RESERVED49;
  __IO uint32_t DIEPINT7;
       uint32_t RESERVED50;
  __IO uint32_t DIEPTSIZ7;
  __IO uint32_t DIEPDMA7;
  __IO uint32_t DTXFSTS7;
       uint32_t RESERVED51;
  __IO uint32_t DIEPCTL8;
       uint32_t RESERVED52;
  __IO uint32_t DIEPINT8;
       uint32_t RESERVED53;
  __IO uint32_t DIEPTSIZ8;
  __IO uint32_t DIEPDMA8;
  __IO uint32_t DTXFSTS8;
       uint32_t RESERVED54;
  __IO uint32_t DIEPCTL9;
       uint32_t RESERVED55;
  __IO uint32_t DIEPINT9;
       uint32_t RESERVED56;
  __IO uint32_t DIEPTSIZ9;
  __IO uint32_t DIEPDMA9;
  __IO uint32_t DTXFSTS9;
       uint32_t RESERVED57;
  __IO uint32_t DIEPCTL10;
       uint32_t RESERVED58;
  __IO uint32_t DIEPINT10;
       uint32_t RESERVED59;
  __IO uint32_t DIEPTSIZ10;
  __IO uint32_t DIEPDMA10;
  __IO uint32_t DTXFSTS10;
       uint32_t RESERVED60;
  __IO uint32_t DIEPCTL11;
       uint32_t RESERVED61;
  __IO uint32_t DIEPINT11;
       uint32_t RESERVED62;
  __IO uint32_t DIEPTSIZ11;
  __IO uint32_t DIEPDMA11;
  __IO uint32_t DTXFSTS11;
       uint32_t RESERVED63;
  __IO uint32_t DIEPCTL12;
       uint32_t RESERVED64;
  __IO uint32_t DIEPINT12;
       uint32_t RESERVED65;
  __IO uint32_t DIEPTSIZ12;
  __IO uint32_t DIEPDMA12;
  __IO uint32_t DTXFSTS12;
       uint32_t RESERVED66;
  __IO uint32_t DIEPCTL13;
       uint32_t RESERVED67;
  __IO uint32_t DIEPINT13;
       uint32_t RESERVED68;
  __IO uint32_t DIEPTSIZ13;
  __IO uint32_t DIEPDMA13;
  __IO uint32_t DTXFSTS13;
       uint32_t RESERVED69;
  __IO uint32_t DIEPCTL14;
       uint32_t RESERVED70;
  __IO uint32_t DIEPINT14;
       uint32_t RESERVED71;
  __IO uint32_t DIEPTSIZ14;
  __IO uint32_t DIEPDMA14;
  __IO uint32_t DTXFSTS14;
       uint32_t RESERVED72;
  __IO uint32_t DIEPCTL15;
       uint32_t RESERVED73;
  __IO uint32_t DIEPINT15;
       uint32_t RESERVED74;
  __IO uint32_t DIEPTSIZ15;
  __IO uint32_t DIEPDMA15;
  __IO uint32_t DTXFSTS15;
       uint32_t RESERVED75;
  __IO uint32_t DOEPCTL0;
       uint32_t RESERVED76;
  __IO uint32_t DOEPINT0;
       uint32_t RESERVED77;
  __IO uint32_t DOEPTSIZ0;
  __IO uint32_t DOEPDMAB0;
       uint32_t RESERVED78[2];
  __IO uint32_t DOEPCTL1;
       uint32_t RESERVED79;
  __IO uint32_t DOEPINT1;
       uint32_t RESERVED80;
  __IO uint32_t DOEPTSIZ1;
  __IO uint32_t DOEPDMAB1;
       uint32_t RESERVED81[2];
  __IO uint32_t DOEPCTL2;
       uint32_t RESERVED82;
  __IO uint32_t DOEPINT2;
       uint32_t RESERVED83;
  __IO uint32_t DOEPTSIZ2;
  __IO uint32_t DOEPDMAB2;
       uint32_t RESERVED84[2];
  __IO uint32_t DOEPCTL3;
       uint32_t RESERVED85;
  __IO uint32_t DOEPINT3;
       uint32_t RESERVED86;
  __IO uint32_t DOEPTSIZ3;
  __IO uint32_t DOEPDMAB3;
       uint32_t RESERVED87[2];
  __IO uint32_t DOEPCTL4;
       uint32_t RESERVED88;
  __IO uint32_t DOEPINT4;
       uint32_t RESERVED89;
  __IO uint32_t DOEPTSIZ4;
  __IO uint32_t DOEPDMAB4;
       uint32_t RESERVED90[2];
  __IO uint32_t DOEPCTL5;
       uint32_t RESERVED91;
  __IO uint32_t DOEPINT5;
       uint32_t RESERVED92;
  __IO uint32_t DOEPTSIZ5;
  __IO uint32_t DOEPDMAB5;
       uint32_t RESERVED93[2];
  __IO uint32_t DOEPCTL6;
       uint32_t RESERVED94;
  __IO uint32_t DOEPINT6;
       uint32_t RESERVED95;
  __IO uint32_t DOEPTSIZ6;
  __IO uint32_t DOEPDMAB6;
       uint32_t RESERVED96[2];
  __IO uint32_t DOEPCTL7;
       uint32_t RESERVED97;
  __IO uint32_t DOEPINT7;
       uint32_t RESERVED98;
  __IO uint32_t DOEPTSIZ7;
  __IO uint32_t DOEPDMAB7;
       uint32_t RESERVED99[2];
  __IO uint32_t DOEPCTL8;
       uint32_t RESERVED100;
  __IO uint32_t DOEPINT8;
       uint32_t RESERVED101;
  __IO uint32_t DOEPTSIZ8;
  __IO uint32_t DOEPDMAB8;
       uint32_t RESERVED102[2];
  __IO uint32_t DOEPCTL9;
       uint32_t RESERVED103;
  __IO uint32_t DOEPINT9;
       uint32_t RESERVED104;
  __IO uint32_t DOEPTSIZ9;
  __IO uint32_t DOEPDMAB9;
       uint32_t RESERVED105[2];
  __IO uint32_t DOEPCTL10;
       uint32_t RESERVED106;
  __IO uint32_t DOEPINT10;
       uint32_t RESERVED107;
  __IO uint32_t DOEPTSIZ10;
  __IO uint32_t DOEPDMAB10;
       uint32_t RESERVED108[2];
  __IO uint32_t DOEPCTL11;
       uint32_t RESERVED109;
  __IO uint32_t DOEPINT11;
       uint32_t RESERVED110;
  __IO uint32_t DOEPTSIZ11;
  __IO uint32_t DOEPDMAB11;
       uint32_t RESERVED111[2];
  __IO uint32_t DOEPCTL12;
       uint32_t RESERVED112;
  __IO uint32_t DOEPINT12;
       uint32_t RESERVED113;
  __IO uint32_t DOEPTSIZ12;
  __IO uint32_t DOEPDMAB12;
       uint32_t RESERVED114[2];
  __IO uint32_t DOEPCTL13;
       uint32_t RESERVED115;
  __IO uint32_t DOEPINT13;
       uint32_t RESERVED116;
  __IO uint32_t DOEPTSIZ13;
  __IO uint32_t DOEPDMAB13;
       uint32_t RESERVED117[2];
  __IO uint32_t DOEPCTL14;
       uint32_t RESERVED118;
  __IO uint32_t DOEPINT14;
       uint32_t RESERVED119;
  __IO uint32_t DOEPTSIZ14;
  __IO uint32_t DOEPDMAB14;
       uint32_t RESERVED120[2];
  __IO uint32_t DOEPCTL15;
       uint32_t RESERVED121;
  __IO uint32_t DOEPINT15;
       uint32_t RESERVED122;
  __IO uint32_t DOEPTSIZ15;
  __IO uint32_t DOEPDMAB15;
       uint32_t RESERVED123[2];
       uint32_t RESERVED143[64];
  __IO uint32_t PCGCCTL;
} OTG_HS_TypeDef;

#define OTG_HS_BASE           (AHB1PERIPH_BASE + 0x20000U)
#define OTG_HS_DFIFO0_BASE    (OTG_HS_BASE + 0x01000U)
#define OTG_HS_DFIFO1_BASE    (OTG_HS_BASE + 0x02000U)
#define OTG_HS_DFIFO2_BASE    (OTG_HS_BASE + 0x03000U)
#define OTG_HS_DFIFO3_BASE    (OTG_HS_BASE + 0x04000U)
#define OTG_HS_DFIFO4_BASE    (OTG_HS_BASE + 0x05000U)
#define OTG_HS_DFIFO5_BASE    (OTG_HS_BASE + 0x06000U)
#define OTG_HS_DFIFO6_BASE    (OTG_HS_BASE + 0x07000U)
#define OTG_HS_DFIFO7_BASE    (OTG_HS_BASE + 0x08000U)
#define OTG_HS_DFIFO8_BASE    (OTG_HS_BASE + 0x09000U)
#define OTG_HS_DFIFO9_BASE    (OTG_HS_BASE + 0x0A000U)
#define OTG_HS_DFIFO10_BASE   (OTG_HS_BASE + 0x0B000U)
#define OTG_HS_DFIFO11_BASE   (OTG_HS_BASE + 0x0C000U)
#define OTG_HS_DFIFO12_BASE   (OTG_HS_BASE + 0x0D000U)
#define OTG_HS_DFIFO13_BASE   (OTG_HS_BASE + 0x0E000U)
#define OTG_HS_DFIFO14_BASE   (OTG_HS_BASE + 0x0F000U)
#define OTG_HS_DFIFO15_BASE   (OTG_HS_BASE + 0x10000U)

#define OTG_HS                ((OTG_HS_TypeDef *) OTG_HS_BASE)
#define OTG_HS_DFIFO0         (((uint32_t *) OTG_HS_DFIFO0_BASE) )
#define OTG_HS_DFIFO1         (((uint32_t *) OTG_HS_DFIFO1_BASE) )
#define OTG_HS_DFIFO2         (((uint32_t *) OTG_HS_DFIFO2_BASE) )
#define OTG_HS_DFIFO3         (((uint32_t *) OTG_HS_DFIFO3_BASE) )
#define OTG_HS_DFIFO4         (((uint32_t *) OTG_HS_DFIFO4_BASE) )
#define OTG_HS_DFIFO5         (((uint32_t *) OTG_HS_DFIFO5_BASE) )
#define OTG_HS_DFIFO6         (((uint32_t *) OTG_HS_DFIFO6_BASE) )
#define OTG_HS_DFIFO7         (((uint32_t *) OTG_HS_DFIFO7_BASE) )
#define OTG_HS_DFIFO8         (((uint32_t *) OTG_HS_DFIFO8_BASE) )
#define OTG_HS_DFIFO9         (((uint32_t *) OTG_HS_DFIFO9_BASE) )
#define OTG_HS_DFIFO10        (((uint32_t *) OTG_HS_DFIFO10_BASE))
#define OTG_HS_DFIFO11        (((uint32_t *) OTG_HS_DFIFO11_BASE))
#define OTG_HS_DFIFO12        (((uint32_t *) OTG_HS_DFIFO12_BASE))
#define OTG_HS_DFIFO13        (((uint32_t *) OTG_HS_DFIFO13_BASE))
#define OTG_HS_DFIFO14        (((uint32_t *) OTG_HS_DFIFO14_BASE))
#define OTG_HS_DFIFO15        (((uint32_t *) OTG_HS_DFIFO15_BASE))

// OTG_HS Host Channel
typedef struct {                        // Host Channel typedef (HC)
  union {
    volatile uint32_t HCCHAR;           // Channel Characteristics
    struct {
               uint32_t MPSIZ     : 11; // Endpoint Maximum Packet Size
               uint32_t EPNUM     :  4; // Endpoint Number
               uint32_t EPDIR     :  1; // Endpoint Direction
               uint32_t Reserved0 :  1; // Reserved
               uint32_t LSDEV     :  1; // Endpoint Low-speed
               uint32_t EPTYP     :  2; // Endpoint Type
               uint32_t MCNT      :  2; // Multicount
               uint32_t DAD       :  7; // Device Address
               uint32_t ODDFRM    :  1; // Periodic Transaction Odd Frame
               uint32_t CHDIS     :  1; // Channel Disable
      volatile uint32_t CHENA     :  1; // Channel Enable
    };
  };
  union {
    uint32_t HCSPLT;                    // Split Control
    struct {
      uint32_t PRTADDR   :  7;          // Port Address
      uint32_t HUBADDR   :  7;          // HUB Address
      uint32_t XACTPOS   :  2;          // Transaction Position
      uint32_t COMPLSPLT :  1;          // Do Complete Split
      uint32_t Reserved1 : 14;          // Reserved
      uint32_t SPLITEN   :  1;          // Split Enable
    };
  };
  union {
    volatile uint32_t HCINT;            // Channel Interrupt
    struct {
      volatile uint32_t XFCR      :  1; // Transfer Completed
      volatile uint32_t CHH       :  1; // Channel Halted
      volatile uint32_t AHBERR    :  1; // AHB Error
      volatile uint32_t STALL     :  1; // STALL Response Received Interrupt
      volatile uint32_t NAK       :  1; // NAK Response Received Interrupt
      volatile uint32_t ACK       :  1; // ACK Response Rece/Transmit Int
      volatile uint32_t NYET      :  1; // NYET Response Received Interrupt
      volatile uint32_t TXERR     :  1; // Transaction Error
      volatile uint32_t BBERR     :  1; // Babble Error
      volatile uint32_t FRMOR     :  1; // Frame Overrun
      volatile uint32_t DTERR     :  1; // Data Toggle Error
               uint32_t Reserved4 : 21; 
    };
  };
  union {
    uint32_t HCINTMSK;                  // Channel Interrupt Mask
    struct {
      uint32_t XFCRM     :  1;          // Transfer Completed Mask
      uint32_t CHHM      :  1;          // Channel Halted Mask
      uint32_t AHBERRM   :  1;          // AHB Error
      uint32_t STALLM    :  1;          // STALL Response Rece Interrupt Mask
      uint32_t NAKM      :  1;          // NAK Response Rece Interrupt Mask
      uint32_t ACKM      :  1;          // ACK Response Rece/Transmit Int Mask
      uint32_t NYETM     :  1;          // NYET Response Rece/Transmit Int Msk
      uint32_t TXERRM    :  1;          // Transaction Error Mask
      uint32_t BBERRM    :  1;          // Babble Error Mask
      uint32_t FRMORM    :  1;          // Frame Overrun Mask
      uint32_t DTERRM    :  1;          // Data Toggle Error Mask
      uint32_t Reserved7 : 21; 
    };
  };
  union {
    volatile uint32_t HCTSIZ;           // Channel Transfer Size
    struct {
               uint32_t XFRSIZ    : 19; // Transfer Size
      volatile uint32_t PKTCNT    : 10; // Packet Count
               uint32_t DPID      :  2; // Data PID
               uint32_t DOPING    :  1; // Do Ping
    };
  };
  uint32_t DMAADDR;                     // DMA Address
  uint32_t Reserved10;                  // Reserved
  uint32_t Reserved11;                  // Reserved
} OTG_HS_HC;

#if   (defined (__CC_ARM))
  #pragma pop
#elif (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
  #pragma clang diagnostic pop
#elif (defined (__GNUC__))
  #pragma GCC diagnostic pop
#endif

// OTG_HS control and status register (OTG_HS_GOTGCTL)
#define  OTG_HS_GOTGCTL_SRQSCS              ((uint32_t)    1U       )
#define  OTG_HS_GOTGCTL_SRQ                 ((uint32_t)    1U  <<  1)
#define  OTG_HS_GOTGCTL_HNGSCS              ((uint32_t)    1U  <<  8)
#define  OTG_HS_GOTGCTL_HNPRQ               ((uint32_t)    1U  <<  9)
#define  OTG_HS_GOTGCTL_HSHNPEN             ((uint32_t)    1U  << 10)
#define  OTG_HS_GOTGCTL_DHNPEN              ((uint32_t)    1U  << 11)
#define  OTG_HS_GOTGCTL_CIDSTS              ((uint32_t)    1U  << 16)
#define  OTG_HS_GOTGCTL_DBCT                ((uint32_t)    1U  << 17)
#define  OTG_HS_GOTGCTL_ASVLD               ((uint32_t)    1U  << 18)
#define  OTG_HS_GOTGCTL_BSVLD               ((uint32_t)    1U  << 19)

// OTG_HS interrupt register (OTG_HS_GOTGINT)
#define  OTG_HS_GOTGINT_SEDET               ((uint32_t)    1U  <<  2)
#define  OTG_HS_GOTGINT_SRSSCHG             ((uint32_t)    1U  <<  8)
#define  OTG_HS_GOTGINT_HNSSCHG             ((uint32_t)    1U  <<  9)
#define  OTG_HS_GOTGINT_HNGDET              ((uint32_t)    1U  << 17)
#define  OTG_HS_GOTGINT_ADTOCHG             ((uint32_t)    1U  << 18)
#define  OTG_HS_GOTGINT_DBCDNE              ((uint32_t)    1U  << 19)

// OTG_HS AHB configuration register (OTG_HS_GAHBCFG)
#define  OTG_HS_GAHBCFG_GINT                ((uint32_t)    1U       )
#define  OTG_HS_GAHBCFG_HBSTLEN_POS         ((uint32_t)           1U)
#define  OTG_HS_GAHBCFG_HBSTLEN_MSK         ((uint32_t) 0x0FU  <<  OTG_HS_GAHBCFG_HBSTLEN_POS)
#define  OTG_HS_GAHBCFG_HBSTLEN(x)         (((uint32_t)    x   <<  OTG_HS_GAHBCFG_HBSTLEN_POS) & OTG_HS_GAHBCFG_HBSTLEN_MSK)
#define  OTG_HS_GAHBCFG_DMAEN               ((uint32_t)    1U  <<  5)
#define  OTG_HS_GAHBCFG_TXFELVL             ((uint32_t)    1U  <<  7)
#define  OTG_HS_GAHBCFG_PTXFELVL            ((uint32_t)    1U  <<  8)

// OTG_HS USB configuration register (OTG_HS_GUSBCFG)
#define  OTG_HS_GUSBCFG_TOCAL_POS           ((uint32_t)           0U)
#define  OTG_HS_GUSBCFG_TOCAL_MSK           ((uint32_t)    7U       )
#define  OTG_HS_GUSBCFG_TOCAL(x)           (((uint32_t)    x        ) & OTG_HS_GUSBCFG_TOCAL_MSK)
#define  OTG_HS_GUSBCFG_PHYSEL              ((uint32_t)    1U  <<  6)
#define  OTG_HS_GUSBCFG_SRPCAP              ((uint32_t)    1U  <<  8)
#define  OTG_HS_GUSBCFG_HNPCAP              ((uint32_t)    1U  <<  9)
#define  OTG_HS_GUSBCFG_TRDT_POS            ((uint32_t)           10U)
#define  OTG_HS_GUSBCFG_TRDT_MSK            ((uint32_t) 0x0FU  << OTG_HS_GUSBCFG_TRDT_POS)
#define  OTG_HS_GUSBCFG_TRDT(x)            (((uint32_t)    x   << OTG_HS_GUSBCFG_TRDT_POS) & OTG_HS_GUSBCFG_TRDT_MSK)
#define  OTG_HS_GUSBCFG_PHYLPCS             ((uint32_t)    1U  << 15)
#define  OTG_HS_GUSBCFG_ULPIFSLS            ((uint32_t)    1U  << 17)
#define  OTG_HS_GUSBCFG_ULPIAR              ((uint32_t)    1U  << 18)
#define  OTG_HS_GUSBCFG_ULPICSM             ((uint32_t)    1U  << 19)
#define  OTG_HS_GUSBCFG_ULPIEVBUSD          ((uint32_t)    1U  << 20)
#define  OTG_HS_GUSBCFG_ULPIEVBUSI          ((uint32_t)    1U  << 21)
#define  OTG_HS_GUSBCFG_TSDPS               ((uint32_t)    1U  << 22)
#define  OTG_HS_GUSBCFG_PCCI                ((uint32_t)    1U  << 23)
#define  OTG_HS_GUSBCFG_PTCI                ((uint32_t)    1U  << 24)
#define  OTG_HS_GUSBCFG_ULPIPD              ((uint32_t)    1U  << 25)
#define  OTG_HS_GUSBCFG_FHMOD               ((uint32_t)    1U  << 29)
#define  OTG_HS_GUSBCFG_FDMOD               ((uint32_t)    1U  << 30)
#define  OTG_HS_GUSBCFG_CTXPKT              ((uint32_t)    1U  << 31)

// OTG_HS reset register (OTG_HS_GRSTCTL)
#define  OTG_HS_GRSTCTL_CSRST               ((uint32_t)    1U       )
#define  OTG_HS_GRSTCTL_HSRST               ((uint32_t)    1U  <<  1)
#define  OTG_HS_GRSTCTL_FCRST               ((uint32_t)    1U  <<  2)
#define  OTG_HS_GRSTCTL_RXFFLSH             ((uint32_t)    1U  <<  4)
#define  OTG_HS_GRSTCTL_TXFFLSH             ((uint32_t)    1U  <<  5)
#define  OTG_HS_GRSTCTL_TXFNUM_POS          ((uint32_t)           6U)
#define  OTG_HS_GRSTCTL_TXFNUM_MSK          ((uint32_t) 0x1FU  <<  OTG_HS_GRSTCTL_TXFNUM_POS)
#define  OTG_HS_GRSTCTL_TXFNUM(x)          (((uint32_t)    x   <<  OTG_HS_GRSTCTL_TXFNUM_POS) & OTG_HS_GRSTCTL_TXFNUM_MSK)
#define  OTG_HS_GRSTCTL_DMAREQ              ((uint32_t)    1U  << 30)
#define  OTG_HS_GRSTCTL_AHBIDL              ((uint32_t)    1U  << 31)

// OTG_HS core interrupt register (OTG_HS_GINTSTS)
#define  OTG_HS_GINTSTS_CMOD                ((uint32_t)    1U       )
#define  OTG_HS_GINTSTS_MMIS                ((uint32_t)    1U  <<  1)
#define  OTG_HS_GINTSTS_OTGINT              ((uint32_t)    1U  <<  2)
#define  OTG_HS_GINTSTS_SOF                 ((uint32_t)    1U  <<  3)
#define  OTG_HS_GINTSTS_RXFLVL              ((uint32_t)    1U  <<  4)
#define  OTG_HS_GINTSTS_NPTXFE              ((uint32_t)    1U  <<  5)
#define  OTG_HS_GINTSTS_GINAKEFF            ((uint32_t)    1U  <<  6)
#define  OTG_HS_GINTSTS_GONAKEFF            ((uint32_t)    1U  <<  7)
#define  OTG_HS_GINTSTS_ESUSP               ((uint32_t)    1U  << 10)
#define  OTG_HS_GINTSTS_USBSUSP             ((uint32_t)    1U  << 11)
#define  OTG_HS_GINTSTS_USBRST              ((uint32_t)    1U  << 12)
#define  OTG_HS_GINTSTS_ENUMDNE             ((uint32_t)    1U  << 13)
#define  OTG_HS_GINTSTS_ISOODRP             ((uint32_t)    1U  << 14)
#define  OTG_HS_GINTSTS_EOPF                ((uint32_t)    1U  << 15)
#define  OTG_HS_GINTSTS_IEPINT              ((uint32_t)    1U  << 18)
#define  OTG_HS_GINTSTS_OEPINT              ((uint32_t)    1U  << 19)
#define  OTG_HS_GINTSTS_IISOIXFR            ((uint32_t)    1U  << 20)
#define  OTG_HS_GINTSTS_IPXFR               ((uint32_t)    1U  << 21)
#define  OTG_HS_GINTSTS_INCOMPISOOUT        ((uint32_t)    1U  << 21)
#define  OTG_HS_GINTSTS_DATAFSUSP           ((uint32_t)    1U  << 22)
#define  OTG_HS_GINTSTS_HPRTINT             ((uint32_t)    1U  << 24)
#define  OTG_HS_GINTSTS_HCINT               ((uint32_t)    1U  << 25)
#define  OTG_HS_GINTSTS_PTXFE               ((uint32_t)    1U  << 26)
#define  OTG_HS_GINTSTS_CIDSCHG             ((uint32_t)    1U  << 28)
#define  OTG_HS_GINTSTS_DISCINT             ((uint32_t)    1U  << 29)
#define  OTG_HS_GINTSTS_SRQINT              ((uint32_t)    1U  << 30)
#define  OTG_HS_GINTSTS_WKUPINT             ((uint32_t)    1U  << 31)

// OTG_HS interrupt mask register (OTG_HS_GINTMSK)
#define  OTG_HS_GINTMSK_MMISM               ((uint32_t)    1U  <<  1)
#define  OTG_HS_GINTMSK_OTGINT              ((uint32_t)    1U  <<  2)
#define  OTG_HS_GINTMSK_SOFM                ((uint32_t)    1U  <<  3)
#define  OTG_HS_GINTMSK_RXFLVLM             ((uint32_t)    1U  <<  4)
#define  OTG_HS_GINTMSK_NPTXFEM             ((uint32_t)    1U  <<  5)
#define  OTG_HS_GINTMSK_GINAKEFFM           ((uint32_t)    1U  <<  6)
#define  OTG_HS_GINTMSK_GONAKEFFM           ((uint32_t)    1U  <<  7)
#define  OTG_HS_GINTMSK_ESUSPM              ((uint32_t)    1U  << 10)
#define  OTG_HS_GINTMSK_USBSUSPM            ((uint32_t)    1U  << 11)
#define  OTG_HS_GINTMSK_USBRST              ((uint32_t)    1U  << 12)
#define  OTG_HS_GINTMSK_ENUMDNEM            ((uint32_t)    1U  << 13)
#define  OTG_HS_GINTMSK_ISOODRPM            ((uint32_t)    1U  << 14)
#define  OTG_HS_GINTMSK_EOPFM               ((uint32_t)    1U  << 15)
#define  OTG_HS_GINTMSK_EPMISM              ((uint32_t)    1U  << 17)
#define  OTG_HS_GINTMSK_IEPINT              ((uint32_t)    1U  << 18)
#define  OTG_HS_GINTMSK_OEPINT              ((uint32_t)    1U  << 19)
#define  OTG_HS_GINTMSK_IISOIXFRM           ((uint32_t)    1U  << 20)
#define  OTG_HS_GINTMSK_IPXFRM              ((uint32_t)    1U  << 21)
#define  OTG_HS_GINTMSK_IISOOXFRM           ((uint32_t)    1U  << 21)
#define  OTG_HS_GINTMSK_FSUSPM              ((uint32_t)    1U  << 22)
#define  OTG_HS_GINTMSK_PRTIM               ((uint32_t)    1U  << 24)
#define  OTG_HS_GINTMSK_HCIM                ((uint32_t)    1U  << 25)
#define  OTG_HS_GINTMSK_PTXFEM              ((uint32_t)    1U  << 26)
#define  OTG_HS_GINTMSK_CIDSCHGM            ((uint32_t)    1U  << 28)
#define  OTG_HS_GINTMSK_DISCINT             ((uint32_t)    1U  << 29)
#define  OTG_HS_GINTMSK_SRQIM               ((uint32_t)    1U  << 30)
#define  OTG_HS_GINTMSK_WUIM                ((uint32_t)    1U  << 31)

// OTG_HS status debug read register (OTG_HS_GRXSTSR)
#define  OTG_HS_GRXSTSR_CHNUM_POS           ((uint32_t)           0U)
#define  OTG_HS_GRXSTSR_CHNUM_MSK           ((uint32_t) 0x0FU       )
#define  OTG_HS_GRXSTSR_CHNUM(x)           (((uint32_t)    x        ) & OTG_HS_GRXSTSR_CHNUM_MSK)
#define  OTG_HS_GRXSTSR_EPNUM_POS           ((uint32_t)           0U)
#define  OTG_HS_GRXSTSR_EPNUM_MSK           ((uint32_t) 0x0FU       )
#define  OTG_HS_GRXSTSR_EPNUM(x)           (((uint32_t)    x        ) & OTG_HS_GRXSTSR_EPNUM_MSK)
#define  OTG_HS_GRXSTSR_BCNT_POS            ((uint32_t)           4U)
#define  OTG_HS_GRXSTSR_BCNT_MSK            ((uint32_t)0x7FFU  << OTG_HS_GRXSTSR_BCNT_POS)
#define  OTG_HS_GRXSTSR_BCNT(x)            (((uint32_t)    x   << OTG_HS_GRXSTSR_BCNT_POS) & OTG_HS_GRXSTSR_BCNT_MSK)
#define  OTG_HS_GRXSTSR_DPID_POS            ((uint32_t)           15U)
#define  OTG_HS_GRXSTSR_DPID_MSK            ((uint32_t)    3U  << OTG_HS_GRXSTSR_DPID_POS)
#define  OTG_HS_GRXSTSR_DPID(x)            (((uint32_t)    x   << OTG_HS_GRXSTSR_DPID_POS) & OTG_HS_GRXSTSR_DPID_MSK)
#define  OTG_HS_GRXSTSR_PKTSTS_POS          ((uint32_t)           17U)
#define  OTG_HS_GRXSTSR_PKTSTS_MSK          ((uint32_t) 0x0FU  << OTG_HS_GRXSTSR_PKTSTS_POS)
#define  OTG_HS_GRXSTSR_PKTSTS(x)          (((uint32_t)    x   << OTG_HS_GRXSTSR_PKTSTS_POS) & OTG_HS_GRXSTSR_PKTSTS_MSK)
#define  OTG_HS_GRXSTSR_FRMNUM_POS          ((uint32_t)           21U)
#define  OTG_HS_GRXSTSR_FRMNUM_MSK          ((uint32_t) 0x0FU  << OTG_HS_GRXSTSR_FRMNUM_POS)
#define  OTG_HS_GRXSTSR_FRMNUM(x)          (((uint32_t)    x   << OTG_HS_GRXSTSR_FRMNUM_POS) & OTG_HS_GRXSTSR_FRMNUM_MSK)

// OTG_HS status read and pop register (OTG_HS_GRXSTSP)
#define  OTG_HS_GRXSTSP_CHNUM_POS           ((uint32_t)           0U)
#define  OTG_HS_GRXSTSP_CHNUM_MSK           ((uint32_t) 0x0FU       )
#define  OTG_HS_GRXSTSP_CHNUM(x)           (((uint32_t)    x        ) & OTG_HS_GRXSTSP_CHNUM_MSK)
#define  OTG_HS_GRXSTSP_EPNUM_POS           ((uint32_t)           0U)
#define  OTG_HS_GRXSTSP_EPNUM_MSK           ((uint32_t) 0x0FU       )
#define  OTG_HS_GRXSTSP_EPNUM(x)           (((uint32_t)    x        ) & OTG_HS_GRXSTSP_EPNUM_MSK)
#define  OTG_HS_GRXSTSP_BCNT_POS            ((uint32_t)           4U)
#define  OTG_HS_GRXSTSP_BCNT_MSK            ((uint32_t)0x7FFU  << OTG_HS_GRXSTSP_BCNT_POS)
#define  OTG_HS_GRXSTSP_BCNT(x)            (((uint32_t)    x   << OTG_HS_GRXSTSP_BCNT_POS) & OTG_HS_GRXSTSP_BCNT_MSK)
#define  OTG_HS_GRXSTSP_DPID_POS            ((uint32_t)           15U)
#define  OTG_HS_GRXSTSP_DPID_MSK            ((uint32_t)    3U  << OTG_HS_GRXSTSP_DPID_POS)
#define  OTG_HS_GRXSTSP_DPID(x)            (((uint32_t)    x   << OTG_HS_GRXSTSP_DPID_POS) & OTG_HS_GRXSTSP_DPID_MSK)
#define  OTG_HS_GRXSTSP_PKTSTS_POS          ((uint32_t)           17U)
#define  OTG_HS_GRXSTSP_PKTSTS_MSK          ((uint32_t) 0x0FU  << OTG_HS_GRXSTSP_PKTSTS_POS)
#define  OTG_HS_GRXSTSP_PKTSTS(x)          (((uint32_t)    x   << OTG_HS_GRXSTSP_PKTSTS_POS) & OTG_HS_GRXSTSP_PKTSTS_MSK)
#define  OTG_HS_GRXSTSP_FRMNUM_POS          ((uint32_t)           21U)
#define  OTG_HS_GRXSTSP_FRMNUM_MSK          ((uint32_t) 0x0FU  << OTG_HS_GRXSTSP_FRMNUM_POS)
#define  OTG_HS_GRXSTSP_FRMNUM(x)          (((uint32_t)    x   << OTG_HS_GRXSTSP_FRMNUM_POS) & OTG_HS_GRXSTSP_FRMNUM_MSK)

// OTG_HS Receive FIFO size register (OTG_HS_GRXFSIZ)
#define  OTG_HS_GRXFSIZ_RXFD_POS            ((uint32_t)            0U)
#define  OTG_HS_GRXFSIZ_RXFD_MSK            ((uint32_t)0xFFFFU      )
#define  OTG_HS_GRXFSIZ_RXFD(x)            (((uint32_t)    x        ) & OTG_HS_GRXFSIZ_RXFD_MSK)

// OTG_HS Host non-periodic transmit FIFO size register (OTG_HS_HNPTXFSIZ)
#define  OTG_HS_HNPTXFSIZ_NPTXFSA_POS       ((uint32_t)           0U)
#define  OTG_HS_HNPTXFSIZ_NPTXFSA_MSK       ((uint32_t)0xFFFFU      )
#define  OTG_HS_HNPTXFSIZ_NPTXFSA(x)       (((uint32_t)    x        ) & OTG_HS_HNPTXFSIZ_NPTXFSA_MSK)
#define  OTG_HS_HNPTXFSIZ_NPTXFD_POS        ((uint32_t)           16U)
#define  OTG_HS_HNPTXFSIZ_NPTXFD_MSK        ((uint32_t)0xFFFFU << OTG_HS_HNPTXFSIZ_NPTXFD_POS)
#define  OTG_HS_HNPTXFSIZ_NPTXFD(x)        (((uint32_t)    x   << OTG_HS_HNPTXFSIZ_NPTXFD_POS) & OTG_HS_HNPTXFSIZ_NPTXFD_MSK)

// OTG_HS Endpoint 0 transmit FIFO size register (OTG_HS_DIEPTXF0)
#define  OTG_HS_DIEPTXF0_TX0FSA_POS         ((uint32_t)           0U)
#define  OTG_HS_DIEPTXF0_TX0FSA_MSK         ((uint32_t)0xFFFFU      )
#define  OTG_HS_DIEPTXF0_TX0FSA(x)         (((uint32_t)    x        ) & OTG_HS_DIEPTXF0_TX0FSA_MSK)
#define  OTG_HS_DIEPTXF0_TX0FD_POS          ((uint32_t)           16U)
#define  OTG_HS_DIEPTXF0_TX0FD_MSK          ((uint32_t)0xFFFFU << OTG_HS_DIEPTXF0_TX0FD_POS)
#define  OTG_HS_DIEPTXF0_TX0FD(x)          (((uint32_t)    x   << OTG_HS_DIEPTXF0_TX0FD_POS) & OTG_HS_DIEPTXF0_TX0FD_MSK)

// OTG_HS Host non-periodic transmit FIFO/queue status register (OTG_HS_HNPTXSTS)
#define  OTG_HS_HNPTXSTS_NPTXFSAV_POS       ((uint32_t)           0U)
#define  OTG_HS_HNPTXSTS_NPTXFSAV_MSK       ((uint32_t)0xFFFFU      )
#define  OTG_HS_HNPTXSTS_NPTXFSAV(x)       (((uint32_t)    x        ) & OTG_HS_HNPTXSTS_NPTXFSAV_MSK)
#define  OTG_HS_HNPTXSTS_NPTQXSAV_POS       ((uint32_t)           16U)
#define  OTG_HS_HNPTXSTS_NPTQXSAV_MSK       ((uint32_t) 0xFFU  << OTG_HS_HNPTXSTS_NPTQXSAV_POS)
#define  OTG_HS_HNPTXSTS_NPTQXSAV(x)       (((uint32_t)    x   << OTG_HS_HNPTXSTS_NPTQXSAV_POS) & OTG_HS_HNPTXSTS_NPTQXSAV_MSK)
#define  OTG_HS_HNPTXSTS_NPTXQTOP_POS       ((uint32_t)           24U)
#define  OTG_HS_HNPTXSTS_NPTXQTOP_MSK       ((uint32_t) 0x7FU  << OTG_HS_HNPTXSTS_NPTXQTOP_POS)
#define  OTG_HS_HNPTXSTS_NPTXQTOP(x)       (((uint32_t)    x   << OTG_HS_HNPTXSTS_NPTXQTOP_POS) & OTG_HS_HNPTXSTS_NPTXQTOP_MSK)

// OTG_HS I2C access register (OTG_HS_GI2CCTL)
#define  OTG_HS_GI2CCTL_RWDATA_POS          ((uint32_t)           0U)
#define  OTG_HS_GI2CCTL_RWDATA_MSK          ((uint32_t) 0xFFU       )
#define  OTG_HS_GI2CCTL_RWDATA(x)          (((uint32_t)    x        ) & OTG_HS_GI2CCTL_RWDATA_MSK)
#define  OTG_HS_GI2CCTL_REGADDR_POS         ((uint32_t)           8U)
#define  OTG_HS_GI2CCTL_REGADDR_MSK         ((uint32_t) 0xFFU  << OTG_HS_GI2CCTL_REGADDR_POS)
#define  OTG_HS_GI2CCTL_REGADDR(x)         (((uint32_t)    x   << OTG_HS_GI2CCTL_REGADDR_POS) & OTG_HS_GI2CCTL_REGADDR_MSK)
#define  OTG_HS_GI2CCTL_ADDR_POS            ((uint32_t)           16U)
#define  OTG_HS_GI2CCTL_ADDR_MSK            ((uint32_t) 0x7FU  << OTG_HS_GI2CCTL_ADDR_POS)
#define  OTG_HS_GI2CCTL_ADDR(x)            (((uint32_t)    x   << OTG_HS_GI2CCTL_ADDR_POS) & OTG_HS_GI2CCTL_ADDR_MSK)
#define  OTG_HS_GI2CCTL_I2CEN               ((uint32_t)    1U  << 23)
#define  OTG_HS_GI2CCTL_ACK                 ((uint32_t)    1U  << 24)
#define  OTG_HS_GI2CCTL_I2CDEVADR_POS       ((uint32_t)           26U)
#define  OTG_HS_GI2CCTL_I2CDEVADR_MSK       ((uint32_t)    3U  << OTG_HS_GI2CCTL_I2CDEVADR_POS)
#define  OTG_HS_GI2CCTL_I2CDEVADR(x)       (((uint32_t)    x   << OTG_HS_GI2CCTL_I2CDEVADR_POS) & OTG_HS_GI2CCTL_I2CDEVADR_MSK)
#define  OTG_HS_GI2CCTL_I2CDATSE0           ((uint32_t)    1U  << 28)
#define  OTG_HS_GI2CCTL_RW                  ((uint32_t)    1U  << 30)
#define  OTG_HS_GI2CCTL_BSYDNE              ((uint32_t)    1U  << 31)

// OTG_HS general core configuration register (OTG_HS_GCCFG)
#define  OTG_HS_GCCFG_PWRDWN                ((uint32_t)    1U  << 16)
#define  OTG_HS_GCCFG_I2CPADEN              ((uint32_t)    1U  << 17)
#define  OTG_HS_GCCFG_VBUSASEN              ((uint32_t)    1U  << 18)
#define  OTG_HS_GCCFG_VBUSBSEN              ((uint32_t)    1U  << 19)
#define  OTG_HS_GCCFG_SOFOUTEN              ((uint32_t)    1U  << 20)
#define  OTG_HS_GCCFG_NOVBUSSENS            ((uint32_t)    1U  << 21)

// OTG_HS Host periodic transmit FIFO size register (OTG_HS_HPTXFSIZ)
#define  OTG_HS_HPTXFSIZ_PTXSA_POS          ((uint32_t)           0U)
#define  OTG_HS_HPTXFSIZ_PTXSA_MSK          ((uint32_t)0xFFFFU      )
#define  OTG_HS_HPTXFSIZ_PTXSA(x)          (((uint32_t)    x        ) & OTG_HS_HPTXFSIZ_PTXSA_MSK)
#define  OTG_HS_HPTXFSIZ_PTXFD_POS          ((uint32_t)           16U)
#define  OTG_HS_HPTXFSIZ_PTXFD_MSK          ((uint32_t)0xFFFFU << OTG_HS_HPTXFSIZ_PTXFD_POS)
#define  OTG_HS_HPTXFSIZ_PTXFD(x)          (((uint32_t)    x   << OTG_HS_HPTXFSIZ_PTXFD_POS) & OTG_HS_HPTXFSIZ_PTXFD_MSK)

// OTG_HS device IN endpoint transmit FIFO size register (OTG_HS_DIEPTXFx)
#define  OTG_HS_DIEPTXFx_INEPTXSA_POS       ((uint32_t)           0U)
#define  OTG_HS_DIEPTXFx_INEPTXSA_MSK       ((uint32_t)0xFFFFU      )
#define  OTG_HS_DIEPTXFx_INEPTXSA(x)       (((uint32_t)    x        ) & OTG_HS_DIEPTXFx_INEPTXSA_MSK)
#define  OTG_HS_DIEPTXFx_INEPTXFD_POS       ((uint32_t)           16U)
#define  OTG_HS_DIEPTXFx_INEPTXFD_MSK       ((uint32_t)0xFFFFU << OTG_HS_DIEPTXFx_INEPTXFD_POS)
#define  OTG_HS_DIEPTXFx_INEPTXFD(x)       (((uint32_t)    x   << OTG_HS_DIEPTXFx_INEPTXFD_POS) & OTG_HS_DIEPTXFx_INEPTXFD_MSK)

// OTG_HS Host configuration register (OTG_HS_HCFG)
#define  OTG_HS_HCFG_FSLSPCS_POS            ((uint32_t)           0U)
#define  OTG_HS_HCFG_FSLSPCS_MSK            ((uint32_t)    3U       )
#define  OTG_HS_HCFG_FSLSPCS(x)            (((uint32_t)    x        ) & OTG_HS_HCFG_FSLSPCS_MSK)
#define  OTG_HS_HCFG_FSLSS                  ((uint32_t)    1U  <<  2)

// OTG_HS Host frame interval register (OTG_HS_HFIR)
#define  OTG_HS_HFIR_FRIVL_POS              ((uint32_t)           0U)
#define  OTG_HS_HFIR_FRIVL_MSK              ((uint32_t)0xFFFFU      )
#define  OTG_HS_HFIR_FRIVL(x)              (((uint32_t)    x        ) & OTG_HS_HFIR_FRIVL_MSK)

// OTG_HS Host frame number/frame time remaining register (OTG_HS_HFNUM)
#define  OTG_HS_HFNUM_FRNUM_POS             ((uint32_t)           0U)
#define  OTG_HS_HFNUM_FRNUM_MSK             ((uint32_t)0xFFFFU      )
#define  OTG_HS_HFNUM_FRNUM(x)             (((uint32_t)    x        ) & OTG_HS_HFNUM_FRNUM_MSK)
#define  OTG_HS_HFNUM_FTREM_POS             ((uint32_t)           16U)
#define  OTG_HS_HFNUM_FTREM_MSK             ((uint32_t)0xFFFFU << OTG_HS_HFNUM_FRNUM_POS)
#define  OTG_HS_HFNUM_FTREM(x)             (((uint32_t)    x   << OTG_HS_HFNUM_FRNUM_POS) & OTG_HS_HFNUM_FRNUM_MSK)

// OTG_HS Host periodic transmit FIFO/queue status register (OTG_HS_HPTXSTS)
#define  OTG_HS_HPTXSTS_PTXFSAVL_POS        ((uint32_t)           0U)
#define  OTG_HS_HPTXSTS_PTXFSAVL_MSK        ((uint32_t)0xFFFFU      )
#define  OTG_HS_HPTXSTS_PTXFSAVL(x)        (((uint32_t)    x        ) & OTG_HS_HPTXSTS_PTXFSAVL_MSK)
#define  OTG_HS_HPTXSTS_PTXQSAV_POS         ((uint32_t)           16U)
#define  OTG_HS_HPTXSTS_PTXQSAV_MSK         ((uint32_t) 0xFFU  << OTG_HS_HPTXSTS_PTXQSAV_POS)
#define  OTG_HS_HPTXSTS_PTXQSAV(x)         (((uint32_t)    x   << OTG_HS_HPTXSTS_PTXQSAV_POS) & OTG_HS_HPTXSTS_PTXQSAV_MSK)
#define  OTG_HS_HPTXSTS_PTXQTOP_POS         ((uint32_t)           24U)
#define  OTG_HS_HPTXSTS_PTXQTOP_MSK         ((uint32_t) 0xFFU  << OTG_HS_HPTXSTS_PTXQTOP_POS)
#define  OTG_HS_HPTXSTS_PTXQTOP(x)         (((uint32_t)    x   << OTG_HS_HPTXSTS_PTXQTOP_POS) & OTG_HS_HPTXSTS_PTXQTOP_MSK)

// OTG_HS Host all channels interrupt register (OTG_HS_HAINT)
#define  OTG_HS_HAINT_HAINT_POS             ((uint32_t)           0U)
#define  OTG_HS_HAINT_HAINT_MSK             ((uint32_t)0xFFFFU      )
#define  OTG_HS_HAINT_HAINT(x)             (((uint32_t)    x        ) & OTG_HS_HAINT_HAINT_MSK)

// OTG_HS Host all channels interrupt mask register (OTG_HS_HAINTMSK)
#define  OTG_HS_HAINTMSK_HAINTM_POS         ((uint32_t)           0U)
#define  OTG_HS_HAINTMSK_HAINTM_MSK         ((uint32_t)0xFFFFU      )
#define  OTG_HS_HAINTMSK_HAINTM(x)         (((uint32_t)    x        ) & OTG_HS_HAINTMSK_HAINTM_MSK)

// OTG_HS Host port control and status register (OTG_HS_HPRT)
#define  OTG_HS_HPRT_PCSTS                  ((uint32_t)    1U       )
#define  OTG_HS_HPRT_PCDET                  ((uint32_t)    1U  <<  1)
#define  OTG_HS_HPRT_PENA                   ((uint32_t)    1U  <<  2)
#define  OTG_HS_HPRT_PENCHNG                ((uint32_t)    1U  <<  3)
#define  OTG_HS_HPRT_POCA                   ((uint32_t)    1U  <<  4)
#define  OTG_HS_HPRT_POCCHNG                ((uint32_t)    1U  <<  5)
#define  OTG_HS_HPRT_PRES                   ((uint32_t)    1U  <<  6)
#define  OTG_HS_HPRT_PSUSP                  ((uint32_t)    1U  <<  7)
#define  OTG_HS_HPRT_PRST                   ((uint32_t)    1U  <<  8)
#define  OTG_HS_HPRT_PLSTS_POS              ((uint32_t)           10U)
#define  OTG_HS_HPRT_PLSTS_MSK              ((uint32_t)    3U  << OTG_HS_HPRT_PLSTS_POS)
#define  OTG_HS_HPRT_PLSTS(x)              (((uint32_t)    x   << OTG_HS_HPRT_PLSTS_POS) & OTG_HS_HPRT_PLSTS_MSK)
#define  OTG_HS_HPRT_PPWR                   ((uint32_t)    1U  << 12)
#define  OTG_HS_HPRT_PTCTL_POS              ((uint32_t)           13U)
#define  OTG_HS_HPRT_PTCTL_MSK              ((uint32_t) 0x0FU  << OTG_HS_HPRT_PTCTL_POS)
#define  OTG_HS_HPRT_PTCTL(x)              (((uint32_t)    x   << OTG_HS_HPRT_PTCTL_POS) & OTG_HS_HPRT_PTCTL_MSK)
#define  OTG_HS_HPRT_PSPD_POS               ((uint32_t)           17U)
#define  OTG_HS_HPRT_PSPD_MSK               ((uint32_t)    3U  << OTG_HS_HPRT_PSPD_POS)
#define  OTG_HS_HPRT_PSPD(x)               (((uint32_t)    x   << OTG_HS_HPRT_PSPD_POS) & OTG_HS_HPRT_PSPD_MSK)

// OTG_HS Host channel-x characteristics register (OTG_HS_HCCHARx)
#define  OTG_HS_HCCHARx_MPSIZ_POS           ((uint32_t)           0U)
#define  OTG_HS_HCCHARx_MPSIZ_MSK           ((uint32_t)0x7FFU       )
#define  OTG_HS_HCCHARx_MPSIZ(x)           (((uint32_t)    x        ) & OTG_HS_HCCHARx_MPSIZ_MSK)
#define  OTG_HS_HCCHARx_EPNUM_POS           ((uint32_t)           11U)
#define  OTG_HS_HCCHARx_EPNUM_MSK           ((uint32_t) 0x0FU  << OTG_HS_HCCHARx_EPNUM_POS)
#define  OTG_HS_HCCHARx_EPNUM(x)           (((uint32_t)    x   << OTG_HS_HCCHARx_EPNUM_POS) & OTG_HS_HCCHARx_EPNUM_MSK)
#define  OTG_HS_HCCHARx_EPDIR               ((uint32_t)    1U  << 15)
#define  OTG_HS_HCCHARx_LSDEV               ((uint32_t)    1U  << 17)
#define  OTG_HS_HCCHARx_EPTYP_POS           ((uint32_t)           18U)
#define  OTG_HS_HCCHARx_EPTYP_MSK           ((uint32_t)    3U  << OTG_HS_HCCHARx_EPTYP_POS)
#define  OTG_HS_HCCHARx_EPTYP(x)           (((uint32_t)    x   << OTG_HS_HCCHARx_EPTYP_POS) & OTG_HS_HCCHARx_EPTYP_MSK)
#define  OTG_HS_HCCHARx_MCNT_POS            ((uint32_t)           20U)
#define  OTG_HS_HCCHARx_MCNT_MSK            ((uint32_t)    3U  << OTG_HS_HCCHARx_MCNT_POS)
#define  OTG_HS_HCCHARx_MCNT(x)            (((uint32_t)    x   << OTG_HS_HCCHARx_MCNT_POS) & OTG_HS_HCCHARx_MCNT_MSK)
#define  OTG_HS_HCCHARx_DAD_POS             ((uint32_t)           22U)
#define  OTG_HS_HCCHARx_DAD_MSK             ((uint32_t) 0x7FU  << OTG_HS_HCCHARx_DAD_POS)
#define  OTG_HS_HCCHARx_DAD(x)             (((uint32_t)    x   << OTG_HS_HCCHARx_DAD_POS) & OTG_HS_HCCHARx_DAD_MSK)
#define  OTG_HS_HCCHARx_ODDFRM              ((uint32_t)    1U  << 29)
#define  OTG_HS_HCCHARx_CHDIS               ((uint32_t)    1U  << 30)
#define  OTG_HS_HCCHARx_CHENA               ((uint32_t)    1U  << 31)

// OTG_HS Host channel-x split control register (OTG_HS_HCSPLTx)
#define  OTG_HS_HCSPLTx_PRTADDR_POS         ((uint32_t)           0U)
#define  OTG_HS_HCSPLTx_PRTADDR_MSK         ((uint32_t) 0x7FU       )
#define  OTG_HS_HCSPLTx_PRTADDR(x)         (((uint32_t)    x        ) & OTG_HS_HCSPLTx_PRTADDR_MSK)
#define  OTG_HS_HCSPLTx_HUBADDR_POS         ((uint32_t)           7U)
#define  OTG_HS_HCSPLTx_HUBADDR_MSK         ((uint32_t) 0x7FU  << OTG_HS_HCSPLTx_HUBADDR_POS)
#define  OTG_HS_HCSPLTx_HUBADDR(x)         (((uint32_t)    x   << OTG_HS_HCSPLTx_HUBADDR_POS) & OTG_HS_HCSPLTx_HUBADDR_MSK)
#define  OTG_HS_HCSPLTx_XACTPOS_POS         ((uint32_t)           14U)
#define  OTG_HS_HCSPLTx_XACTPOS_MSK         ((uint32_t)    3U  << OTG_HS_HCSPLTx_XACTPOS_POS)
#define  OTG_HS_HCSPLTx_XACTPOS(x)         (((uint32_t)    x   << OTG_HS_HCSPLTx_XACTPOS_POS) & OTG_HS_HCSPLTx_XACTPOS_MSK)
#define  OTG_HS_HCSPLTx_COMPLSPLT           ((uint32_t)    1U  << 16)
#define  OTG_HS_HCSPLTx_SPLITEN             ((uint32_t)    1U  << 31)

// OTG_HS Host channel-x interrupt register (OTG_HS_HCINTx)
#define  OTG_HS_HCINTx_XFRC                 ((uint32_t)    1U       )
#define  OTG_HS_HCINTx_CHH                  ((uint32_t)    1U  <<  1)
#define  OTG_HS_HCINTx_AHBERR               ((uint32_t)    1U  <<  2)
#define  OTG_HS_HCINTx_STALL                ((uint32_t)    1U  <<  3)
#define  OTG_HS_HCINTx_NAK                  ((uint32_t)    1U  <<  4)
#define  OTG_HS_HCINTx_ACK                  ((uint32_t)    1U  <<  5)
#define  OTG_HS_HCINTx_NYET                 ((uint32_t)    1U  <<  6)
#define  OTG_HS_HCINTx_TXERR                ((uint32_t)    1U  <<  7)
#define  OTG_HS_HCINTx_BBERR                ((uint32_t)    1U  <<  8)
#define  OTG_HS_HCINTx_FRMOR                ((uint32_t)    1U  <<  9)
#define  OTG_HS_HCINTx_DTERR                ((uint32_t)    1U  << 10)
#define  OTG_HS_HCINTx_ERR                  ((uint32_t) 0x0FU  <<  7)

// OTG_HS Host channel-x interrupt mask register (OTG_HS_HCINTMSKx)
#define  OTG_HS_HCINTMSKx_XFRCM             ((uint32_t)    1U       )
#define  OTG_HS_HCINTMSKx_CHHM              ((uint32_t)    1U  <<  1)
#define  OTG_HS_HCINTMSKx_AHBERR            ((uint32_t)    1U  <<  2)
#define  OTG_HS_HCINTMSKx_STALLM            ((uint32_t)    1U  <<  3)
#define  OTG_HS_HCINTMSKx_NAKM              ((uint32_t)    1U  <<  4)
#define  OTG_HS_HCINTMSKx_ACKM              ((uint32_t)    1U  <<  5)
#define  OTG_HS_HCINTMSKx_NYET              ((uint32_t)    1U  <<  6)
#define  OTG_HS_HCINTMSKx_TXERRM            ((uint32_t)    1U  <<  7)
#define  OTG_HS_HCINTMSKx_BBERRM            ((uint32_t)    1U  <<  8)
#define  OTG_HS_HCINTMSKx_FRMORM            ((uint32_t)    1U  <<  9)
#define  OTG_HS_HCINTMSKx_DTERRM            ((uint32_t)    1U  << 10)

// OTG_HS Host channel-x transfer size register (OTG_HS_HCTSIZx)
#define  OTG_HS_HCTSIZx_XFRSIZ_POS          ((uint32_t)           0U)
#define  OTG_HS_HCTSIZx_XFRSIZ_MSK          ((uint32_t)0x7FFFFU     )
#define  OTG_HS_HCTSIZx_XFRSIZ(x)          (((uint32_t)    x        ) & OTG_HS_HCTSIZx_XFRSIZ_MSK)
#define  OTG_HS_HCTSIZx_PKTCNT_POS          ((uint32_t)           19U)
#define  OTG_HS_HCTSIZx_PKTCNT_MSK          ((uint32_t)0x3FFU  << OTG_HS_HCTSIZx_PKTCNT_POS)
#define  OTG_HS_HCTSIZx_PKTCNT(x)          (((uint32_t)    x   << OTG_HS_HCTSIZx_PKTCNT_POS) & OTG_HS_HCTSIZx_PKTCNT_MSK)
#define  OTG_HS_HCTSIZx_DPID_POS            ((uint32_t)           29U)
#define  OTG_HS_HCTSIZx_DPID_MSK            ((uint32_t)    3U  << OTG_HS_HCTSIZx_DPID_POS)
#define  OTG_HS_HCTSIZx_DPID(x)            (((uint32_t)    x   << OTG_HS_HCTSIZx_DPID_POS) & OTG_HS_HCTSIZx_DPID_MSK)
#define  OTG_HS_HCTSIZx_DPID_DATA0          (OTG_HS_HCTSIZx_DPID(0U))
#define  OTG_HS_HCTSIZx_DPID_DATA2          (OTG_HS_HCTSIZx_DPID(1U))
#define  OTG_HS_HCTSIZx_DPID_DATA1          (OTG_HS_HCTSIZx_DPID(2U))
#define  OTG_HS_HCTSIZx_DPID_MDATA          (OTG_HS_HCTSIZx_DPID(3U))
#define  OTG_HS_HCTSIZx_DPID_SETUP          (OTG_HS_HCTSIZx_DPID(3U))
#define  OTG_HS_HCISIZx_DOPING              ((uint32_t)    1U  << 31)

// OTG_HS device configuration register (OTG_HS_DCFG)
#define  OTG_HS_DCFG_DSPD_POS               ((uint32_t)           0U)
#define  OTG_HS_DCFG_DSPD_MSK               ((uint32_t)    3U       )
#define  OTG_HS_DCFG_DSPD(x)               (((uint32_t)    x        ) & OTG_HS_DCFG_DSPD_MSK)
#define  OTG_HS_DCFG_NZLSOHSK               ((uint32_t)    1U  <<  2)
#define  OTG_HS_DCFG_DAD_POS                ((uint32_t)           4U)
#define  OTG_HS_DCFG_DAD_MSK                ((uint32_t) 0x7FU  << OTG_HS_DCFG_DAD_POS)
#define  OTG_HS_DCFG_DAD(x)                (((uint32_t)    x   << OTG_HS_DCFG_DAD_POS) & OTG_HS_DCFG_DAD_MSK)
#define  OTG_HS_DCFG_PFIVL_POS              ((uint32_t)           11U)
#define  OTG_HS_DCFG_PFIVL_MSK              ((uint32_t)    3U  << OTG_HS_DCFG_PFIVL_POS)
#define  OTG_HS_DCFG_PFIVL(x)              (((uint32_t)    x   << OTG_HS_DCFG_PFIVL_POS) & OTG_HS_DCFG_PFIVL_MSK)
#define  OTG_HS_DCFG_PERSCHIVL_POS          ((uint32_t)           24U)
#define  OTG_HS_DCFG_PERSCHIVL_MSK          ((uint32_t)    3U  << OTG_HS_DCFG_PERSCHIVL_POS)
#define  OTG_HS_DCFG_PERSCHIVL(x)          (((uint32_t)    x   << OTG_HS_DCFG_PERSCHIVL_POS) & OTG_HS_DCFG_PERSCHIVL_MSK)

// OTG_HS device control register (OTG_HS_DCTL)
#define  OTG_HS_DCTL_RWUSIG                 ((uint32_t)    1U       )
#define  OTG_HS_DCTL_SDIS                   ((uint32_t)    1U  <<  1)
#define  OTG_HS_DCTL_GINSTS                 ((uint32_t)    1U  <<  2)
#define  OTG_HS_DCTL_GONSTS                 ((uint32_t)    1U  <<  3)
#define  OTG_HS_DCTL_TCTL_POS               ((uint32_t)           4U)
#define  OTG_HS_DCTL_TCTL_MSK               ((uint32_t)    7U  << OTG_HS_DCTL_TCTL_POS)
#define  OTG_HS_DCTL_TCTL(x)               (((uint32_t)    x   << OTG_HS_DCTL_TCTL_POS) & OTG_HS_DCTL_TCTL_MSK)
#define  OTG_HS_DCTL_SGINAK                 ((uint32_t)    1U  <<  7)
#define  OTG_HS_DCTL_CGINAK                 ((uint32_t)    1U  <<  8)
#define  OTG_HS_DCTL_SGONAK                 ((uint32_t)    1U  <<  9)
#define  OTG_HS_DCTL_CGONAK                 ((uint32_t)    1U  << 10)
#define  OTG_HS_DCTL_POPRGDNE               ((uint32_t)    1U  << 11)

// OTG_HS device status register (OTG_HS_DSTS)
#define  OTG_HS_DSTS_SUSPSTS                ((uint32_t)    1U       )
#define  OTG_HS_DSTS_ENUMSPD_POS            ((uint32_t)           1U)
#define  OTG_HS_DSTS_ENUMSPD_MSK            ((uint32_t)    3U  << OTG_HS_DSTS_ENUMSPD_POS)
#define  OTG_HS_DSTS_ENUMSPD(x)            (((uint32_t)    x   << OTG_HS_DSTS_ENUMSPD_POS) & OTG_HS_DSTS_ENUMSPD_MSK)
#define  OTG_HS_DSTS_EERR                   ((uint32_t)    1U  <<  3)
#define  OTG_HS_DSTS_FNSOF_POS              ((uint32_t)           8U)
#define  OTG_HS_DSTS_FNSOF_MSK              ((uint32_t)0x3FFFU << OTG_HS_DSTS_FNSOF_POS)
#define  OTG_HS_DSTS_FNSOF(x)              (((uint32_t)    x   << OTG_HS_DSTS_FNSOF_POS) & OTG_HS_DSTS_FNSOF_MSK)

// OTG_HS device IN endpoint common interrupt mask register (OTG_HS_DIEPMSK)
#define  OTG_HS_DIEPMSK_XFRCM               ((uint32_t)    1U       )
#define  OTG_HS_DIEPMSK_EPDM                ((uint32_t)    1U  <<  1)
#define  OTG_HS_DIEPMSK_TOM                 ((uint32_t)    1U  <<  3)
#define  OTG_HS_DIEPMSK_ITTXFEMSK           ((uint32_t)    1U  <<  4)
#define  OTG_HS_DIEPMSK_INEPNMM             ((uint32_t)    1U  <<  5)
#define  OTG_HS_DIEPMSK_INEPNEM             ((uint32_t)    1U  <<  6)
#define  OTG_HS_DIEPMSK_TXFURM              ((uint32_t)    1U  <<  8)
#define  OTG_HS_DIEPMSK_BIM                 ((uint32_t)    1U  <<  9)

// OTG_HS device OUT endpoint common interrupt mask register (OTG_HS_DOEPMSK)
#define  OTG_HS_DOEPMSK_XFRCM               ((uint32_t)    1U       )
#define  OTG_HS_DOEPMSK_EPDM                ((uint32_t)    1U  <<  1)
#define  OTG_HS_DOEPMSK_STUPM               ((uint32_t)    1U  <<  3)
#define  OTG_HS_DOEPMSK_OTEPDM              ((uint32_t)    1U  <<  4)
#define  OTG_HS_DOEPMSK_B2BSTUP             ((uint32_t)    1U  <<  6)
#define  OTG_HS_DOEPMSK_OPEM                ((uint32_t)    1U  <<  8)
#define  OTG_HS_DOEPMSK_BOIM                ((uint32_t)    1U  <<  9)

// OTG_HS device all endpoints interrupt register (OTG_HS_DAINT)
#define  OTG_HS_DAINT_IEPINT(in_ep)         ((uint32_t)((1U <<         in_ep)  & 0x0000FFFFU))
#define  OTG_HS_DAINT_OEPINT(out_ep)        ((uint32_t)((1U << (16U + out_ep)) & 0xFFFF0000U))

// OTG_HS device all endpoints interrupt mask register (OTG_HS_DAINTMSK)
#define  OTG_HS_DAINTMSK_IEPM(in_ep)        ((uint32_t)((1U <<         in_ep)  & 0x0000FFFFU))
#define  OTG_HS_DAINTMSK_OEPM(out_ep)       ((uint32_t)((1U << (16U + out_ep)) & 0xFFFF0000U))

// OTG_HS device Vbus discharge time register (OTG_HS_DVBUSDIS)
#define  OTG_HS_DVBUSDIS_VBUSDT_POS         ((uint32_t)           0U)
#define  OTG_HS_DVBUSDIS_VBUSDT_MSK         ((uint32_t)0xFFFFU      )
#define  OTG_HS_DVBUSDIS_VBUSDT(x)         (((uint32_t)    x        ) & OTG_HS_DVBUSDIS_VBUSDT_MSK)

// OTG_HS device Vbus pulsing time register (OTG_HS_DVBUSPULSE)
#define  OTG_HS_DVBUSPULSE_DVBUSP_POS       ((uint32_t)           0U)
#define  OTG_HS_DVBUSPULSE_DVBUSP_MSK       ((uint32_t)0xFFFU       )
#define  OTG_HS_DVBUSPULSE_DVBUSP(x)       (((uint32_t)    x        ) & OTG_HS_DVBUSPULSE_DVBUSP)

// OTG_HS Device threshold control register (OTG_HS_DTHRCTL)
#define  OTG_HS_DTHRCTL_NONISOTHREN         ((uint32_t)    1U       )
#define  OTG_HS_DTHRCTL_ISOTHREN            ((uint32_t)    1U  <<  1)
#define  OTG_HS_DTHRCTL_TXTHRLEN_POS        ((uint32_t)           2U)
#define  OTG_HS_DTHRCTL_TXTHRLEN_MSK        ((uint32_t)0x1FFU  << OTG_HS_DTHRCTL_TXTHRLEN_POS)
#define  OTG_HS_DTHRCTL_TXTHRLEN(x)        (((uint32_t)    x   << OTG_HS_DTHRCTL_TXTHRLEN_POS) & OTG_HS_DTHRCTL_TXTHRLEN_MSK)
#define  OTG_HS_DTHRCTL_RXTHREN             ((uint32_t)    1U  << 16)
#define  OTG_HS_DTHRCTL_RXTHRLEN_POS        ((uint32_t)           17U)
#define  OTG_HS_DTHRCTL_RXTHRLEN_MSK        ((uint32_t)0x1FFU  << OTG_HS_DTHRCTL_RXTHRLEN_POS)
#define  OTG_HS_DTHRCTL_RXTHRLEN(x)        (((uint32_t)    x   << OTG_HS_DTHRCTL_RXTHRLEN_POS) & OTG_HS_DTHRCTL_RXTHRLEN_MSK)
#define  OTG_HS_DTHRCTL_ARPEN               ((uint32_t)    1U  << 27)

// OTG_HS device IN endpoint FIFO emtpy interrupt mask register (OTG_HS_DIEPEMPMSK)
#define  OTG_HS_DIEPEMPMSK_INEPTXFEM_POS    ((uint32_t)           0U)
#define  OTG_HS_DIEPEMPMSK_INEPTXFEM_MSK    ((uint32_t)0xFFFFU      )
#define  OTG_HS_DIEPEMPMSK_INEPTXFEM(x)    (((uint32_t)    x        ) & OTG_HS_DIEPEMPMSK_INEPTXFEM_MSK)

// OTG_HS device each endpoint interrupt register (OTG_HS_DEACHINT)
#define  OTG_HS_DEACHINT_IEP1INT            ((uint32_t)    1U  <<  1)
#define  OTG_HS_DEACHINT_OEP1INT            ((uint32_t)    1U  << 17)

// OTG_HS device each endpoint interrupt register mask (OTG_HS_DEACHINTMSK)
#define  OTG_HS_DEACHINTMSK_IEP1INTM        ((uint32_t)    1U  <<  1)
#define  OTG_HS_DEACHINTMSK_OEP1INTM        ((uint32_t)    1U  << 17)

// OTG_HS device endpoint-x control register (OTG_HS_DIEPCTLx)
#define  OTG_HS_DIEPCTLx_MPSIZ_POS          ((uint32_t)           0U)
#define  OTG_HS_DIEPCTLx_MPSIZ_MSK          ((uint32_t)0x7FFU       )
#define  OTG_HS_DIEPCTLx_MPSIZ(x)          (((uint32_t)    x        ) & OTG_HS_DIEPCTLx_MPSIZ_MSK)
#define  OTG_HS_DIEPCTLx_USBAEP             ((uint32_t)    1U  << 15)
#define  OTG_HS_DIEPCTLx_EONUM_POS          ((uint32_t)           16U)
#define  OTG_HS_DIEPCTLx_EONUM              ((uint32_t)    1U  << OTG_HS_DIEPCTLx_EONUM_POS)
#define  OTG_HS_DIEPCTLx_DPID               ((uint32_t)    1U  << 16)
#define  OTG_HS_DIEPCTLx_NAKSTS             ((uint32_t)    1U  << 17)
#define  OTG_HS_DIEPCTLx_EPTYP_POS          ((uint32_t)           18U)
#define  OTG_HS_DIEPCTLx_EPTYP_MSK          ((uint32_t)    3U  << OTG_HS_DIEPCTLx_EPTYP_POS)
#define  OTG_HS_DIEPCTLx_EPTYP(x)          (((uint32_t)    x   << OTG_HS_DIEPCTLx_EPTYP_POS) & OTG_HS_DIEPCTLx_EPTYP_MSK)
#define  OTG_HS_DIEPCTLx_STALL              ((uint32_t)    1U  << 21)
#define  OTG_HS_DIEPCTLx_TXFNUM_POS         ((uint32_t)           22U)
#define  OTG_HS_DIEPCTLx_TXFNUM_MSK         ((uint32_t) 0x0FU  << OTG_HS_DIEPCTLx_TXFNUM_POS)
#define  OTG_HS_DIEPCTLx_TXFNUM(x)         (((uint32_t)    x   << OTG_HS_DIEPCTLx_TXFNUM_POS) & OTG_HS_DIEPCTLx_TXFNUM_MSK)
#define  OTG_HS_DIEPCTLx_CNAK               ((uint32_t)    1U  << 26)
#define  OTG_HS_DIEPCTLx_SNAK               ((uint32_t)    1U  << 27)
#define  OTG_HS_DIEPCTLx_SD0PID             ((uint32_t)    1U  << 28)
#define  OTG_HS_DIEPCTLx_SEVNFRM            ((uint32_t)    1U  << 28)
#define  OTG_HS_DIEPCTLx_SODDFRM            ((uint32_t)    1U  << 29)
#define  OTG_HS_DIEPCTLx_EPDIS              ((uint32_t)    1U  << 30)
#define  OTG_HS_DIEPCTLx_EPENA              ((uint32_t)    1U  << 31)

// OTG_HS device control OUT endpoint 0 control register (OTG_HS_DOEPCTL0)
#define  OTG_HS_DOEPCTL0_MPSIZ_POS          ((uint32_t)           0U)
#define  OTG_HS_DOEPCTL0_MPSIZ_MSK          ((uint32_t)    3U       )
#define  OTG_HS_DOEPCTL0_MPSIZ(x)          (((uint32_t)    x        ) & OTG_HS_DOEPCTL0_MPSIZ_MSK)
#define  OTG_HS_DOEPCTL0_USBAEP             ((uint32_t)    1U  << 15)
#define  OTG_HS_DOEPCTL0_NAKSTS             ((uint32_t)    1U  << 17)
#define  OTG_HS_DOEPCTL0_EPTYP_POS          ((uint32_t)           18U)
#define  OTG_HS_DOEPCTL0_EPTYP_MSK          ((uint32_t)    3U  << OTG_HS_DOEPCTL0_EPTYP_POS)
#define  OTG_HS_DOEPCTL0_EPTYP(x)          (((uint32_t)    x   << OTG_HS_DOEPCTL0_EPTYP_POS) & OTG_HS_DOEPCTL0_EPTYP_MSK)
#define  OTG_HS_DOEPCTL0_SNPM               ((uint32_t)    1U  << 20)
#define  OTG_HS_DOEPCTL0_STALL              ((uint32_t)    1U  << 21)
#define  OTG_HS_DOEPCTL0_CNAK               ((uint32_t)    1U  << 26)
#define  OTG_HS_DOEPCTL0_SNAK               ((uint32_t)    1U  << 27)
#define  OTG_HS_DOEPCTL0_EPDIS              ((uint32_t)    1U  << 30)
#define  OTG_HS_DOEPCTL0_EPENA              ((uint32_t)    1U  << 31)

// OTG_HS device endpoint-x control register (OTG_HS_DOEPCTLx)
#define  OTG_HS_DOEPCTLx_MPSIZ_POS          ((uint32_t)           0U)
#define  OTG_HS_DOEPCTLx_MPSIZ_MSK          ((uint32_t)0x7FFU       )
#define  OTG_HS_DOEPCTLx_MPSIZ(x)          (((uint32_t)    x        ) & OTG_HS_DOEPCTLx_MPSIZ_MSK)
#define  OTG_HS_DOEPCTLx_USBAEP             ((uint32_t)    1U  << 15)
#define  OTG_HS_DOEPCTLx_EONUM_POS          ((uint32_t)           16U)
#define  OTG_HS_DOEPCTLx_EONUM              ((uint32_t)    1U  << OTG_HS_DOEPCTLx_EONUM_POS)
#define  OTG_HS_DOEPCTLx_DPID               ((uint32_t)    1U  << 16)
#define  OTG_HS_DOEPCTLx_NAKSTS             ((uint32_t)    1U  << 17)
#define  OTG_HS_DOEPCTLx_EPTYP_POS          ((uint32_t)           18U)
#define  OTG_HS_DOEPCTLx_EPTYP_MSK          ((uint32_t)    3U  << OTG_HS_DOEPCTLx_EPTYP_POS)
#define  OTG_HS_DOEPCTLx_EPTYP(x)          (((uint32_t)    x   << OTG_HS_DOEPCTLx_EPTYP_POS) & OTG_HS_DOEPCTLx_EPTYP_MSK)
#define  OTG_HS_DOEPCTLx_SNPM               ((uint32_t)    1U  << 20)
#define  OTG_HS_DOEPCTLx_STALL              ((uint32_t)    1U  << 21)
#define  OTG_HS_DOEPCTLx_CNAK               ((uint32_t)    1U  << 26)
#define  OTG_HS_DOEPCTLx_SNAK               ((uint32_t)    1U  << 27)
#define  OTG_HS_DOEPCTLx_SD0PID             ((uint32_t)    1U  << 28)
#define  OTG_HS_DOEPCTLx_SEVNFRM            ((uint32_t)    1U  << 28)
#define  OTG_HS_DOEPCTLx_SODDFRM            ((uint32_t)    1U  << 29)
#define  OTG_HS_DOEPCTLx_EPDIS              ((uint32_t)    1U  << 30)
#define  OTG_HS_DOEPCTLx_EPENA              ((uint32_t)    1U  << 31)

// OTG_HS device endpoint-x interrupt register (OTG_HS_DIEPINTx)
#define  OTG_HS_DIEPINTx_XFCR               ((uint32_t)    1U       )
#define  OTG_HS_DIEPINTx_EPDISD             ((uint32_t)    1U  <<  1)
#define  OTG_HS_DIEPINTx_TOC                ((uint32_t)    1U  <<  3)
#define  OTG_HS_DIEPINTx_ITTXFE             ((uint32_t)    1U  <<  4)
#define  OTG_HS_DIEPINTx_INEPNE             ((uint32_t)    1U  <<  6)
#define  OTG_HS_DIEPINTx_TXFE               ((uint32_t)    1U  <<  7)
#define  OTG_HS_DIEPINTx_TXFIFOUDRN         ((uint32_t)    1U  <<  8)
#define  OTG_HS_DIEPINTx_BNA                ((uint32_t)    1U  <<  9)
#define  OTG_HS_DIEPINTx_PKTDRPSTS          ((uint32_t)    1U  << 11)
#define  OTG_HS_DIEPINTx_BERR               ((uint32_t)    1U  << 12)
#define  OTG_HS_DIEPINTx_NAK                ((uint32_t)    1U  << 13)

// OTG_HS device endpoint-x interrupt register (OTG_HS_DOEPINTx)
#define  OTG_HS_DOEPINTx_XFCR               ((uint32_t)    1U       )
#define  OTG_HS_DOEPINTx_EPDISD             ((uint32_t)    1U  <<  1)
#define  OTG_HS_DOEPINTx_STUP               ((uint32_t)    1U  <<  3)
#define  OTG_HS_DOEPINTx_OTEPDIS            ((uint32_t)    1U  <<  4)
#define  OTG_HS_DOEPINTx_B2BSTUP            ((uint32_t)    1U  <<  6)
#define  OTG_HS_DOEPINTx_NYET               ((uint32_t)    1U  << 14)

// OTG_HS device IN endpoint 0 transfer size register (OTG_HS_DIEPTSIZ0)
#define  OTG_HS_DIEPTSIZ0_XFRSIZ_POS        ((uint32_t)           0U)
#define  OTG_HS_DIEPTSIZ0_XFRSIZ_MSK        ((uint32_t) 0x7FU       )
#define  OTG_HS_DIEPTSIZ0_XFRSIZ(x)        (((uint32_t)    x        ) & OTG_HS_DIEPTSIZ0_XFRSIZ_MSK)
#define  OTG_HS_DIEPTSIZ0_PKTCNT_POS        ((uint32_t)          19U)
#define  OTG_HS_DIEPTSIZ0_PKTCNT_MSK        ((uint32_t)    3U  << OTG_HS_DIEPTSIZ0_PKTCNT_POS)
#define  OTG_HS_DIEPTSIZ0_PKTCNT(x)        (((uint32_t)    x   << OTG_HS_DIEPTSIZ0_PKTCNT_POS) & OTG_HS_DIEPTSIZ0_PKTCNT_MSK)

// OTG_HS device OUT endpoint 0 transfer size register (OTG_HS_DOEPTSIZ0)
#define  OTG_HS_DOEPTSIZ0_XFRSIZ_POS        ((uint32_t)           0U)
#define  OTG_HS_DOEPTSIZ0_XFRSIZ_MSK        ((uint32_t) 0x7FU       )
#define  OTG_HS_DOEPTSIZ0_XFRSIZ(x)        (((uint32_t)    x        ) & OTG_HS_DOEPTSIZ0_XFRSIZ_MSK)
#define  OTG_HS_DOEPTSIZ0_PKTCNT            ((uint32_t)    1U  << 19)
#define  OTG_HS_DOEPTSIZ0_STUPCNT_POS       ((uint32_t)           29U)
#define  OTG_HS_DOEPTSIZ0_STUPCNT_MSK       ((uint32_t)    3U  << OTG_HS_DOEPTSIZ0_STUPCNT_POS)
#define  OTG_HS_DOEPTSIZ0_STUPCNT(x)       (((uint32_t)    x   << OTG_HS_DOEPTSIZ0_STUPCNT_POS) & OTG_HS_DOEPTSIZ0_STUPCNT_MSK)

// OTG_HS device IN endpoints transver size register (OTG_HS_DIEPTSIZx)
#define  OTG_HS_DIEPTSIZx_XFRSIZ_POS        ((uint32_t)           0U)
#define  OTG_HS_DIEPTSIZx_XFRSIZ_MSK        ((uint32_t)0x7FFFFU     )
#define  OTG_HS_DIEPTSIZx_XFRSIZ(x)        (((uint32_t)    x        ) & OTG_HS_DIEPTSIZx_XFRSIZ_MSK)
#define  OTG_HS_DIEPTSIZx_PKTCNT_POS        ((uint32_t)           19U)
#define  OTG_HS_DIEPTSIZx_PKTCNT_MSK        ((uint32_t)0x3FFU  << OTG_HS_DIEPTSIZx_PKTCNT_POS)
#define  OTG_HS_DIEPTSIZx_PKTCNT(x)        (((uint32_t)    x   << OTG_HS_DIEPTSIZx_PKTCNT_POS) & OTG_HS_DIEPTSIZx_PKTCNT_MSK)
#define  OTG_HS_DIEPTSIZx_MCNT_POS          ((uint32_t)           29U)
#define  OTG_HS_DIEPTSIZx_MCNT_MSK          ((uint32_t)    3U  << OTG_HS_DIEPTSIZx_MCNT_POS)
#define  OTG_HS_DIEPTSIZx_MCNT(x)          (((uint32_t)    x   << OTG_HS_DIEPTSIZx_MCNT_POS) & OTG_HS_DIEPTSIZx_MCNT_MSK)

// OTG_HS device IN endpoint transmit FIFO status register (OTG_HS_DTXFSTSx)
#define  OTG_HS_DTXFSTSx_INEPTFSAV_POS      ((uint32_t)           0U)
#define  OTG_HS_DTXFSTSx_INEPTFSAV_MSK      ((uint32_t)0xFFFFU      )
#define  OTG_HS_DTXFSTSx_INEPTFSAV(x)      (((uint32_t)    x        ) & OTG_HS_DTXFSTSx_INEPTFSAV_MSK)

// OTG_HS device OUT endpoint-x transfer size register (OTG_HS_DOEPTSIZx)
#define  OTG_HS_DOEPTSIZx_XFRSIZ_POS        ((uint32_t)           0U)
#define  OTG_HS_DOEPTSIZx_XFRSIZ_MSK        ((uint32_t)0x7FFFFU     )
#define  OTG_HS_DOEPTSIZx_XFRSIZ(x)        (((uint32_t)    x        ) & OTG_HS_DOEPTSIZx_XFRSIZ_MSK)
#define  OTG_HS_DOEPTSIZx_PKTCNT_POS        ((uint32_t)           19U)
#define  OTG_HS_DOEPTSIZx_PKTCNT_MSK        ((uint32_t)0x3FFU  << OTG_HS_DOEPTSIZx_PKTCNT_POS)
#define  OTG_HS_DOEPTSIZx_PKTCNT(x)        (((uint32_t)    x   << OTG_HS_DOEPTSIZx_PKTCNT_POS) & OTG_HS_DOEPTSIZx_PKTCNT_MSK)
#define  OTG_HS_DOEPTSIZx_RXDPID_POS        ((uint32_t)           29U)
#define  OTG_HS_DOEPTSIZx_RXDPID_MSK        ((uint32_t)    3U  << OTG_HS_DOEPTSIZx_RXDPID_POS)
#define  OTG_HS_DOEPTSIZx_RXDPID(x)        (((uint32_t)    x   << OTG_HS_DOEPTSIZx_RXDPID_POS) & OTG_HS_DOEPTSIZx_RXDPID_MSK)
#define  OTG_HS_DOEPTSIZx_STUPCNT_POS       ((uint32_t)           29U)
#define  OTG_HS_DOEPTSIZx_STUPCNT_MSK       ((uint32_t)    3U  << OTG_HS_DOEPTSIZx_STUPCNT_POS)
#define  OTG_HS_DOEPTSIZx_STUPCNT(x)       (((uint32_t)    x   << OTG_HS_DOEPTSIZx_STUPCNT_POS) & OTG_HS_DOEPTSIZx_STUPCNT_MSK)

// OTG_HS power and gate clock gating control register (OTG_HS_PCGCCTL)
#define  OTG_HS_PCGCCTL_STPPCLK             ((uint32_t)    1U       )
#define  OTG_HS_PCGCCTL_GATEHCLK            ((uint32_t)    1U  <<  1)
#define  OTG_HS_PCGCCTL_PHYSUSP             ((uint32_t)    1U  <<  4)

// Global functions and variables exported by OTG_HS_STM32F7xx.c module
extern uint8_t otg_hs_role;

extern void OTG_HS_PinsConfigure   (uint8_t pins_mask);
extern void OTG_HS_PinsUnconfigure (uint8_t pins_mask);
extern void OTG_HS_PinVbusOnOff    (bool state);
extern bool OTG_HS_PinGetOC        (void);

extern void OTG_HS_IRQHandler      (void);

// Global functions and variables exported by driver .c module
extern void USBD_HS_IRQ (uint32_t gintsts);
extern void USBH_HS_IRQ (uint32_t gintsts);

extern ARM_DRIVER_USBD Driver_USBD1;
extern ARM_DRIVER_USBH Driver_USBH1;
