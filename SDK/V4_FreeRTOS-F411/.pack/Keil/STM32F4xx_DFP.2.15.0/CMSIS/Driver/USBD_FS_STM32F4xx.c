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
 * $Date:        27. February 2019
 * $Revision:    V2.21
 *
 * Driver:       Driver_USBD0
 * Configured:   via RTE_Device.h configuration file
 * Project:      USB Full/Low-Speed Device Driver for ST STM32F4xx
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                  Value
 *   ---------------------                  -----
 *   Connect to hardware via Driver_USBD# = 0
 * --------------------------------------------------------------------------
 * Defines used for driver configuration (at compile time):
 *
 *   USBD0_MAX_ENDPOINT_NUM:  defines maximum number of IN/OUT Endpoint pairs
 *                            that driver will support with Control Endpoint 0
 *                            not included, this value impacts driver memory
 *                            requirements
 *     - default value:       3
 *     - maximum value:       3
 *
 *   USBD0_RX_FIFO_SIZE:      FIFO size for reception for all endpoints
 *     - default value:       640
 *
 *   USBD0_TX0_FIFO_SIZE:     FIFO size for transmission on endpoint 0
 *     - default value:       160
 *
 *   USBD0_TX1_FIFO_SIZE:     FIFO size for transmission on endpoint 1
 *     - default value:       160
 *
 *   USBD0_TX2_FIFO_SIZE:     FIFO size for transmission on endpoint 2
 *     - default value:       160
 *
 *   USBD0_TX3_FIFO_SIZE:     FIFO size for transmission on endpoint 3
 *     - default value:       160
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 2.21
 *    Updated USBD_EndpointConfigure function to check that maximum packet 
 *    size requested fits into configured FIFO (compile time configured)
 *  Version 2.20
 *    Corrected VBUS detection for STM32F469xx and STM32F479xx devices
 *  Version 2.19
 *    Added timeout to endless loops
 *    Renamed USBD_MAX_ENDPOINT_NUM to USBD0_MAX_ENDPOINT_NUM
 *    Renamed OTG_RX_FIFO_SIZE to USBD0_RX_FIFO_SIZE
 *    Renamed OTG_TXx_FIFO_SIZE to USBD0_TXx_FIFO_SIZE (x = 0 .. 3)
 *  Version 2.18
 *    Removed CMSIS-RTOS dependency
 *  Version 2.17
 *    Minor changes not affecting functionality
 *  Version 2.16
 *    Corrected resume event signaling
 *  Version 2.15
 *    VBUS detection is selected automatically based on VBUS sensing pin
 *    setting (in RTE_Device.h or by STM32CubeMX)
 *  Version 2.14
 *    Corrected initial resume signaling after USB Bus Reset
 *    Corrected device status information
 *    VBUS sensing enabled by default
 *  Version 2.13
 *    Updated Isochronous transfer
 *  Version 2.12
 *    Updated IN Endpoint FIFO flush procedure
 *  Version 2.11
 *    Corrected PowerControl function for:
 *      - Unconditional Power Off
 *      - Conditional Power full (driver must be initialized)
 *  Version 2.10
 *    STM32CubeMX generated code can also be used to configure the driver.
 *  Version 2.9
 *    Removed global variable otg_fs_state
 *  Version 2.8
 *    PowerControl for Power OFF and Uninitialize functions made unconditional
 *  Version 2.7
 *    Corrected transferred size during transfer
 *  Version 2.6
 *    Corrected IN ZLP sending
 *  Version 2.5
 *    VBUS sensing disabled if USBD_VBUS_DETECT is not enabled
 *  Version 2.4
 *    Reorganized, common endpoint structure for IN and OUT endpoints
 *  Version 2.3
 *    Send data on first NAK to prevent simultaneous write to FIFO from library
 *    and from IRQ routine
 *  Version 2.2
 *    Removed unnecessary FIFO __packed attribute
 *  Version 2.1
 *    Update for USB Device CMSIS Driver API v2.01
 *  Version 2.0
 *    Updated to 2.00 API
 *  Version 1.4
 *    Multiple packet read
 *  Version 1.3
 *    Based on API V1.10 (namespace prefix ARM_ added)
 *  Version 1.2
 *    Removed include of rl_usb.h header
 *  Version 1.0
 *    Initial release
 */

/*! \page stm32f4_usbd_fs CMSIS-Driver USBD_FS Setup

The CMSIS-Driver USBD_FS requires:
  - Setup of USB clk to 48MHz
  - Configuration of USB_OTG_FS
 
Valid settings for various evaluation boards are listed in the table below:

Peripheral Resource | MCBSTM32F400       | STM32F4-Discovery  | 32F401C-Discovery  | 32F429I-Discovery
:-------------------|:-------------------|:-------------------|:-------------------|:------------------
USB_OTG_FS Mode     | <b>Device_only</b> | <b>Device_only</b> | <b>Device_only</b> | n/a
Activate_VBUS       | <b>enabled</b>     | <b>enabled</b>     | <b>enabled</b>     | n/a

For different boards, refer to the hardware schematics to reflect correct setup values.
 
The STM32CubeMX configuration for MCBSTM32F400 with steps for Pinout, Clock, and System Configuration are 
listed below. Enter the values that are marked \b bold.
 
Pinout tab
----------
  1. Configure USBD mode
     - Peripherals \b USB_OTG_FS: Mode=<b>Device_Only</b>
     - Peripherals \b USB_OTG_FS: Activate_VBUS=<b>enabled</b>
 
Clock Configuration tab
-----------------------
  1. Configure USB Clock: "48MHz clocks (MHz)": 48
 
Configuration tab
-----------------
  1. Under Connectivity open \b USB_OTG_FS Configuration:
     - DMA Settings: not used
     - <b>GPIO Settings</b>: review settings, no changes required
          Pin Name | Signal on Pin | GPIO mode | GPIO Pull-up/Pull..| Maximum out | User Label
          :--------|:--------------|:----------|:-------------------|:------------|:----------
          PA11     | USB_OTG_FS_DM | Alternate | No pull-up and no..| High        |.
          PA12     | USB_OTG_FS_DP | Alternate | No pull-up and no..| High        |.
     - <b>NVIC Settings</b>: enable interrupts
          Interrupt Table                      | Enable | Preemption Priority | Sub Priority
          :------------------------------------|:-------|:--------------------|:--------------
          USB On The Go FS global interrupt    |\b ON   | 0                   | 0
     - Parameter Settings: not used
     - User Constants: not used
     - Click \b OK to close the USB_OTG_FS Configuration dialog
*/

/*! \cond */

#include <stdint.h>
#include <string.h>

#include "Driver_USBD.h"

#include "OTG_FS_STM32F4xx.h"

#ifndef USBD0_MAX_ENDPOINT_NUM
#define USBD0_MAX_ENDPOINT_NUM         (3U)
#endif
#if    (USBD0_MAX_ENDPOINT_NUM > 3)
#error  Too many Endpoints, maximum IN/OUT Endpoint pairs that this driver supports is 3 !!!
#endif

#ifdef RTE_DEVICE_FRAMEWORK_CUBE_MX
#ifdef MX_USB_OTG_FS_DEVICE
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
#endif
#endif


// USBD Driver *****************************************************************

#define ARM_USBD_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(2,21)

// Driver Version
static const ARM_DRIVER_VERSION usbd_driver_version = { ARM_USBD_API_VERSION, ARM_USBD_DRV_VERSION };

// Driver Capabilities
static const ARM_USBD_CAPABILITIES usbd_driver_capabilities = {
#ifdef MX_USB_OTG_FS_VBUS_Pin
  1U,   // VBUS Detection
  1U,   // Event VBUS On
  1U    // Event VBUS Off
#else
  0U,   // VBUS Detection
  0U,   // Event VBUS On
  0U    // Event VBUS Off
#endif
#if (defined(ARM_USBD_API_VERSION) && (ARM_USBD_API_VERSION >= 0x202U))
, 0U    // Reserved
#endif
};

#define OTG                     (OTG_FS)

#define EP_NUM(ep_addr)         ((ep_addr) & ARM_USB_ENDPOINT_NUMBER_MASK)
#define EP_ID(ep_addr)          ((EP_NUM(ep_addr) * 2U) + (((ep_addr) >> 7) & 1U))

// FIFO sizes in bytes (total available memory for FIFOs is 1.25 kB)
#ifndef USBD0_RX_FIFO_SIZE
#define USBD0_RX_FIFO_SIZE      (640U)
#endif
#ifndef USBD0_TX0_FIFO_SIZE
#define USBD0_TX0_FIFO_SIZE     (160U)
#endif
#ifndef USBD0_TX1_FIFO_SIZE
#define USBD0_TX1_FIFO_SIZE     (160U)
#endif
#ifndef USBD0_TX2_FIFO_SIZE
#define USBD0_TX2_FIFO_SIZE     (160U)
#endif
#ifndef USBD0_TX3_FIFO_SIZE
#define USBD0_TX3_FIFO_SIZE     (160U)
#endif

