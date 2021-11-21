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
 * $Revision:    V2.11
 *
 * Project:      Ethernet Media Access (MAC) Definitions for STM32F4xx
 * -------------------------------------------------------------------------- */

#ifndef __EMAC_STM32F4XX_H
#define __EMAC_STM32F4XX_H

#include <string.h>

#include "cmsis_compiler.h"

#include "Driver_ETH_MAC.h"
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
  #if (defined(RTE_Drivers_ETH_MAC0) && !RTE_ETH)
  #error "Ethernet not configured in RTE_Device.h!"
  #endif

  #if (RTE_ETH_MII && RTE_ETH_RMII)
  #error "Ethernet interface configuration in RTE_Device.h is invalid!"
  #endif

  #define MX_ETH_MDC_GPIOx        RTE_ETH_MDI_MDC_PORT
  #define MX_ETH_MDC_GPIO_Pin     (1U << RTE_ETH_MDI_MDC_PIN)
  #define MX_ETH_MDIO_GPIOx       RTE_ETH_MDI_MDIO_PORT
  #define MX_ETH_MDIO_GPIO_Pin    (1U << RTE_ETH_MDI_MDIO_PIN)

  #if (RTE_ETH_MII)
    #define ETH_MII                 1

    #define MX_ETH_TXD0_GPIOx       RTE_ETH_MII_TXD0_PORT
    #define MX_ETH_TXD0_GPIO_Pin    (1U << RTE_ETH_MII_TXD0_PIN)
    #define MX_ETH_TXD1_GPIOx       RTE_ETH_MII_TXD1_PORT
    #define MX_ETH_TXD1_GPIO_Pin    (1U << RTE_ETH_MII_TXD1_PIN)
    #define MX_ETH_TXD2_GPIOx       RTE_ETH_MII_TXD2_PORT
    #define MX_ETH_TXD2_GPIO_Pin    (1U << RTE_ETH_MII_TXD2_PIN)
    #define MX_ETH_TXD3_GPIOx       RTE_ETH_MII_TXD3_PORT
    #define MX_ETH_TXD3_GPIO_Pin    (1U << RTE_ETH_MII_TXD3_PIN)
    #define MX_ETH_RXD0_GPIOx       RTE_ETH_MII_RXD0_PORT
    #define MX_ETH_RXD0_GPIO_Pin    (1U << RTE_ETH_MII_RXD0_PIN)
    #define MX_ETH_RXD1_GPIOx       RTE_ETH_MII_RXD1_PORT
    #define MX_ETH_RXD1_GPIO_Pin    (1U << RTE_ETH_MII_RXD1_PIN)
    #define MX_ETH_RXD2_GPIOx       RTE_ETH_MII_RXD2_PORT
    #define MX_ETH_RXD2_GPIO_Pin    (1U << RTE_ETH_MII_RXD2_PIN)
    #define MX_ETH_RXD3_GPIOx       RTE_ETH_MII_RXD3_PORT
    #define MX_ETH_RXD3_GPIO_Pin    (1U << RTE_ETH_MII_RXD3_PIN)
    #define MX_ETH_TX_EN_GPIOx      RTE_ETH_MII_TX_EN_PORT
    #define MX_ETH_TX_EN_GPIO_Pin   (1U << RTE_ETH_MII_TX_EN_PIN)
    #define MX_ETH_TX_CLK_GPIOx     RTE_ETH_MII_TX_CLK_PORT
    #define MX_ETH_TX_CLK_GPIO_Pin  (1U << RTE_ETH_MII_TX_CLK_PIN)
    #define MX_ETH_RX_CLK_GPIOx     RTE_ETH_MII_RX_CLK_PORT
    #define MX_ETH_RX_CLK_GPIO_Pin  (1U << RTE_ETH_MII_RX_CLK_PIN)
    #define MX_ETH_CRS_GPIOx        RTE_ETH_MII_CRS_PORT
    #define MX_ETH_CRS_GPIO_Pin     (1U << RTE_ETH_MII_CRS_PIN)
    #define MX_ETH_COL_GPIOx        RTE_ETH_MII_COL_PORT
    #define MX_ETH_COL_GPIO_Pin     (1U << RTE_ETH_MII_COL_PIN)
    #define MX_ETH_RX_DV_GPIOx      RTE_ETH_MII_RX_DV_PORT
    #define MX_ETH_RX_DV_GPIO_Pin   (1U << RTE_ETH_MII_RX_DV_PIN)
    #define MX_ETH_RX_ER_GPIOx      RTE_ETH_MII_RX_ER_PORT
    #define MX_ETH_RX_ER_GPIO_Pin   (1U << RTE_ETH_MII_RX_ER_PIN)

  #else
    #define ETH_MII                 0

    #define MX_ETH_TXD0_GPIOx       RTE_ETH_RMII_TXD0_PORT
    #define MX_ETH_TXD0_GPIO_Pin    (1U << RTE_ETH_RMII_TXD0_PIN)
    #define MX_ETH_TXD1_GPIOx       RTE_ETH_RMII_TXD1_PORT
    #define MX_ETH_TXD1_GPIO_Pin    (1U << RTE_ETH_RMII_TXD1_PIN)
    #define MX_ETH_RXD0_GPIOx       RTE_ETH_RMII_RXD0_PORT
    #define MX_ETH_RXD0_GPIO_Pin    (1U << RTE_ETH_RMII_RXD0_PIN)
    #define MX_ETH_RXD1_GPIOx       RTE_ETH_RMII_RXD1_PORT
    #define MX_ETH_RXD1_GPIO_Pin    (1U << RTE_ETH_RMII_RXD1_PIN)
    #define MX_ETH_TX_EN_GPIOx      RTE_ETH_RMII_TX_EN_PORT
    #define MX_ETH_TX_EN_GPIO_Pin   (1U << RTE_ETH_RMII_TX_EN_PIN)
    #define MX_ETH_REF_CLK_GPIOx    RTE_ETH_RMII_REF_CLK_PORT
    #define MX_ETH_REF_CLK_GPIO_Pin (1U << RTE_ETH_RMII_REF_CLK_PIN)
    #define MX_ETH_CRS_DV_GPIOx     RTE_ETH_RMII_CRS_DV_PORT
    #define MX_ETH_CRS_DV_GPIO_Pin  (1U << RTE_ETH_RMII_CRS_DV_PIN)

#endif /* RTE_ETH_RMII */

#else /* MX_Device.h */
  #if defined(MX_ETH_TXD2_Pin)   && defined(MX_ETH_TXD3_Pin)   && \
      defined(MX_ETH_RXD2_Pin)   && defined(MX_ETH_RXD3_Pin)   && \
      defined(MX_ETH_TX_CLK_Pin) && defined(MX_ETH_RX_CLK_Pin)
    #define ETH_MII             1
  #else
    #define ETH_MII             0
  #endif
#endif

/* EMAC Driver state flags */
#define EMAC_FLAG_INIT      (1 << 0)    // Driver initialized
#define EMAC_FLAG_POWER     (1 << 1)    // Driver power on
#define EMAC_FLAG_DMA_INIT  (1 << 2)    // DMA Initialized

/* TDES0 - DMA Descriptor TX Packet Control/Status */
#define DMA_TX_OWN      0x80000000U     // Own bit 1=DMA,0=CPU
#define DMA_TX_IC       0x40000000U     // Interrupt on completition
#define DMA_TX_LS       0x20000000U     // Last segment
#define DMA_TX_FS       0x10000000U     // First segment
#define DMA_TX_DC       0x08000000U     // Disable CRC
#define DMA_TX_DP       0x04000000U     // Disable pad
#define DMA_TX_TTSE     0x02000000U     // Transmit time stamp enable
#define DMA_TX_CIC      0x00C00000U     // Checksum insertion control
#define DMA_TX_CIC_IP   0x00400000U     // Checksum insertion for IP header only
#define DMA_TX_TER      0x00200000U     // Transmit end of ring
#define DMA_TX_TCH      0x00100000U     // Second address chained
#define DMA_TX_TTSS     0x00020000U     // Transmit time stamp status
#define DMA_TX_IHE      0x00010000U     // IP header error status
#define DMA_TX_ES       0x00008000U     // Error summary
#define DMA_TX_JT       0x00004000U     // Jabber timeout
#define DMA_TX_FF       0x00002000U     // Frame flushed
#define DMA_TX_IPE      0x00001000U     // IP payload error
#define DMA_TX_LC       0x00000800U     // Loss of carrier
#define DMA_TX_NC       0x00000400U     // No carrier
#define DMA_TX_LCOL     0x00000200U     // Late collision
#define DMA_TX_EC       0x00000100U     // Excessive collision
#define DMA_TX_VF       0x00000080U     // VLAN frame
#define DMA_TX_CC       0x00000078U     // Collision count
#define DMA_TX_ED       0x00000004U     // Excessive deferral
#define DMA_TX_UF       0x00000002U     // Underflow error
#define DMA_TX_DB       0x00000001U     // Deferred bit