#define OTG_TX_FIFO(n)          *((volatile uint32_t *)(OTG_FS_BASE + 0x1000U + (n * 0x1000U)))
#define OTG_RX_FIFO             *((volatile uint32_t *)(OTG_FS_BASE + 0x1000U))

#define OTG_DIEPTSIZ(ep_num)    *((volatile uint32_t *)(&OTG->DIEPTSIZ0 + (ep_num * 8U)))
#define OTG_DIEPCTL(ep_num)     *((volatile uint32_t *)(&OTG->DIEPCTL0  + (ep_num * 8U)))
#define OTG_DTXFSTS(ep_num)     *((volatile uint32_t *)(&OTG->DTXFSTS0  + (ep_num * 8U)))
#define OTG_DOEPTSIZ(ep_num)    *((volatile uint32_t *)(&OTG->DOEPTSIZ0 + (ep_num * 8U)))
#define OTG_DOEPCTL(ep_num)     *((volatile uint32_t *)(&OTG->DOEPCTL0  + (ep_num * 8U)))
#define OTG_DIEPINT(ep_num)     *((volatile uint32_t *)(&OTG->DIEPINT0  + (ep_num * 8U)))
#define OTG_DOEPINT(ep_num)     *((volatile uint32_t *)(&OTG->DOEPINT0  + (ep_num * 8U)))

#define OTG_EP_IN_TYPE(ep_num)  ((OTG_DIEPCTL(ep_num) >> 18) & 3U)
#define OTG_EP_OUT_TYPE(ep_num) ((OTG_DOEPCTL(ep_num) >> 18) & 3U)

static const uint16_t USBD0_TX_FIFO_SIZE[4] = { 
  USBD0_TX0_FIFO_SIZE, 
  USBD0_TX1_FIFO_SIZE, 
  USBD0_TX2_FIFO_SIZE, 
  USBD0_TX3_FIFO_SIZE 
};

typedef struct {                        // Endpoint structure definition
  uint8_t  *data;
  uint32_t  num;
  uint32_t  num_transferred_total;
  uint16_t  num_transferring;
  uint16_t  max_packet_size;
  uint8_t   active;
  uint8_t   in_nak;
  uint8_t   in_zlp;
  uint8_t   in_flush;
} ENDPOINT_t;

static ARM_USBD_SignalDeviceEvent_t   SignalDeviceEvent;
static ARM_USBD_SignalEndpointEvent_t SignalEndpointEvent;

static bool                hw_powered     = false;
static bool                hw_initialized = false;
static ARM_USBD_STATE      usbd_state;
static uint32_t            setup_packet[2];     // Setup packet data
static volatile uint8_t    setup_received;      // Setup packet received

// Endpoints runtime information
static volatile ENDPOINT_t ep[(USBD0_MAX_ENDPOINT_NUM + 1U) * 2U];

// Function prototypes
static uint16_t USBD_GetFrameNumber (void);


// Auxiliary functions

/**
  \fn          void USBD_FlushInEpFifo (uint8_t FIFO_num)
  \brief       Flush IN Endpoint FIFO
  \param[in]   FIFO_num  IN Endpoint FIFO number
                - FIFO_num.0..3: IN Endpoint FIFO to Flush
                - FIFO_num.4:    All IN Endpoint FIFOs to Flush
*/
static void USBD_FlushInEpFifo (uint8_t FIFO_num) {
  uint32_t tout_cnt;

  // Wait, with timeout, for flush to end
  tout_cnt = SystemCoreClock / 64U;
  while ((OTG->GRSTCTL & OTG_FS_GRSTCTL_TXFFLSH) != 0U) {
    if (tout_cnt-- == 0U) { return; }
  }

  OTG->GRSTCTL  = (OTG->GRSTCTL & ~OTG_FS_GRSTCTL_TXFNUM_MSK) | OTG_FS_GRSTCTL_TXFNUM(FIFO_num);
  OTG->GRSTCTL |= OTG_FS_GRSTCTL_TXFFLSH;

  // Wait, with timeout, for flush to end
  tout_cnt = SystemCoreClock / 64U;
  while ((OTG->GRSTCTL & OTG_FS_GRSTCTL_TXFFLSH) != 0U) {
    if (tout_cnt-- == 0U) { return; }
  }
}

/**
  \fn          void USBD_Reset (void)
  \brief       Reset USB Endpoint settings and variables.
*/
static void USBD_Reset (void) {
  uint8_t  i;
  uint32_t epctl;

  // Reset global variables
  setup_packet[0] = 0U;
  setup_packet[1] = 0U;
  setup_received  = 0U;
  memset((void *)((uint32_t)ep), 0U, sizeof(ep));

  // Clear Endpoint mask registers
  OTG->DOEPMSK = 0U;
  OTG->DIEPMSK = 0U;

  for (i = 1U; i <= USBD0_MAX_ENDPOINT_NUM; i++) {
    // Endpoint set NAK
    epctl = OTG_FS_DOEPCTLx_SNAK;
    if ((OTG_DOEPCTL(i) & OTG_FS_DOEPCTLx_EPENA) != 0U) {
      // Disable enabled Endpoint
      epctl |= OTG_FS_DOEPCTLx_EPDIS;
    }
    OTG_DOEPCTL(i) = epctl;

    // Endpoint set NAK
    epctl = OTG_FS_DIEPCTLx_SNAK;
    if ((OTG_DIEPCTL(i) & OTG_FS_DIEPCTLx_EPENA) != 0U) {
      // Disable enabled Endpoint
      epctl |= OTG_FS_DIEPCTLx_EPDIS;
    }
    OTG_DIEPCTL(i) = epctl;

    // Clear IN Endpoint interrupts
    OTG_DIEPINT(i) = OTG_FS_DIEPINTx_XFCR    |
                     OTG_FS_DIEPINTx_EPDISD  |
                     OTG_FS_DIEPINTx_TOC     |
                     OTG_FS_DIEPINTx_ITTXFE  |
                     OTG_FS_DIEPINTx_INEPNE  |
                     OTG_FS_DIEPINTx_TXFE    ;

    // Clear OUT Endpoint interrupts
    OTG_DOEPINT(i) = OTG_FS_DOEPINTx_XFCR    |
                     OTG_FS_DOEPINTx_EPDISD  |
                     OTG_FS_DOEPINTx_STUP    |
                     OTG_FS_DOEPINTx_OTEPDIS |
                     OTG_FS_DOEPINTx_B2BSTUP ;
  }

  // Flush all IN Endpoint FIFOs
  USBD_FlushInEpFifo (0x10U);

  // Set device address to 0
  OTG->DCFG       = (OTG->DCFG & ~OTG_FS_DCFG_DAD_MSK);
  OTG->DAINTMSK   =  OTG_FS_DAINT_IEPINT(0U) |  // Enable IN Endpoint0 interrupt
                     OTG_FS_DAINT_OEPINT(0U);   // Enable OUT Endpoint0 interrupt

  // Enable Setup phase done, OUT Endpoint disabled and OUT transfer complete interrupt
  OTG->DOEPMSK    =  OTG_FS_DOEPMSK_STUPM    |
                     OTG_FS_DOEPMSK_EPDM     |
                     OTG_FS_DOEPMSK_XFRCM    ;

  // Enable In Endpoint disable and IN transfer complete interrupt
  OTG->DIEPMSK    =  OTG_FS_DIEPMSK_EPDM     |
                     OTG_FS_DIEPMSK_XFRCM    ;

  // Configure FIFOs
  OTG->GRXFSIZ    =   USBD0_RX_FIFO_SIZE  / 4U;
  OTG->DIEPTXF0   =  (USBD0_RX_FIFO_SIZE  / 4U) |
                    ((USBD0_TX0_FIFO_SIZE / 4U) << OTG_FS_DIEPTXFx_INEPTXFD_POS);

  OTG->DIEPTXF1   = ((USBD0_RX_FIFO_SIZE  + USBD0_TX0_FIFO_SIZE) / 4U) |
                    ((USBD0_TX1_FIFO_SIZE / 4U) << OTG_FS_DIEPTXFx_INEPTXFD_POS);

  OTG->DIEPTXF2   = ((USBD0_RX_FIFO_SIZE  + USBD0_TX0_FIFO_SIZE + USBD0_TX1_FIFO_SIZE) / 4U) |
                    ((USBD0_TX2_FIFO_SIZE / 4U) << OTG_FS_DIEPTXFx_INEPTXFD_POS);

  OTG->DIEPTXF3   = ((USBD0_RX_FIFO_SIZE  + USBD0_TX0_FIFO_SIZE + USBD0_TX1_FIFO_SIZE +
                      USBD0_TX2_FIFO_SIZE)/ 4U) |
                    ((USBD0_TX3_FIFO_SIZE / 4U) << OTG_FS_DIEPTXFx_INEPTXFD_POS);
}