/* TDES1 - DMA Descriptor TX Packet Control */
#define DMA_RX_TBS2     0x1FFF0000U     // Transmit buffer 2 size
#define DMA_RX_TBS1     0x00001FFFU     // Transmit buffer 1 size

/* RDES0 - DMA Descriptor RX Packet Status */
#define DMA_RX_OWN      0x80000000U     // Own bit 1=DMA,0=CPU
#define DMA_RX_AFM      0x40000000U     // Destination address filter fail
#define DMA_RX_FL       0x3FFF0000U     // Frame length mask
#define DMA_RX_ES       0x00008000U     // Error summary
#define DMA_RX_DE       0x00004000U     // Descriptor error
#define DMA_RX_SAF      0x00002000U     // Source address filter fail
#define DMA_RX_LE       0x00001000U     // Length error
#define DMA_RX_OE       0x00000800U     // Overflow error
#define DMA_RX_VLAN     0x00000400U     // VLAN tag
#define DMA_RX_FS       0x00000200U     // First descriptor
#define DMA_RX_LS       0x00000100U     // Last descriptor
#define DMA_RX_IPHCE    0x00000080U     // IPv4 header checksum error
#define DMA_RX_LC       0x00000040U     // late collision
#define DMA_RX_FT       0x00000020U     // Frame type
#define DMA_RX_RWT      0x00000010U     // Receive watchdog timeout
#define DMA_RX_RE       0x00000008U     // Receive error
#define DMA_RX_DRE      0x00000004U     // Dribble bit error
#define DMA_RX_CE       0x00000002U     // CRC error
#define DMA_RX_RMAM     0x00000001U     // Rx MAC adr.match/payload cks.error

/* RDES1 - DMA Descriptor RX Packet Control */
#define DMA_RX_DIC      0x80000000U     // Disable interrupt on completion
#define DMA_RX_RBS2     0x1FFF0000U     // Receive buffer 2 size
#define DMA_RX_RER      0x00008000U     // Receive end of ring
#define DMA_RX_RCH      0x00004000U     // Second address chained
#define DMA_RX_RBS1     0x00001FFFU     // Receive buffer 1 size

typedef struct _ETH_PIN {
  GPIO_TypeDef *port;
  uint16_t      pin;
  uint16_t      reserved;
} ETH_PIN;

/* EMAC Driver Control Information */
typedef struct {
  ARM_ETH_MAC_SignalEvent_t cb_event;   // Event callback
  uint8_t       flags;                  // Control and state flags
  uint8_t       tx_index;               // Transmit descriptor index
  uint8_t       rx_index;               // Receive descriptor index
#if (EMAC_CHECKSUM_OFFLOAD)
  bool          tx_cks_offload;         // Checksum offload enabled/disabled
#endif
#if (EMAC_TIME_STAMP)
  uint8_t       tx_ts_index;            // Transmit Timestamp descriptor index
#endif
  uint8_t      *frame_end;              // End of assembled frame fragments
} EMAC_CTRL;

// Global functions and variables exported by driver .c module
extern ARM_DRIVER_ETH_MAC Driver_ETH_MAC0;
#endif /* __EMAC_STM32F4XX_H */