/**
  \fn          void USBD_EndpointReadSet (uint8_t ep_addr)
  \brief       Set Endpoint for next read.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
*/
static void USBD_EndpointReadSet (uint8_t ep_addr) {
  volatile ENDPOINT_t *ptr_ep;
  uint16_t             num;
  uint8_t              ep_num;

  ptr_ep = &ep[EP_ID(ep_addr)];
  ep_num = EP_NUM(ep_addr);

  // Set packet count and transfer size
  if  (ptr_ep->num > ptr_ep->max_packet_size) { num = ptr_ep->max_packet_size; }
  else                                        { num = (uint16_t)ptr_ep->num;   }

  if (ep_num != 0U) {
    OTG_DOEPTSIZ(ep_num) = (1U << OTG_FS_DOEPTSIZx_PKTCNT_POS ) |
                            num                                 ;
  } else {
    OTG_DOEPTSIZ(0U)     = (1U << OTG_FS_DOEPTSIZx_PKTCNT_POS ) |
                           (3U << OTG_FS_DOEPTSIZ0_STUPCNT_POS) |
                            num                                 ;
  }
  // Set correct frame for Isochronous Endpoint
  if (OTG_EP_OUT_TYPE(ep_num) == ARM_USB_ENDPOINT_ISOCHRONOUS) {
    if ((USBD_GetFrameNumber() & 1U) != 0U) { OTG_DOEPCTL(ep_num) |= OTG_FS_DOEPCTLx_SEVNFRM; }
    else                                    { OTG_DOEPCTL(ep_num) |= OTG_FS_DOEPCTLx_SODDFRM; }
  }

  // Clear NAK and enable Endpoint
  OTG_DOEPCTL(ep_num) |= OTG_FS_DOEPCTLx_EPENA | OTG_FS_DOEPCTLx_CNAK;
}

/**
  \fn          int32_t USBD_ReadFromFifo (uint8_t ep_addr, uint16_t num)
  \brief       Read data from USB Endpoint.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \param[in]   num      number of data bytes to read
  \return      number of data bytes read
*/
static int32_t USBD_ReadFromFifo (uint8_t ep_addr, uint16_t num) {
  volatile ENDPOINT_t *ptr_ep;
  volatile uint32_t   *ptr_src;
  uint8_t             *ptr_dest;
  uint32_t             i, residue;
  uint8_t              ep_num;
  uint8_t              tmp_buf[4];

  ptr_ep = &ep[EP_ID(ep_addr)];
  ep_num = EP_NUM(ep_addr);

  // Check if Endpoint is activated and buffer available
  if ((OTG_DOEPCTL(ep_num) & OTG_FS_DOEPCTLx_USBAEP) == 0U) { return 0U; }
  if (ptr_ep->data == 0U)                                   { return 0U; }

  if (num > ptr_ep->num) { num = (uint16_t)ptr_ep->num; }

  // If Isochronous Endpoint
  if (OTG_EP_OUT_TYPE(ep_num) == ARM_USB_ENDPOINT_ISOCHRONOUS) {
    // If DATA PID = DATA0 and number of packets in
    // which this payload was received = 1, then data is valid
    if ((OTG_DOEPTSIZ(ep_num) & (OTG_FS_DOEPTSIZx_PKTCNT_MSK | OTG_FS_DOEPTSIZx_RXDPID_MSK)) != 0) { num = 0U; }
  }

  // Copy data from FIFO
  ptr_src  = (volatile uint32_t *)(OTG_FS_BASE + 0x1000U);
  ptr_dest = (         uint8_t  *)((uint32_t)ptr_ep->data + ptr_ep->num_transferred_total);
  i        = num / 4U;
  while (i != 0U) {
    __UNALIGNED_UINT32_WRITE(ptr_dest, *ptr_src);
    ptr_dest += 4U;
    i--;
  }
  ptr_ep->num_transferred_total += num;

  // If data size is not equal n*4
  residue = num % 4U;
  if (residue != 0U) {
    __UNALIGNED_UINT32_WRITE(tmp_buf, OTG_RX_FIFO);
    for (i = 0U; i < residue; i++) {
      *ptr_dest++ = tmp_buf[i];
    }
  }

  if (num != ptr_ep->max_packet_size) { ptr_ep->num  = 0U;  }
  else                                { ptr_ep->num -= num; }

  return num;
}

/**
  \fn          void USBD_WriteToFifo (uint8_t ep_addr)
  \brief       Write data to Endpoint FIFO.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
*/
static void USBD_WriteToFifo (uint8_t ep_addr) {
  volatile ENDPOINT_t *ptr_ep;
  volatile uint32_t   *ptr_dest;
  uint8_t             *ptr_src;
  uint16_t             num, i;
  uint8_t              ep_num;

  ptr_ep = &ep[EP_ID(ep_addr)];
  ep_num = EP_NUM(ep_addr);

  if (ptr_ep->num > ptr_ep->max_packet_size) { num = ptr_ep->max_packet_size; }
  else                                       { num = (uint16_t)ptr_ep->num;   }

  // Check if enough space in FIFO
  if ((OTG_DTXFSTS(ep_num) * 4U) < num) { return; }

  // Set transfer size and packet count
  OTG_DIEPTSIZ(ep_num) = (1U << OTG_FS_DIEPTSIZx_PKTCNT_POS) |
                         (1U << OTG_FS_DIEPTSIZx_MCNT_POS)   |
                          num                                ;

  // Set correct frame for Isochronous Endpoint
  if (OTG_EP_IN_TYPE(ep_num) == ARM_USB_ENDPOINT_ISOCHRONOUS) {
    if ((USBD_GetFrameNumber() & 1U) != 0U) { OTG_DIEPCTL(ep_num) |= OTG_FS_DIEPCTLx_SEVNFRM; }
    else                                    { OTG_DIEPCTL(ep_num) |= OTG_FS_DIEPCTLx_SODDFRM; }
  }

  // Enable Endpoint and clear NAK
  OTG_DIEPCTL(ep_num) |= OTG_FS_DIEPCTLx_EPENA | OTG_FS_DIEPCTLx_CNAK;

  ptr_src  = (         uint8_t  *)((uint32_t)ptr_ep->data + ptr_ep->num_transferred_total);
  ptr_dest = (volatile uint32_t *)(OTG_FS_BASE + 0x1000U + (ep_num * 0x1000U));

  ptr_ep->num_transferring  = num;
  ptr_ep->num              -= num;
  ptr_ep->in_zlp            = 0U;

  // Copy data to FIFO
  i = (num + 3U) >> 2;
  while (i != 0U) {
    *ptr_dest = __UNALIGNED_UINT32_READ(ptr_src);
    ptr_src += 4U;
    i--;
  }
}


// USBD Driver functions

/**
  \fn          ARM_DRIVER_VERSION USBD_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION USBD_GetVersion (void) { return usbd_driver_version; }

/**
  \fn          ARM_USBD_CAPABILITIES USBD_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_USBD_CAPABILITIES
*/
static ARM_USBD_CAPABILITIES USBD_GetCapabilities (void) { return usbd_driver_capabilities; }

/**
  \fn          int32_t USBD_Initialize (ARM_USBD_SignalDeviceEvent_t   cb_device_event,
                                        ARM_USBD_SignalEndpointEvent_t cb_endpoint_event)
  \brief       Initialize USB Device Interface.
  \param[in]   cb_device_event    Pointer to \ref ARM_USBD_SignalDeviceEvent
  \param[in]   cb_endpoint_event  Pointer to \ref ARM_USBD_SignalEndpointEvent
  \return      \ref execution_status
*/
static int32_t USBD_Initialize (ARM_USBD_SignalDeviceEvent_t   cb_device_event,
                                ARM_USBD_SignalEndpointEvent_t cb_endpoint_event) {

  if (hw_initialized == true) {
    return ARM_DRIVER_OK;
  }

  SignalDeviceEvent   = cb_device_event;
  SignalEndpointEvent = cb_endpoint_event;

  otg_fs_role = ARM_USB_ROLE_DEVICE;
#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
  OTG_FS_PinsConfigure (ARM_USB_PIN_DP | ARM_USB_PIN_DM
#ifdef MX_USB_OTG_FS_VBUS_Pin
                      | ARM_USB_PIN_VBUS
#endif
                       );
#endif

#ifdef RTE_DEVICE_FRAMEWORK_CUBE_MX
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
#endif

  hw_initialized = true;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_Uninitialize (void)
  \brief       De-initialize USB Device Interface.
  \return      \ref execution_status
*/
static int32_t USBD_Uninitialize (void) {

#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
  OTG_FS_PinsUnconfigure (ARM_USB_PIN_DP | ARM_USB_PIN_DM
#ifdef MX_USB_OTG_FS_VBUS_Pin
                        | ARM_USB_PIN_VBUS
#endif
                         );
#else
  hpcd_USB_OTG_FS.Instance = NULL;
#endif

  otg_fs_role = ARM_USB_ROLE_NONE;

  hw_initialized = false;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_PowerControl (ARM_POWER_STATE state)
  \brief       Control USB Device Interface Power.
  \param[in]   state  Power state
  \return      \ref execution_status
*/
static int32_t USBD_PowerControl (ARM_POWER_STATE state) {
  uint32_t tout_ms;

  if ((state != ARM_POWER_OFF)  && 
      (state != ARM_POWER_FULL) && 
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  switch (state) {
    case ARM_POWER_OFF:
      RCC->AHB2ENR  |=  RCC_AHB2ENR_OTGFSEN;            // OTG FS clock enable
#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
      NVIC_DisableIRQ      (OTG_FS_IRQn);               // Disable interrupt
      NVIC_ClearPendingIRQ (OTG_FS_IRQn);               // Clear pending interrupt
#endif
      hw_powered     =  false;                          // Clear powered flag
      OTG->DCTL     |=  OTG_FS_DCTL_SDIS;               // Soft disconnect enabled
      OTG->GAHBCFG  &= ~OTG_FS_GAHBCFG_GINTMSK;         // Disable USB interrupts
      RCC->AHB2RSTR |=  RCC_AHB2RSTR_OTGFSRST;          // Reset OTG FS module
                                                        // Reset variables
      setup_received =  0U;
      memset((void *)((uint32_t)&usbd_state), 0, sizeof(usbd_state));
      memset((void *)((uint32_t)ep),          0, sizeof(ep));

      OTG->GCCFG    &= ~OTG_FS_GCCFG_PWRDWN;            // Power down on-chip PHY
      OTG->PCGCCTL  |=  OTG_FS_PCGCCTL_STPPCLK;         // Stop PHY clock
      OTG->GCCFG     =  0U;                             // Reset core configuration

#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
      RCC->AHB2ENR  &= ~RCC_AHB2ENR_OTGFSEN;            // Disable OTG FS clock
#else
      if (hpcd_USB_OTG_FS.Instance != NULL) {
        HAL_PCD_MspDeInit(&hpcd_USB_OTG_FS);
      }
#endif
      break;

    case ARM_POWER_FULL:
      if (hw_initialized == false) {
        return ARM_DRIVER_ERROR;
      }
      if (hw_powered     == true) {
        return ARM_DRIVER_OK;
      }
#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
      RCC->AHB2ENR  |=  RCC_AHB2ENR_OTGFSEN;            // OTG FS clock enable
#else
      HAL_PCD_MspInit(&hpcd_USB_OTG_FS);
#endif
      RCC->AHB2RSTR |=  RCC_AHB2RSTR_OTGFSRST;          // Reset OTG FS module
      HAL_Delay(1U);
      RCC->AHB2RSTR &= ~RCC_AHB2RSTR_OTGFSRST;          // Clear reset of OTG FS module
      HAL_Delay(1U);

      OTG->GCCFG    |=  OTG_FS_GCCFG_PWRDWN;            // Power up on-chip PHY
      OTG->GUSBCFG  |=  OTG_FS_GUSBCFG_PHYSEL;          // Full-speed transceiver

      // Wait until AHB Master state machine is in the idle condition, max 100 ms
      tout_ms = 100U;
      while ((OTG->GRSTCTL & OTG_FS_GRSTCTL_AHBIDL) == 0U) {
        if (tout_ms == 0U) {
          return ARM_DRIVER_ERROR_TIMEOUT;
        }
        tout_ms--;
        HAL_Delay(1U);
      }

      OTG->GRSTCTL  |=  OTG_FS_GRSTCTL_CSRST;           // Core soft reset

      // Wait Core reset to finish, max 100 ms
      tout_ms = 100U;
      while ((OTG->GRSTCTL & OTG_FS_GRSTCTL_CSRST) != 0U) {
        if (tout_ms == 0U) {
          return ARM_DRIVER_ERROR_TIMEOUT;
        }
        tout_ms--;
        HAL_Delay(1U);
      }
      HAL_Delay(1U);

      // Wait until AHB Master state machine is in the idle condition, max 100 ms
      tout_ms = 100U;
      while ((OTG->GRSTCTL & OTG_FS_GRSTCTL_AHBIDL) == 0U) {
        if (tout_ms == 0U) {
          return ARM_DRIVER_ERROR_TIMEOUT;
        }
        tout_ms--;
        HAL_Delay(1U);
      }

      OTG->DCTL     |=  OTG_FS_DCTL_SDIS;               // Soft disconnect enabled

      // Set turnaround time
      OTG->GUSBCFG   = ((OTG->GUSBCFG & ~OTG_FS_GUSBCFG_TRDT_MSK) |
                         OTG_FS_GUSBCFG_TRDT(15U))                ;
      if (((OTG->GUSBCFG & OTG_FS_GUSBCFG_FDMOD) == 0U) || ((OTG->GUSBCFG & OTG_FS_GUSBCFG_FHMOD) != 0U)) {
        OTG->GUSBCFG &= ~OTG_FS_GUSBCFG_FHMOD;          // Clear force host mode
        OTG->GUSBCFG |=  OTG_FS_GUSBCFG_FDMOD;          // Force device mode
        HAL_Delay(50U);
      }

      USBD_Reset ();                                    // Reset variables and endpoint settings

#ifdef MX_USB_OTG_FS_VBUS_Pin
#ifdef USB_OTG_GCCFG_VBDEN
      OTG->GCCFG    |=  USB_OTG_GCCFG_VBDEN;            // Enable  VBUS sensing (on STM32F469xx and STM32F479xx devices)
#else
      OTG->GCCFG    |=  OTG_FS_GCCFG_VBUSBSEN;          // Enable  VBUS sensing device "B"
#endif
#else
#ifdef USB_OTG_GCCFG_VBDEN
      OTG->GOTGCTL  |=  USB_OTG_GOTGCTL_BVALOVAL;       // B-peripheral session valid override value 1
      OTG->GOTGCTL  |=  USB_OTG_GOTGCTL_BVALOEN;        // B-peripheral session valid override enabled
      OTG->GCCFG    &= ~USB_OTG_GCCFG_VBDEN;            // Disable VBUS sensing (on STM32F469xx and STM32F479xx devices)
#else
      OTG->GCCFG    |=  OTG_FS_GCCFG_NOVBUSSENS;        // Disable VBUS sensing
#endif
#endif

      OTG->DCFG     |=  OTG_FS_DCFG_DSPD_MSK;           // Full Speed

      OTG->GINTMSK   = (OTG_FS_GINTMSK_USBSUSPM |       // Unmask interrupts
                        OTG_FS_GINTMSK_USBRST   |
                        OTG_FS_GINTMSK_ENUMDNEM |
                        OTG_FS_GINTMSK_RXFLVLM  |
                        OTG_FS_GINTMSK_IEPINT   |
                        OTG_FS_GINTMSK_OEPINT   |
#ifdef MX_USB_OTG_FS_VBUS_Pin
                        OTG_FS_GINTMSK_SRQIM    |
                        OTG_FS_GINTMSK_OTGINT   |
#endif
                        OTG_FS_GINTMSK_WUIM)    ;

      OTG->GAHBCFG  |= (OTG_FS_GAHBCFG_GINTMSK  |       // Enable interrupts
                        OTG_FS_GAHBCFG_TXFELVL) ;

      hw_powered     = true;                            // Set powered flag
#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
      NVIC_EnableIRQ   (OTG_FS_IRQn);                   // Enable interrupt
#endif
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_DeviceConnect (void)
  \brief       Connect USB Device.
  \return      \ref execution_status
*/
static int32_t USBD_DeviceConnect (void) {

  if (hw_powered == false) { return ARM_DRIVER_ERROR; }

  OTG->DCTL  &= ~OTG_FS_DCTL_SDIS;      // Soft disconnect disabled
  OTG->GCCFG |=  OTG_FS_GCCFG_PWRDWN;   // Power up on-chip PHY

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_DeviceDisconnect (void)
  \brief       Disconnect USB Device.
  \return      \ref execution_status
*/
static int32_t USBD_DeviceDisconnect (void) {

  if (hw_powered == false) { return ARM_DRIVER_ERROR; }

  OTG->DCTL  |=  OTG_FS_DCTL_SDIS;      // Soft disconnect enabled
  OTG->GCCFG &= ~OTG_FS_GCCFG_PWRDWN;   // Power down on-chip PHY

  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_USBD_STATE USBD_DeviceGetState (void)
  \brief       Get current USB Device State.
  \return      Device State \ref ARM_USBD_STATE
*/
static ARM_USBD_STATE USBD_DeviceGetState (void) {
  return usbd_state;
}

/**
  \fn          int32_t USBD_DeviceRemoteWakeup (void)
  \brief       Trigger USB Remote Wakeup.
  \return      \ref execution_status
*/
static int32_t USBD_DeviceRemoteWakeup (void) {

  if (hw_powered == false) { return ARM_DRIVER_ERROR; }

  OTG->DCTL |=   OTG_FS_DCTL_RWUSIG;    // Remote wakeup signaling
  HAL_Delay(5U);
  OTG->DCTL &=  ~OTG_FS_DCTL_RWUSIG;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_DeviceSetAddress (uint8_t dev_addr)
  \brief       Set USB Device Address.
  \param[in]   dev_addr  Device Address
  \return      \ref execution_status
*/
static int32_t USBD_DeviceSetAddress (uint8_t dev_addr) {

  if (hw_powered == false) { return ARM_DRIVER_ERROR; }

  OTG->DCFG = (OTG->DCFG & ~OTG_FS_DCFG_DAD_MSK) | OTG_FS_DCFG_DAD(dev_addr);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_ReadSetupPacket (uint8_t *setup)
  \brief       Read setup packet received over Control Endpoint.
  \param[out]  setup  Pointer to buffer for setup packet
  \return      \ref execution_status
*/
static int32_t USBD_ReadSetupPacket (uint8_t *setup) {

  if (hw_powered == false)  { return ARM_DRIVER_ERROR; }
  if (setup_received == 0U) { return ARM_DRIVER_ERROR; }

  setup_received = 0U;
  memcpy(setup, setup_packet, 8);

  if (setup_received != 0U) {           // If new setup packet was received while this was being read
    return ARM_DRIVER_ERROR;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_EndpointConfigure (uint8_t  ep_addr,
                                               uint8_t  ep_type,
                                               uint16_t ep_max_packet_size)
  \brief       Configure USB Endpoint.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \param[in]   ep_type  Endpoint Type (ARM_USB_ENDPOINT_xxx)
  \param[in]   ep_max_packet_size Endpoint Maximum Packet Size
  \return      \ref execution_status
*/
static int32_t USBD_EndpointConfigure (uint8_t  ep_addr,
                                       uint8_t  ep_type,
                                       uint16_t ep_max_packet_size) {
  volatile ENDPOINT_t *ptr_ep;
  uint8_t              ep_num;
  uint16_t             ep_mps;
  bool                 ep_dir;

  ep_num = EP_NUM(ep_addr);
  ep_dir = (ep_addr & ARM_USB_ENDPOINT_DIRECTION_MASK) == ARM_USB_ENDPOINT_DIRECTION_MASK;
  ep_mps =  ep_max_packet_size & ARM_USB_ENDPOINT_MAX_PACKET_SIZE_MASK;

  if (ep_num > USBD0_MAX_ENDPOINT_NUM) { return ARM_DRIVER_ERROR_PARAMETER; }
  if (ep_dir) {                        // IN endpoint
    if (ep_mps > USBD0_TX_FIFO_SIZE[ep_num])                                { return ARM_DRIVER_ERROR_PARAMETER; }
  } else {                             // OUT endpoint
    if ((ep_mps + 60U + (8 * USBD0_MAX_ENDPOINT_NUM)) > USBD0_RX_FIFO_SIZE) { return ARM_DRIVER_ERROR_PARAMETER; }
  }
  if (hw_powered == false)             { return ARM_DRIVER_ERROR; }

  ptr_ep = &ep[EP_ID(ep_addr)];
  if (ptr_ep->active != 0U)            { return ARM_DRIVER_ERROR_BUSY; }

  // Clear Endpoint transfer and configuration information
  memset((void *)((uint32_t)ptr_ep), 0, sizeof (ENDPOINT_t));

  // Set maximum packet size to requested
  ptr_ep->max_packet_size = ep_mps;

  if (ep_dir != 0U) {                                   // IN Endpoint
    ptr_ep->in_nak = 0U;                                // Clear IN Endpoint NAK flag

    // Configure IN Endpoint
    OTG_DIEPCTL(ep_num) = ((uint32_t)ep_num  <<  OTG_FS_DIEPCTLx_TXFNUM_POS) |  // FIFO Number
                          ((uint32_t)ep_type <<  OTG_FS_DIEPCTLx_EPTYP_POS ) |  // Endpoint Type
                           ep_mps                                            ;  // Max Packet Size

    if ((ep_type == ARM_USB_ENDPOINT_BULK) || (ep_type == ARM_USB_ENDPOINT_INTERRUPT)) {
      // Set DATA0 PID for Interrupt or Bulk Endpoint
      OTG_DIEPCTL(ep_num) |= OTG_FS_DIEPCTLx_SD0PID;
    }

    OTG_DIEPCTL(ep_num)   |= OTG_FS_DIEPCTLx_USBAEP;    // Activate Endpoint

    if ((OTG_DIEPCTL(ep_num) & OTG_FS_DIEPCTLx_EPENA) != 0U) {
      OTG_DIEPCTL(ep_num) |= OTG_FS_DIEPCTLx_EPDIS;     // Disable Endpoint
    }

    OTG->DAINTMSK |= (1U << ep_num);                    // Enable Endpoint interrupt

  } else {                                              // OUT Endpoint

    // Configure OUT Endpoint
    OTG_DOEPCTL(ep_num) = ((uint32_t)ep_type <<  OTG_FS_DOEPCTLx_EPTYP_POS) |   // Endpoint Type
                           OTG_FS_DOEPCTLx_SNAK                             |   // Set NAK
                           ep_mps                                           ;   // Max Packet Size

    if ((ep_type == ARM_USB_ENDPOINT_BULK) || (ep_type == ARM_USB_ENDPOINT_INTERRUPT)) {
      // Set DATA0 PID for Interrupt or Bulk Endpoint
      OTG_DOEPCTL(ep_num) |= OTG_FS_DOEPCTLx_SD0PID;
    }

    OTG_DOEPCTL(ep_num) |= OTG_FS_DOEPCTLx_USBAEP;      // Activate Endpoint

    OTG->DAINTMSK |= (1U << (ep_num + 16));             // Enable Endpoint interrupt
  }

  if (ep_type == ARM_USB_ENDPOINT_ISOCHRONOUS) {
    // Enable End of Periodic Frame Interrupt for Isochronous Endpoint
    OTG->GINTMSK |= OTG_FS_GINTMSK_EOPFM;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_EndpointUnconfigure (uint8_t ep_addr)
  \brief       Unconfigure USB Endpoint.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \return      \ref execution_status
*/
static int32_t USBD_EndpointUnconfigure (uint8_t ep_addr) {
  volatile ENDPOINT_t *ptr_ep;
  uint32_t             tout_cnt;
  uint8_t              ep_num, i, IsoEpEnCnt;
  bool                 ep_dir;

  ep_num = EP_NUM(ep_addr);
  if (ep_num > USBD0_MAX_ENDPOINT_NUM) { return ARM_DRIVER_ERROR_PARAMETER; }
  if (hw_powered == false)             { return ARM_DRIVER_ERROR; }

  ptr_ep = &ep[EP_ID(ep_addr)];
  if (ptr_ep->active != 0U)            { return ARM_DRIVER_ERROR_BUSY; }

  ep_dir = (ep_addr & ARM_USB_ENDPOINT_DIRECTION_MASK) == ARM_USB_ENDPOINT_DIRECTION_MASK;

  OTG->DAINTMSK &= ~(1U << (ep_num + ((ep_dir ^ 1U) << 4)));    // Disable Endpoint interrupt

  // Clear Endpoint transfer and configuration information
  memset((void *)((uint32_t)ptr_ep), 0, sizeof (ENDPOINT_t));

  IsoEpEnCnt = 0U;

  // Count Active Isochronous OUT and IN Endpoints
  if ((OTG_EP_OUT_TYPE(ep_num) == ARM_USB_ENDPOINT_ISOCHRONOUS) ||
      (OTG_EP_IN_TYPE (ep_num) == ARM_USB_ENDPOINT_ISOCHRONOUS)) {
    for (i = 1U; i <= USBD0_MAX_ENDPOINT_NUM; i++) {
      if ((OTG_DOEPCTL(i) & OTG_FS_DOEPCTLx_USBAEP) != 0U) {
        if (OTG_EP_OUT_TYPE(i) == ARM_USB_ENDPOINT_ISOCHRONOUS) {
          IsoEpEnCnt++;
        }
      }
      if ((OTG_DIEPCTL(i) & OTG_FS_DIEPCTLx_USBAEP) != 0U) {
        if (OTG_EP_IN_TYPE(i) == ARM_USB_ENDPOINT_ISOCHRONOUS) {
          IsoEpEnCnt++;
        }
      }
    }

    // If Last Active Isochronous OUT or IN Endpoint, Disable EOPF
    if (IsoEpEnCnt == 1U) { OTG->GINTMSK &= ~OTG_FS_GINTMSK_EOPFM; }
  }

  // IN Endpoint
  if (ep_dir != 0U) {
    // Disable Enabled IN Endpoint
    if ((OTG_DIEPCTL(ep_num) & OTG_FS_DIEPCTLx_EPENA) != 0U) {
      OTG_DIEPCTL(ep_num)  |=  OTG_FS_DIEPCTLx_EPDIS;
    }

    OTG_DIEPCTL(ep_num)    |=  OTG_FS_DIEPCTLx_SNAK;    // Set Endpoint NAK

    // Deactivate Endpoint
    if (ep_num != 0U) { OTG_DIEPCTL(ep_num)  &= ~OTG_FS_DIEPCTLx_USBAEP; }

  } else {
  // OUT Endpoint

    OTG->DCTL |= OTG_FS_DCTL_SGONAK;                    // Set Global OUT NAK

    // Wait, with timeout, while Global OUT NAK effective is not set
    tout_cnt = SystemCoreClock / 64U;
    while ((OTG->GINTSTS & OTG_FS_GINTSTS_GONAKEFF) == 0U) {
      if (tout_cnt-- == 0U) { return ARM_DRIVER_ERROR; }
    }

    OTG_DOEPCTL(ep_num) |= OTG_FS_DOEPCTLx_SNAK;        // Set Endpoint NAK

    if (ep_num != 0U) {
      // Disable Enabled OUT Endpoint
      if ((OTG_DOEPCTL(ep_num) & OTG_FS_DOEPCTLx_EPENA) != 0U) {
        OTG_DOEPCTL(ep_num)  |=  OTG_FS_DOEPCTLx_EPDIS;

        // Wait, with timeout, while Endpoint disabled interrupt is not set
        tout_cnt = SystemCoreClock / 64U;
        while ((OTG_DOEPINT(ep_num) & OTG_FS_DOEPINTx_EPDISD) == 0U) {
          if (tout_cnt-- == 0U) { return ARM_DRIVER_ERROR; }
        }
      }
      OTG_DOEPCTL(ep_num) &= ~OTG_FS_DOEPCTLx_USBAEP;   // Deactivate Endpoint
    }

    OTG->DCTL |= OTG_FS_DCTL_CGONAK;                    // Clear Global OUT NAK
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_EndpointStall (uint8_t ep_addr, bool stall)
  \brief       Set/Clear Stall for USB Endpoint.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \param[in]   stall  Operation
                - \b false Clear
                - \b true Set
  \return      \ref execution_status
*/
static int32_t USBD_EndpointStall (uint8_t ep_addr, bool stall) {
  volatile ENDPOINT_t *ptr_ep;
  uint32_t             tout_cnt;
  uint8_t              ep_num;
  bool                 ep_dir;

  ep_num = EP_NUM(ep_addr);
  if (ep_num > USBD0_MAX_ENDPOINT_NUM) { return ARM_DRIVER_ERROR_PARAMETER; }
  if (hw_powered == false)             { return ARM_DRIVER_ERROR; }

  ptr_ep = &ep[EP_ID(ep_addr)];
  if (ptr_ep->active != 0U)            { return ARM_DRIVER_ERROR_BUSY; }

  ep_dir = (ep_addr & ARM_USB_ENDPOINT_DIRECTION_MASK) == ARM_USB_ENDPOINT_DIRECTION_MASK;

  if (stall != 0U) {                                    // Activate STALL
    if (ep_dir != 0U) {                                 // IN Endpoint
      if ((OTG_DIEPCTL(ep_num) & OTG_FS_DIEPCTLx_EPENA) != 0U) {
        // Set flush flag to Flush IN FIFO in Endpoint disabled interrupt
        ptr_ep->in_flush = 1U;
        OTG_DIEPCTL(ep_num)  |= OTG_FS_DIEPCTLx_STALL | OTG_FS_DIEPCTLx_EPDIS;
      } else {
        OTG_DIEPCTL(ep_num)  |= OTG_FS_DIEPCTLx_STALL;
        USBD_FlushInEpFifo (ep_num);
      }
    } else {                                            // OUT Endpoint
      OTG->DCTL |= OTG_FS_DCTL_SGONAK;                  // Set Global OUT NAK

      // Wait, with timeout, while Global OUT NAK effective is not set
      tout_cnt = SystemCoreClock / 64U;
      while ((OTG->GINTSTS & OTG_FS_GINTSTS_GONAKEFF) == 0U) {
        if (tout_cnt-- == 0U) { return ARM_DRIVER_ERROR; }
      }

      // Stall OUT Endpoint
      if ((OTG_DOEPCTL(ep_num) & OTG_FS_DOEPCTLx_EPENA) != 0U) {
        OTG_DOEPCTL(ep_num)  |= OTG_FS_DOEPCTLx_STALL | OTG_FS_DOEPCTLx_EPDIS;
      } else {
        OTG_DOEPCTL(ep_num)  |= OTG_FS_DOEPCTLx_STALL;
      }

      OTG->DCTL |= OTG_FS_DCTL_CGONAK;                  // Clear global NAK
    }
  } else {                                              // Clear STALL
    ptr_ep->in_nak = 0U;
    ptr_ep->in_zlp = 0U;
    if (ep_dir != 0U) {                                 // IN Endpoint
      if ((OTG_DIEPCTL(ep_num) & OTG_FS_DIEPCTLx_EPENA) != 0U) {  // If Endpoint enabled
        OTG_DIEPCTL(ep_num)   |= OTG_FS_DIEPCTLx_EPDIS; // Disable Endpoint
      }

      // Set DATA0 PID for Interrupt and Bulk Endpoint
      if (((OTG_DIEPCTL(ep_num) & OTG_FS_DIEPCTLx_EPTYP_MSK) >> OTG_FS_DIEPCTLx_EPTYP_POS) > 1U) {
        OTG_DIEPCTL(ep_num) |= OTG_FS_DIEPCTLx_SD0PID;
      }

      OTG_DIEPCTL(ep_num) &= ~OTG_FS_DIEPCTLx_STALL;    // Clear Stall
    } else {                                            // Clear OUT Endpoint stall
      // Set DATA0 PID for Interrupt and Bulk Endpoint
      if (((OTG_DOEPCTL(ep_num) & OTG_FS_DOEPCTLx_EPTYP_MSK) >> OTG_FS_DOEPCTLx_EPTYP_POS) > 1U) {
        OTG_DOEPCTL(ep_num) |= OTG_FS_DOEPCTLx_SD0PID;
      }
      OTG_DOEPCTL(ep_num) &= ~OTG_FS_DOEPCTLx_STALL;    // Clear Stall
    }
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_EndpointTransfer (uint8_t ep_addr, uint8_t *data, uint32_t num)
  \brief       Read data from or Write data to USB Endpoint.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \param[out]  data Pointer to buffer for data to read or with data to write
  \param[in]   num  Number of data bytes to transfer
  \return      \ref execution_status
*/
static int32_t USBD_EndpointTransfer (uint8_t ep_addr, uint8_t *data, uint32_t num) {
  volatile ENDPOINT_t *ptr_ep;
  uint8_t              ep_num;
  bool                 ep_dir;

  ep_num = EP_NUM(ep_addr);
  if (ep_num > USBD0_MAX_ENDPOINT_NUM) { return ARM_DRIVER_ERROR_PARAMETER; }
  if (hw_powered == false)             { return ARM_DRIVER_ERROR; }

  ptr_ep = &ep[EP_ID(ep_addr)];
  if (ptr_ep->active != 0U)            { return ARM_DRIVER_ERROR_BUSY; }

  ep_dir = (ep_addr & ARM_USB_ENDPOINT_DIRECTION_MASK) == ARM_USB_ENDPOINT_DIRECTION_MASK;

  ptr_ep->active = 1U;

  ptr_ep->data                  = data;
  ptr_ep->num                   = num;
  ptr_ep->num_transferred_total = 0U;
  ptr_ep->num_transferring      = 0U;

  if (ep_dir != 0U) {                                     // IN Endpoint
    if (OTG_EP_IN_TYPE(ep_num) != ARM_USB_ENDPOINT_ISOCHRONOUS) {
      if (num == 0U) {
        ptr_ep->in_zlp     =  1U;                         // Send IN ZLP requested
      }
      ptr_ep->in_nak       =  1U;                         // Set IN Endpoint NAK flag
      OTG_DIEPCTL(ep_num) |=  OTG_FS_DIEPCTLx_CNAK;       // Clear NAK
      OTG_DIEPCTL(ep_num) |=  OTG_FS_DIEPCTLx_SNAK;       // Set NAK
      OTG->DIEPMSK        |=  OTG_FS_DIEPMSK_INEPNEM;     // Enable NAK effective interrupt
    }
  } else {                                                // OUT Endpoint
    if (OTG_EP_OUT_TYPE(ep_num) != ARM_USB_ENDPOINT_ISOCHRONOUS) {
      USBD_EndpointReadSet(ep_addr);
    }
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          uint32_t USBD_EndpointTransferGetResult (uint8_t ep_addr)
  \brief       Get result of USB Endpoint transfer.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \return      number of successfully transferred data bytes
*/
static uint32_t USBD_EndpointTransferGetResult (uint8_t ep_addr) {

  if (EP_NUM(ep_addr) > USBD0_MAX_ENDPOINT_NUM) { return 0U; }

  return (ep[EP_ID(ep_addr)].num_transferred_total);
}

/**
  \fn          int32_t USBD_EndpointTransferAbort (uint8_t ep_addr)
  \brief       Abort current USB Endpoint transfer.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \return      \ref execution_status
*/
static int32_t USBD_EndpointTransferAbort (uint8_t ep_addr) {
  volatile ENDPOINT_t *ptr_ep;
  uint8_t              ep_num;

  ep_num = EP_NUM(ep_addr);
  if (ep_num > USBD0_MAX_ENDPOINT_NUM) { return ARM_DRIVER_ERROR_PARAMETER; }
  if (hw_powered == false)             { return ARM_DRIVER_ERROR; }

  ptr_ep = &ep[EP_ID(ep_addr)];

  ptr_ep->num    = 0U;
  ptr_ep->in_nak = 0U;
  ptr_ep->in_zlp = 0U;

  if ((ep_addr & ARM_USB_ENDPOINT_DIRECTION_MASK) != 0U) {
    if ((OTG_DIEPCTL(ep_num) & OTG_FS_DIEPCTLx_EPENA) != 0U) {
      // Set flush flag to Flush IN FIFO in Endpoint disabled interrupt
      ptr_ep->in_flush = 1U;

      // Disable enabled Endpoint and set NAK
      OTG_DIEPCTL(ep_num) |= OTG_FS_DIEPCTLx_EPDIS | OTG_FS_DIEPCTLx_SNAK;
    } else {
      // Endpoint is already disabled. Set NAK
      OTG_DIEPCTL(ep_num) |= OTG_FS_DIEPCTLx_SNAK;

      // Flush IN EP FIFO
      USBD_FlushInEpFifo (ep_num);
    }

  } else {
    if ((OTG_DOEPCTL(ep_num) & OTG_FS_DOEPCTLx_EPENA) != 0U) {
      // Disable enabled Endpoint and set NAK
      OTG_DOEPCTL(ep_num) |= OTG_FS_DOEPCTLx_EPDIS | OTG_FS_DOEPCTLx_SNAK;
    } else {
      // Endpoint is already disabled. Set NAK
      OTG_DOEPCTL(ep_num) |= OTG_FS_DOEPCTLx_SNAK;
    }
  }

  ptr_ep->active = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          uint16_t USBD_GetFrameNumber (void)
  \brief       Get current USB Frame Number.
  \return      Frame Number
*/
static uint16_t USBD_GetFrameNumber (void) {

  if (hw_powered == false) { return 0U; }

  return ((OTG->DSTS & OTG_FS_DSTS_FNSOF_MSK) >> OTG_FS_DSTS_FNSOF_POS);
}

/**
  \fn          void USBD_FS_IRQ (uint32_t gintsts)
  \brief       USB Device Interrupt Routine (IRQ).
*/
void USBD_FS_IRQ (uint32_t gintsts) {
  volatile ENDPOINT_t *ptr_ep, *ptr_ep_in;
  uint32_t             val, msk, ep_int;
#ifdef MX_USB_OTG_FS_VBUS_Pin
  uint32_t             gotgint;
#endif
  uint16_t             num;
  uint8_t              ep_num, i;
  static uint32_t      IsoInIncomplete = 0U;

  if ((gintsts & OTG_FS_GINTSTS_USBRST) != 0U) {        // Reset interrupt
    OTG->GINTSTS  =  OTG_FS_GINTSTS_USBRST;
    OTG->GINTMSK |=  OTG_FS_GINTMSK_SOFM;               // Unmask SOF interrupts (to detect initial SOF)
    USBD_Reset();
    usbd_state.active = 0U;
    SignalDeviceEvent(ARM_USBD_EVENT_RESET);
  }

  if ((gintsts & OTG_FS_GINTSTS_USBSUSP) != 0U) {       // Suspend interrupt
    OTG->GINTSTS  =  OTG_FS_GINTSTS_USBSUSP;
    OTG->PCGCCTL |=  OTG_FS_PCGCCTL_STPPCLK;            // Stop PHY clock
    usbd_state.active = 0U;
    SignalDeviceEvent(ARM_USBD_EVENT_SUSPEND);
  }

  if ((gintsts & OTG_FS_GINTSTS_WKUPINT) != 0U) {       // Resume interrupt
    OTG->GINTSTS  =  OTG_FS_GINTSTS_WKUPINT;
    OTG->PCGCCTL &= ~OTG_FS_PCGCCTL_STPPCLK;            // Start PHY clock
    usbd_state.active = 1U;
    SignalDeviceEvent(ARM_USBD_EVENT_RESUME);
  }

  if ((gintsts & OTG_FS_GINTSTS_ENUMDNE) != 0U) {       // Speed enumeration completed
    OTG->GINTSTS  = OTG_FS_GINTSTS_ENUMDNE;
    usbd_state.speed  = ARM_USB_SPEED_FULL;
    OTG->DCTL    |= OTG_FS_DCTL_CGINAK;                 // Clear global IN NAK
    OTG->DCTL    |= OTG_FS_DCTL_CGONAK;                 // Clear global OUT NAK
  }

  if ((usbd_state.active == 0U) &&
      (gintsts & OTG_FS_GINTSTS_SOF) != 0U) {           // First SOF after Reset
    OTG->GINTMSK &= ~OTG_FS_GINTMSK_SOFM;               // Mask SOF interrupts
    OTG->GINTSTS  =  OTG_FS_GINTSTS_SOF;
    usbd_state.active = 1U;
    SignalDeviceEvent(ARM_USBD_EVENT_RESUME);
  }

  if ((gintsts & OTG_FS_GINTSTS_RXFLVL) != 0U) {        // Receive FIFO interrupt
    val    =  OTG->GRXSTSP;
    ep_num =  val & 0x0FU;
    num    = (val >>  4) & 0x7FFU;
    switch  ((val >> 17) & 0x0FU) {
      case 6:                                           // Setup packet
        // Read setup packet
        setup_packet[0] = OTG_RX_FIFO;
        setup_packet[1] = OTG_RX_FIFO;

        // Analyze Setup packet for SetAddress
        if ((setup_packet[0] & 0xFFFFU) == 0x0500U) {
          USBD_DeviceSetAddress((setup_packet[0] >> 16) & 0xFFU);
        }
        setup_received = 1U;
        break;
      case 2:                                           // OUT packet
        USBD_ReadFromFifo(ep_num, num);
        break;
      case 1:                                           // Global OUT NAK
      case 3:                                           // OUT transfer completed
      case 4:                                           // SETUP transaction completed
      default:
        break;
    }
  }

  // OUT Packet
  if ((gintsts & OTG_FS_GINTSTS_OEPINT) != 0U) {
    msk    = (((OTG->DAINT & OTG->DAINTMSK) >> 16) & 0xFFFFU);
    ep_num = 0U;
    do {
      if (((msk >> ep_num) & 1U) != 0U) {
        ep_int = OTG_DOEPINT(ep_num) & OTG->DOEPMSK;
        ptr_ep = &ep[EP_ID(ep_num)];
        if ((ep_int & OTG_FS_DOEPINTx_EPDISD) != 0U) {  // If Endpoint disabled
          if (OTG_EP_OUT_TYPE(ep_num) == ARM_USB_ENDPOINT_ISOCHRONOUS) {
            // Isochronous OUT Endpoint
            // Set packet count and transfer size
            OTG_DOEPTSIZ(ep_num) = (1U << OTG_FS_DOEPTSIZx_PKTCNT_POS) |
                                   (ptr_ep->num);

            // Set correct frame
            if ((USBD_GetFrameNumber() & 1U) != 0U) { OTG_DOEPCTL(ep_num) |= OTG_FS_DOEPCTLx_SEVNFRM; }
            else                                    { OTG_DOEPCTL(ep_num) |= OTG_FS_DOEPCTLx_SODDFRM; }

            OTG_DOEPCTL(ep_num) |= OTG_FS_DOEPCTLx_EPENA | OTG_FS_DOEPCTLx_CNAK;
          }
          OTG_DOEPINT(ep_num) = OTG_FS_DOEPINTx_EPDISD;
        }

        // Setup phase done interrupt
        if ((ep_int & OTG_FS_DOEPINTx_STUP) != 0U) {
          ptr_ep->num = 0U;
          OTG_DOEPINT(ep_num) = OTG_FS_DOEPINTx_STUP;
          SignalEndpointEvent(ep_num, ARM_USBD_EVENT_SETUP);
        }

        // Transfer complete interrupt
        if ((ep_int & OTG_FS_DOEPINTx_XFCR) != 0U) {
          OTG_DOEPINT(ep_num) = OTG_FS_DOEPINTx_XFCR;
          if (ptr_ep->num != 0U) {
            if (OTG_EP_OUT_TYPE(ep_num) != ARM_USB_ENDPOINT_ISOCHRONOUS) {
              USBD_EndpointReadSet(ep_num);
            }
          } else {
            ptr_ep->active = 0U;
            SignalEndpointEvent(ep_num, ARM_USBD_EVENT_OUT);
          }
        }
      }
      ep_num++;
    } while ((msk >> ep_num) != 0U);
  }

  // IN Packet
  if ((gintsts & OTG_FS_GINTSTS_IEPINT) != 0U) {
    msk    = (OTG->DAINT & OTG->DAINTMSK & 0xFFFFU);
    ep_num = 0U;
    do {
      if (((msk >> ep_num) & 1U) != 0U) {
        ep_int = OTG_DIEPINT(ep_num) & OTG->DIEPMSK;
        ptr_ep = &ep[EP_ID(ep_num | ARM_USB_ENDPOINT_DIRECTION_MASK)];

        if ((ep_int & OTG_FS_DIEPINTx_EPDISD) != 0U) {  // If Endpoint disabled
          OTG_DIEPINT(ep_num) = OTG_FS_DIEPINTx_EPDISD;
          if (ptr_ep->in_flush == 1U) {
            // Clear flush flag
            ptr_ep->in_flush = 0U;  
            // Flush IN Endpoint FIFO
            USBD_FlushInEpFifo(ep_num);
          } else if (OTG_EP_IN_TYPE(ep_num) == ARM_USB_ENDPOINT_ISOCHRONOUS) {
            if ((IsoInIncomplete & (1U << ep_num)) != 0U) {
              // Flush IN Endpoint FIFO and write new data if available
              USBD_FlushInEpFifo(ep_num);
              if (ptr_ep->data != NULL) { 
                ptr_ep->num += ptr_ep->num_transferring;
                USBD_WriteToFifo(ep_num | ARM_USB_ENDPOINT_DIRECTION_MASK);
              }
              IsoInIncomplete &= ~(1U << ep_num);
            }
          }
        }

        // IN Endpoint NAK effective
        if ((ep_int & OTG_FS_DIEPINTx_INEPNE) != 0U) {
          if (ptr_ep->in_nak != 0U) {
            ptr_ep->in_nak = 0U;
            val = 0U;
            ptr_ep_in = &ep[0];
            for (i = 0U; i < USBD0_MAX_ENDPOINT_NUM; i++) {
              val |= ptr_ep_in->in_nak;
              ptr_ep_in += 2U;
            }

            // If no more forced NAKs, disable IN NAK effective interrupt
            if (val == 0U) { OTG->DIEPMSK &= ~OTG_FS_DIEPMSK_INEPNEM; }

            // If Data available, write Data
            if ((ptr_ep->num != 0U) || (ptr_ep->in_zlp != 0U)) {
              USBD_WriteToFifo(ep_num | ARM_USB_ENDPOINT_DIRECTION_MASK);
            }
          }
          OTG_DIEPINT(ep_num) = OTG_FS_DIEPINTx_INEPNE;
        }

        // Transmit completed
        if ((ep_int & OTG_FS_DIEPINTx_XFCR) != 0U) {
          OTG_DIEPINT(ep_num) = OTG_FS_DIEPINTx_XFCR;
          ptr_ep->num_transferred_total += ptr_ep->num_transferring;
          if (ptr_ep->num == 0U) {
            ptr_ep->data   = NULL;
            ptr_ep->active = 0U;
            SignalEndpointEvent(ep_num | ARM_USB_ENDPOINT_DIRECTION_MASK, ARM_USBD_EVENT_IN);
          } else {
            if (OTG_EP_IN_TYPE(ep_num) != ARM_USB_ENDPOINT_ISOCHRONOUS) {
              USBD_WriteToFifo(ep_num | ARM_USB_ENDPOINT_DIRECTION_MASK);
            }
          }
        }
      }
      ep_num++;
    } while ((msk >> ep_num != 0U));
  }

  // End of periodic frame
  if ((gintsts & OTG_FS_GINTSTS_EOPF) != 0U) {

    // Clear interrupt flags
    OTG->GINTSTS = OTG_FS_GINTSTS_EOPF | OTG_FS_GINTSTS_IPXFR | OTG_FS_GINTSTS_IISOIXFR;

    // Check enabled isochronous OUT Endpoints
    for (ep_num = 1U; ep_num <= USBD0_MAX_ENDPOINT_NUM; ep_num++) {
      if (OTG_EP_OUT_TYPE(ep_num) != ARM_USB_ENDPOINT_ISOCHRONOUS) { continue; }
      if ((OTG_DOEPCTL(ep_num) & OTG_FS_DOEPCTLx_USBAEP) == 0U)    { continue; }

      if ((gintsts & OTG_FS_GINTSTS_IPXFR) != 0U) {
        // Incomplete Isochronous OUT transfer
        if ((USBD_GetFrameNumber() & 1U) == ((OTG_DOEPCTL(ep_num) >> OTG_FS_DOEPCTLx_EONUM_POS) & 1U)) {
          if ((OTG_DOEPCTL(ep_num) & OTG_FS_DOEPCTLx_EPENA) != 0U) {
            OTG_DOEPCTL(ep_num) |= OTG_FS_DOEPCTLx_EPDIS;
          }
        }
      } else {
        // Isochronous OUT transfer completed
        if (ep[EP_ID(ep_num)].num != 0U) {
          USBD_EndpointReadSet(ep_num);
        }
      }
    }

    // Check enabled isochronous IN Endpoints
    for (ep_num = 1U; ep_num <= USBD0_MAX_ENDPOINT_NUM; ep_num++) {
      if (OTG_EP_IN_TYPE(ep_num) != ARM_USB_ENDPOINT_ISOCHRONOUS)  { continue; }
      if ((OTG_DIEPCTL(ep_num)   &  OTG_FS_DIEPCTLx_USBAEP) == 0U) { continue; }

      if ((gintsts & OTG_FS_GINTSTS_IISOIXFR) != 0U) {
        if ((OTG_DIEPCTL(ep_num) & OTG_FS_DIEPCTLx_EPENA) != 0U) {
          if ((USBD_GetFrameNumber() & 1U) == ((OTG_DIEPCTL(ep_num) >> OTG_FS_DIEPCTLx_EONUM_POS) & 1U)) {
            IsoInIncomplete  |= (1U << ep_num);
            OTG_DIEPCTL(ep_num) |= OTG_FS_DIEPCTLx_EPDIS | OTG_FS_DIEPCTLx_SNAK;
          }
        }
      } else {
        if (ep[EP_ID(ep_num | ARM_USB_ENDPOINT_DIRECTION_MASK)].num != 0U) {
          USBD_WriteToFifo (ep_num | ARM_USB_ENDPOINT_DIRECTION_MASK);
        }
      }
    }
  }
#ifdef MX_USB_OTG_FS_VBUS_Pin
  if ((gintsts & OTG_FS_GINTSTS_SRQINT) != 0U) {
    OTG->GINTSTS = OTG_FS_GINTSTS_SRQINT;
    usbd_state.vbus = 1U;
    SignalDeviceEvent(ARM_USBD_EVENT_VBUS_ON);
  }

  if ((gintsts & OTG_FS_GINTSTS_OTGINT) != 0U) {
    gotgint = OTG->GOTGINT;
    OTG->GOTGINT = gotgint;
    if ((gotgint & OTG_FS_GOTGINT_SEDET) != 0U) {
      usbd_state.vbus   = 0U;
      usbd_state.speed  = 0U;
      usbd_state.active = 0U;
      SignalDeviceEvent(ARM_USBD_EVENT_VBUS_OFF);
    }
  }
#endif
}

ARM_DRIVER_USBD Driver_USBD0 = {
  USBD_GetVersion,
  USBD_GetCapabilities,
  USBD_Initialize,
  USBD_Uninitialize,
  USBD_PowerControl,
  USBD_DeviceConnect,
  USBD_DeviceDisconnect,
  USBD_DeviceGetState,
  USBD_DeviceRemoteWakeup,
  USBD_DeviceSetAddress,
  USBD_ReadSetupPacket,
  USBD_EndpointConfigure,
  USBD_EndpointUnconfigure,
  USBD_EndpointStall,
  USBD_EndpointTransfer,
  USBD_EndpointTransferGetResult,
  USBD_EndpointTransferAbort,
  USBD_GetFrameNumber
};

/*! \endcond */
