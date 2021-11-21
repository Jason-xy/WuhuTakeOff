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
 * $Date:        6. November 2018
 * $Revision:    V1.16
 *
 * Driver:       Driver_CAN1/2/3
 * Configured:   via RTE_Device.h configuration file
 * Project:      CAN Driver for ST STM32F4xx
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                 Value   CAN Interface
 *   ---------------------                 -----   -------------
 *   Connect to hardware via Driver_CAN# = 1       use CAN1
 *   Connect to hardware via Driver_CAN# = 2       use CAN2
 *   Connect to hardware via Driver_CAN# = 3       use CAN3
 * --------------------------------------------------------------------------
 * Defines used for driver configuration (at compile time):
 *
 *   CAN_CLOCK_TOLERANCE:  defines maximum allowed clock tolerance in 1/1024 steps
 *     - default value:    15 (approx. 1.5 %)
 *   CAN1_FILTER_BANK_NUM: defines maximum number of Filter Banks used for CAN1 controller (0..28)
 *                         (sum of maximum number of Filter Banks used for CAN1 and CAN2 must not exceed 28)
 *     - default value:    14
 *   CAN2_FILTER_BANK_NUM: defines maximum number of Filter Banks used for CAN2 controller (0..28)
 *                         (sum of maximum number of Filter Banks used for CAN1 and CAN2 must not exceed 28)
 *     - default value:    14
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.16
 *    Corrected SetBitrate function to leave Silent and Loopback mode as they were
 *    Corrected SetMode function to clear Silent and Loopback mode when NORMAL mode is activated
 *  Version 1.15
 *    Corrected MessageSend function to only access required data for sending
 *  Version 1.14
 *    Corrected abort message send functionality (IRQ deadloop)
 *  Version 1.13
 *    Added support for ARM Compiler 6
 *  Version 1.12
 *    Corrected SetBitrate function
 *  Version 1.11
 *    Corrected comment and code for CubeMX CAN interrupts settings
 *    (should be disabled)
 *  Version 1.10
 *    Corrected filter setting for adding/removing maskable Standard ID
 *  Version 1.9
 *    Added CAN3
 *  Version 1.8
 *    Corrected clearing of overrun flag in interrupt routine
 *  Version 1.7
 *    Added port configuration for ports supported by new subfamily
 *  Version 1.6
 *    Corrected CAN2 initialization was disabling CAN1 filters
 *  Version 1.5
 *    Corrected receive overrun signaling
 *  Version 1.4
 *    Corrected functionality when NULL pointer is provided for one or both 
 *    signal callbacks in Initialize function call
 *  Version 1.3
 *    Corrected functionality when only one CAN controller is used
 *  Version 1.2
 *    Corrected PowerControl function for:
 *      - Unconditional Power Off
 *      - Conditional Power full (driver must be initialized)
 *  Version 1.1
 *    Corrected option for selecting only CAN1
 *    Added clock enable before power off
 *  Version 1.0
 *    Initial release
 */

/*! \page stm32f4_can CMSIS-Driver CAN Setup

The CMSIS-Driver CAN requires:
  - APB1 Clock (maximum frequency is 42 MHz)
  - Configuration of CAN
 
Valid settings for various evaluation boards are listed in the table below:

Peripheral Resource | MCBSTM32F400         | STM32F4-Discovery    | 32F401C-Discovery    | 32F429I-Discovery
:-------------------|:---------------------|:---------------------|:---------------------|:------------------
CAN1 Mode           | CAN1: <b>Master</b>  | CAN1: <b>Master</b>  | n/a                  | CAN1: <b>Master</b>
CAN1_RX pin         | PI9                  | PA11/PB8/PD0         | n/a                  | PA11/PB8/PD0
CAN1_TX pin         | PH13                 | PA12/PB9/PD1         | n/a                  | PA12/PB9/PD1
CAN2 Mode           | CAN2: <b>Slave</b>   | CAN2: <b>Slave</b>   | n/a                  | CAN2: <b>Slave</b>
CAN2_RX pin         | PB5                  | PB5/PB12             | n/a                  | PB5/PB12
CAN2_TX pin         | PB13                 | PB6/PB13             | n/a                  | PB6/PB13
CAN3 Mode           | CAN3: <b>Master</b>  | CAN3: <b>Master</b>  | n/a
CAN3_RX pin         | n/a                  | n/a                  | n/a
CAN3_TX pin         | n/a                  | n/a                  | n/a
 
For different boards, refer to the hardware schematics to reflect correct setup values.
 
The STM32CubeMX configuration for Keil MCBSTM32F400 Board with steps for Pinout, Clock, and System Configuration are 
listed below. Enter the values that are marked \b bold.
 
Pinout tab
----------
  1. Configure CAN1 mode
     - Peripheral \b CAN1: Mode=<b>Master Mode</b>
  2. Configure CAN2 mode
     - Peripheral \b CAN2: Mode=<b>Slave Mode</b>
 
Clock Configuration tab
-----------------------
  1. Configure APB1 Clock (maximum frequency is 42 MHz)
     - Setup "APB1 peripheral clocks (MHz)" to match application requirements
 
Configuration tab
-----------------
  1. Under Connectivity open \b CAN1 Configuration:
     - Parameter Settings: not used
     - User Constants: not used
     - <b>NVIC Settings</b>: review settings, no changes required
          Interrupt Table          | Enable | Preemption Priority | Sub Priority
          :------------------------|:-------|:--------------------|:--------------
          CAN1 TX interrupts       |\b OFF  | 0                   | 0
          CAN1 RX0 interrupts      |\b OFF  | 0                   | 0
          CAN1 RX1 interrupts      |\b OFF  | 0                   | 0
          CAN1 SCE interrupts      |\b OFF  | 0                   | 0
     - <b>GPIO Settings</b>: review settings, no changes required
          Pin Name | Signal on Pin |  GPIO mode  | GPIO Pull-up/Pull..| Maximum out | User Label
          :--------|:--------------|:------------|:-------------------|:------------|:----------
          PI9      | CAN1_RX       | Alternate ..| No pull-up and no..| High        |.
          PH13     | CAN1_TX       | Alternate ..| No pull-up and no..| High        |.
     - Click \b OK to close the CAN1 Configuration dialog
  2. Under Connectivity open \b CAN2 Configuration:
     - Parameter Settings: not used
     - User Constants: not used
     - <b>NVIC Settings</b>: review settings, no changes required
          Interrupt Table          | Enable | Preemption Priority | Sub Priority
          :------------------------|:-------|:--------------------|:--------------
          CAN2 TX interrupts       |\b OFF  | 0                   | 0
          CAN2 RX0 interrupts      |\b OFF  | 0                   | 0
          CAN2 RX1 interrupts      |\b OFF  | 0                   | 0
          CAN2 SCE interrupts      |\b OFF  | 0                   | 0
     - <b>GPIO Settings</b>: review settings, no changes required
          Pin Name | Signal on Pin |  GPIO mode  | GPIO Pull-up/Pull..| Maximum out | User Label
          :--------|:--------------|:------------|:-------------------|:------------|:----------
          PB5      | CAN2_RX       | Alternate ..| No pull-up and no..| High        |.
          PB13     | CAN2_TX       | Alternate ..| No pull-up and no..| High        |.
     - Click \b OK to close the CAN2 Configuration dialog
*/

/*! \cond */


#include "CAN_STM32F4xx.h"


// Externally overridable configuration definitions

// Maximum allowed clock tolerance in 1/1024 steps
#ifndef CAN_CLOCK_TOLERANCE
#define CAN_CLOCK_TOLERANCE             (15U)   // 15/1024 approx. 1.5 %
#endif

// Maximum number of Message Objects used for CAN1 controller
#ifndef CAN1_FILTER_BANK_NUM
#define CAN1_FILTER_BANK_NUM            (14UL)
#endif
// Maximum number of Message Objects used for CAN2 controller
#ifndef CAN2_FILTER_BANK_NUM
#define CAN2_FILTER_BANK_NUM            (14UL)
#endif

#define CAN3_FILTER_BANK_NUM            (14UL)

#if   ((CAN1_FILTER_BANK_NUM + CAN2_FILTER_BANK_NUM) > 28U)
#error  Too many Filter Banks defined, maximum sum of Filter Banks for both CAN1 and CAN2 is 28 !!!
#endif

#define CAN_RX_OBJ_NUM                  (2U)          // Number of receive objects
#define CAN_TX_OBJ_NUM                  (3U)          // Number of transmit objects
#define CAN_TOT_OBJ_NUM                 (CAN_RX_OBJ_NUM + CAN_TX_OBJ_NUM)

// Local Functions *************************************************************

/**
  \fn          void Enable_GPIO_Clock (const GPIO_TypeDef *port)
  \brief       Enable GPIO clock
*/
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
static void Enable_GPIO_Clock (const GPIO_TypeDef *GPIOx) {
  if      (GPIOx == GPIOA) { __GPIOA_CLK_ENABLE(); }
  else if (GPIOx == GPIOB) { __GPIOB_CLK_ENABLE(); }
  else if (GPIOx == GPIOC) { __GPIOC_CLK_ENABLE(); }
  else if (GPIOx == GPIOD) { __GPIOD_CLK_ENABLE(); }
  else if (GPIOx == GPIOE) { __GPIOE_CLK_ENABLE(); }
#if defined(GPIOF)
  else if (GPIOx == GPIOF) { __GPIOF_CLK_ENABLE(); }
#endif
#if defined(GPIOG)
  else if (GPIOx == GPIOG) { __GPIOG_CLK_ENABLE(); }
#endif
#if defined(GPIOH)
  else if (GPIOx == GPIOH) { __GPIOH_CLK_ENABLE(); }
#endif
#if defined(GPIOI)
  else if (GPIOx == GPIOI) { __GPIOI_CLK_ENABLE(); }
#endif
#if defined(GPIOJ)
  else if (GPIOx == GPIOJ) { __GPIOJ_CLK_ENABLE(); }
#endif
#if defined(GPIOK)
  else if (GPIOx == GPIOK) { __GPIOK_CLK_ENABLE(); }
#endif
}
#endif


// CAN Driver ******************************************************************

#define ARM_CAN_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,16)        // CAN driver version

// Driver Version
static const ARM_DRIVER_VERSION can_driver_version = { ARM_CAN_API_VERSION, ARM_CAN_DRV_VERSION };

// Driver Capabilities
static const ARM_CAN_CAPABILITIES can_driver_capabilities = {
  CAN_TOT_OBJ_NUM,      // Number of CAN Objects available
  1U,                   // Supports reentrant calls to ARM_CAN_MessageSend, ARM_CAN_MessageRead, ARM_CAN_ObjectConfigure and abort message sending used by ARM_CAN_Control.
  0U,                   // Does not support CAN with flexible data-rate mode (CAN_FD)
  0U,                   // Does not support restricted operation mode
  1U,                   // Supports bus monitoring mode
  1U,                   // Supports internal loopback mode
  1U                    // Supports external loopback mode
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
, 0U                    // Reserved bits
#endif
};

// Object Capabilities
static const ARM_CAN_OBJ_CAPABILITIES can_object_capabilities_rx = {
  0U,                   // Object does not support transmission
  1U,                   // Object supports reception
  0U,                   // Object does not support RTR reception and automatic Data transmission
  0U,                   // Object does not support RTR transmission and automatic Data reception
  1U,                   // Object allows assignment of multiple filters to it
  1U,                   // Object supports exact identifier filtering
  0U,                   // Object does not support range identifier filtering
  1U,                   // Object supports mask identifier filtering
  3U                    // Object can buffer 3 messages
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
, 0U                    // Reserved bits
#endif
};
static const ARM_CAN_OBJ_CAPABILITIES can_object_capabilities_tx = {
  1U,                   // Object supports transmission
  0U,                   // Object does not support reception
  0U,                   // Object does not support RTR reception and automatic Data transmission
  0U,                   // Object does not support RTR transmission and automatic Data reception
  0U,                   // Object does not allow assignment of multiple filters to it
  0U,                   // Object does not support exact identifier filtering
  0U,                   // Object does not support range identifier filtering
  0U,                   // Object does not support mask identifier filtering
  1U                    // Object can only buffer 1 message
#if (defined(ARM_CAN_API_VERSION) && (ARM_CAN_API_VERSION >= 0x101U))
, 0U                    // Reserved bits
#endif
};

#define CAN_FRx_32BIT_IDE               ((uint32_t)1U <<  2)
#define CAN_FRx_32BIT_RTR               ((uint32_t)1U <<  1)
#define CAN_FRx_16BIT_L_IDE             ((uint32_t)1U <<  3)
#define CAN_FRx_16BIT_L_RTR             ((uint32_t)1U <<  4)
#define CAN_FRx_16BIT_H_IDE             ((uint32_t)1U << 19)
#define CAN_FRx_16BIT_H_RTR             ((uint32_t)1U << 20)

typedef enum _CAN_FILTER_TYPE {
  CAN_FILTER_TYPE_EXACT_ID    = 0U,
  CAN_FILTER_TYPE_MASKABLE_ID = 1U
} CAN_FILTER_TYPE;

#if    defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern CAN_HandleTypeDef hcan3;
#endif

static CAN_TypeDef * const ptr_CANx[3] = { (CAN_TypeDef *)CAN1, 
                                           (CAN_TypeDef *)CAN2, 
#if defined(CAN3)
                                           (CAN_TypeDef *)CAN3 
#else
                                            NULL 
#endif
                                         };

// Local variables and structures
static uint8_t                     can_driver_powered    [CAN_CTRL_NUM] = { 0U
#if (CAN_CTRL_NUM > 1)
  , 0U
#endif
#if (CAN_CTRL_NUM > 2)
  , 0U
#endif
};
static uint8_t                     can_driver_initialized[CAN_CTRL_NUM] = { 0U
#if (CAN_CTRL_NUM > 1)
  , 0U
#endif
#if (CAN_CTRL_NUM > 2)
  , 0U
#endif
};
static uint8_t                     can_obj_cfg           [CAN_CTRL_NUM][CAN_TOT_OBJ_NUM];
static ARM_CAN_SignalUnitEvent_t   CAN_SignalUnitEvent   [CAN_CTRL_NUM] = { NULL
#if (CAN_CTRL_NUM > 1)
  , NULL
#endif
#if (CAN_CTRL_NUM > 2)
  , NULL
#endif
};
static ARM_CAN_SignalObjectEvent_t CAN_SignalObjectEvent [CAN_CTRL_NUM] = { NULL
#if (CAN_CTRL_NUM > 1)
  , NULL
#endif
#if (CAN_CTRL_NUM > 2)
  , NULL
#endif
};


// Helper Functions

/**
  \fn          int32_t CANx_AddFilter (uint32_t obj_idx, CAN_FILTER_TYPE filter_type, uint32_t id, uint32_t mask, uint8_t x)
  \brief       Add receive filter for specified id or id with mask.
  \param[in]   obj_idx      Receive object index assignment
  \param[in]   filter_type  Type of filter to add
                 - CAN_FILTER_TYPE_EXACT_ID:    exact id filter (id only)
                 - CAN_FILTER_TYPE_MASKABLE_ID: maskable filter (id and mask)
  \param[in]   id           Identifier
  \param[in]   mask         Identifier Mask
  \param[in]   x            Controller number (0..2)
  \return      execution status
*/
static int32_t CANx_AddFilter (uint32_t obj_idx, CAN_FILTER_TYPE filter_type, uint32_t id, uint32_t mask, uint8_t x) {
  CAN_TypeDef *ptr_CAN_master;
  uint32_t     fa1r, frx, fry, msk;
  uint8_t      bank, bank_end;
  int32_t      status;

  if (x >= CAN_CTRL_NUM)         { return ARM_DRIVER_ERROR;           }
  if (obj_idx >= CAN_RX_OBJ_NUM) { return ARM_DRIVER_ERROR_PARAMETER; }
  if ((filter_type != CAN_FILTER_TYPE_EXACT_ID) && 
      (filter_type != CAN_FILTER_TYPE_MASKABLE_ID)) 
                                 { return ARM_DRIVER_ERROR_PARAMETER; }
  if      (x == 2U)              { bank = 0U;                   bank_end = CAN3_FILTER_BANK_NUM;                        }
  else if (x == 1U)              { bank = CAN1_FILTER_BANK_NUM; bank_end = CAN1_FILTER_BANK_NUM + CAN2_FILTER_BANK_NUM; }
  else                           { bank = 0U;                   bank_end = CAN1_FILTER_BANK_NUM;                        }
  if (bank >= bank_end)          { return ARM_DRIVER_ERROR;           }

  if (x <= 1U) {
    ptr_CAN_master = ptr_CANx[0];
  } else {
    ptr_CAN_master = ptr_CANx[2];
  }

  status = ARM_DRIVER_OK;
  ptr_CAN_master->FMR |= CAN_FMR_FINIT;                                 // Enter filter initialization mode

  fa1r = ptr_CAN_master->FA1R;
  msk  = (uint32_t)1U << bank;

  if ((id & ARM_CAN_ID_IDE_Msk) != 0U) {                                // Extended Identifier
    frx = (id << 3) | CAN_FRx_32BIT_IDE;                                // id + IDE
    switch (filter_type) {
      case CAN_FILTER_TYPE_EXACT_ID:
        fry = (id << 3) | CAN_FRx_32BIT_IDE | CAN_FRx_32BIT_RTR;        // id + IDE + RTR
        while (bank <= bank_end) {                                      // Find empty place for id
          if (bank == bank_end) {                                       // If no free found exit
            status = ARM_DRIVER_ERROR;
            break;
          }
          if ((fa1r & msk) == 0U) {                                     // If filter is not active
            ptr_CAN_master->FM1R |=  msk;                               // Select identifier list mode
            ptr_CAN_master->FS1R |=  msk;                               // Select single 32-bit scale configuration
            if (obj_idx & 1U) {
              ptr_CAN_master->FFA1R |=  msk;                            // Assign to FIFO1
            } else {
              ptr_CAN_master->FFA1R &= ~msk;                            // Assign to FIFO0
            }
            ptr_CAN_master->sFilterRegister[bank].FR1 = frx;            // id + IDE
            ptr_CAN_master->sFilterRegister[bank].FR2 = fry;            // id + IDE + RTR
            break;
          }
          msk <<= 1U;
          bank++;
        }
        break;
      case CAN_FILTER_TYPE_MASKABLE_ID:
        fry = (mask << 3) | CAN_FRx_32BIT_IDE;                          // mask + IDE
        while (bank <= bank_end) {                                      // Find empty place for id
          if (bank == bank_end) {                                       // If no free found exit
            status = ARM_DRIVER_ERROR;
            break;
          }
          if ((fa1r & msk) == 0U) {                                     // If filter is not active
            ptr_CAN_master->FM1R &= ~msk;                               // Select identifier mask mode
            ptr_CAN_master->FS1R |=  msk;                               // Select single 32-bit scale configuration
            if (obj_idx & 1U) {
              ptr_CAN_master->FFA1R |=  msk;                            // Assign to FIFO1
            } else {
              ptr_CAN_master->FFA1R &= ~msk;                            // Assign to FIFO0
            }
            ptr_CAN_master->sFilterRegister[bank].FR1 = frx;            // id + IDE
            ptr_CAN_master->sFilterRegister[bank].FR2 = fry;            // mask + IDE
            break;
          }
          msk <<= 1U;
          bank++;
        }
        break;
    }
  } else {                                                              // Standard Identifier
    switch (filter_type) {
      case CAN_FILTER_TYPE_EXACT_ID:
        frx = ((id & 0xFFFFU) <<  5) |                                  // Low 16 bits = id
              ((id & 0xFFFFU) << 21) | CAN_FRx_16BIT_H_RTR;             // High 16 bits = id + RTR
        while (bank <= bank_end) {                                      // Find empty place for id
          if (bank == bank_end) {                                       // If no free found exit
            status = ARM_DRIVER_ERROR;
            break;
          }
          if ((fa1r & msk) == 0U) {                                     // If filter is not active
            ptr_CAN_master->FM1R |=  msk;                               // Select identifier list mode
            ptr_CAN_master->FS1R &= ~msk;                               // Select dual 16-bit scale configuration
            if (obj_idx & 1U) {
              ptr_CAN_master->FFA1R |=  msk;                            // Assign to FIFO1
            } else {
              ptr_CAN_master->FFA1R &= ~msk;                            // Assign to FIFO0
            }
            ptr_CAN_master->sFilterRegister[bank].FR1 = frx;
            ptr_CAN_master->sFilterRegister[bank].FR2 = 0xFFFFFFFFU;    // Disable unused slots
            break;
          } else if  (((ptr_CAN_master->FM1R & msk) != 0U) &&           // If identifier list mode
                      ((ptr_CAN_master->FS1R & msk) == 0U) &&           // If dual 16-bit scale configuration
                     (((ptr_CAN_master->FFA1R>>bank)&1U) == obj_idx)) { // If bank has same FIFO assignment as requested
            if         (ptr_CAN_master->sFilterRegister[bank].FR1==0xFFFFFFFFU){// If n and n+1 entry are not used
              ptr_CAN_master->FA1R &= ~msk;                             // Put filter in inactive mode
              ptr_CAN_master->sFilterRegister[bank].FR1 = frx;          // id and id + RTR
              break;
            } else if  (ptr_CAN_master->sFilterRegister[bank].FR2==0xFFFFFFFFU){// If n+2 and n+3 entry are not used
              ptr_CAN_master->FA1R &= ~msk;                             // Put filter in inactive mode
              ptr_CAN_master->sFilterRegister[bank].FR2 = frx;          // id and id + RTR
              break;
            }
          }
          msk <<= 1U;
          bank++;
        }
        break;
      case CAN_FILTER_TYPE_MASKABLE_ID:
        frx = ((id   & 0xFFFFU) <<  5) |                                // Low 16 bits = id
              ((mask & 0xFFFFU) << 21) ;                                // High 16 bits = mask
        if ((mask & ARM_CAN_ID_IDE_Msk) != 0U) {                        // If IDE masking enabled
          frx |= CAN_FRx_16BIT_H_RTR;                                   // High 16 bits = mask + IDE
        }
        while (bank <= bank_end) {                                      // Find empty place for id
          if (bank == bank_end) {                                       // If no free found exit
            status = ARM_DRIVER_ERROR;
            break;
          }
          if ((fa1r & msk) == 0U) {                                     // If filter is not active
            ptr_CAN_master->FM1R &= ~msk;                               // Select identifier mask mode
            ptr_CAN_master->FS1R &= ~msk;                               // Select dual 16-bit scale configuration
            if (obj_idx & 1U) {
              ptr_CAN_master->FFA1R |=  msk;                            // Assign to FIFO1
            } else {
              ptr_CAN_master->FFA1R &= ~msk;                            // Assign to FIFO0
            }
            ptr_CAN_master->sFilterRegister[bank].FR1 = frx;
            ptr_CAN_master->sFilterRegister[bank].FR2 = 0xFFFFFFFFU;    // Disable unused slots
            break;
          } else if ((((ptr_CAN_master->FM1R | ptr_CAN_master->FS1R)&msk)==0U) && // If identifier mask mode and dual 16-bit scale configuration
                     (((ptr_CAN_master->FFA1R >> bank)&1U)==obj_idx)) { // If bank has same FIFO assignment as requested
            if         (ptr_CAN_master->sFilterRegister[bank].FR1==0xFFFFFFFFU){// If n and n+1 entry are not used
              ptr_CAN_master->FA1R &= ~msk;                             // Put filter in inactive mode
              ptr_CAN_master->sFilterRegister[bank].FR1 = frx;          // id and mask
              break;
            } else if  (ptr_CAN_master->sFilterRegister[bank].FR2==0xFFFFFFFFU){// If n+2 and n+3 entry are not used
              ptr_CAN_master->FA1R &= ~msk;                             // Put filter in inactive mode
              ptr_CAN_master->sFilterRegister[bank].FR2 = frx;          // id and mask
              break;
            }
          }
          msk <<= 1U;
          bank++;
        }
        break;
    }
  }

  if (status == ARM_DRIVER_OK) {
    ptr_CAN_master->FA1R |= msk;                                        // Put filter in active mode
  }
  ptr_CAN_master->FMR  &= ~((uint32_t)CAN_FMR_FINIT);                   // Exit filter initialization mode

  return status;
}

/**
  \fn          int32_t CANx_RemoveFilter (uint32_t obj_idx, CAN_FILTER_TYPE filter_type, uint32_t id, uint32_t mask, uint8_t x)
  \brief       Remove receive filter for specified id or id with mask.
  \param[in]   obj_idx      Receive object index assignment
  \param[in]   filter_type  Type of filter to remove
                 - CAN_FILTER_TYPE_EXACT_ID:    exact id filter (id only)
                 - CAN_FILTER_TYPE_MASKABLE_ID: maskable filter (id and mask)
  \param[in]   id           Identifier
  \param[in]   mask         Identifier Mask
  \param[in]   x            Controller number (0..2)
  \return      execution status
*/
static int32_t CANx_RemoveFilter (uint32_t obj_idx, CAN_FILTER_TYPE filter_type, uint32_t id, uint32_t mask, uint8_t x) {
  CAN_TypeDef *ptr_CAN_master;
  uint32_t     fa1r, frx, fry, msk;
  int32_t      status;
  uint8_t      bank, bank_end;

  if (x >= CAN_CTRL_NUM)         { return ARM_DRIVER_ERROR;           }
  if (obj_idx >= CAN_RX_OBJ_NUM) { return ARM_DRIVER_ERROR_PARAMETER; }
  if ((filter_type != CAN_FILTER_TYPE_EXACT_ID) && 
      (filter_type != CAN_FILTER_TYPE_MASKABLE_ID)) 
                                 { return ARM_DRIVER_ERROR_PARAMETER; }
  if      (x == 2U)              { bank = 0U;                   bank_end = CAN3_FILTER_BANK_NUM;                        }
  else if (x == 1U)              { bank = CAN1_FILTER_BANK_NUM; bank_end = CAN1_FILTER_BANK_NUM + CAN2_FILTER_BANK_NUM; }
  else                           { bank = 0U;                   bank_end = CAN1_FILTER_BANK_NUM;                        }
  if (bank >= bank_end)          { return ARM_DRIVER_ERROR;           }

  if (x <= 1U) {
    ptr_CAN_master = ptr_CANx[0];
  } else {
    ptr_CAN_master = ptr_CANx[2];
  }

  status = ARM_DRIVER_OK;
  ptr_CAN_master->FMR |= CAN_FMR_FINIT;                                 // Enter filter initialization mode

  fa1r = ptr_CAN_master->FA1R;
  msk  = (uint32_t)1U << bank;

  if ((id & ARM_CAN_ID_IDE_Msk) != 0U) {                                // Extended Identifier
    frx = (id << 3) | CAN_FRx_32BIT_IDE;                                // id + IDE
    switch (filter_type) {
      case CAN_FILTER_TYPE_EXACT_ID:
        fry = (id << 3) | CAN_FRx_32BIT_IDE | CAN_FRx_32BIT_RTR;        // id + IDE + RTR
        while (bank <= bank_end) {                                      // Find empty place for id
          if (bank == bank_end) {                                       // If no free found exit
            status = ARM_DRIVER_ERROR;
            break;
          }
          if (((fa1r & msk) != 0U) &&                                   // If filter is active
             (((ptr_CAN_master->FM1R&ptr_CAN_master->FS1R)&msk)!=0U) && // If identifier list mode and single 32-bit scale configuration
             (((ptr_CAN_master->FFA1R >> bank) & 1U) == obj_idx) &&     // If bank has same FIFO assignment as requested
             (ptr_CAN_master->sFilterRegister[bank].FR1 == frx) &&
             (ptr_CAN_master->sFilterRegister[bank].FR2 == fry)) {
            ptr_CAN_master->FA1R &= ~msk;                               // Put filter in inactive mode
            ptr_CAN_master->sFilterRegister[bank].FR1 = 0xFFFFFFFFU;
            ptr_CAN_master->sFilterRegister[bank].FR2 = 0xFFFFFFFFU;
            break;
          }
          msk <<= 1U;
          bank++;
       }
        break;
      case CAN_FILTER_TYPE_MASKABLE_ID:
        fry = (mask << 3) | CAN_FRx_32BIT_IDE;                          // mask + IDE
        while (bank <= bank_end) {                                      // Find empty place for id
          if (bank == bank_end) {                                       // If no free found exit
            status = ARM_DRIVER_ERROR;
            break;
          }
          if (((fa1r & msk) != 0U) &&                                   // If filter is active
              ((ptr_CAN_master->FM1R & msk) == 0U) &&                   // If identifier mask mode
              ((ptr_CAN_master->FS1R & msk) != 0U) &&                   // If single 32-bit scale configuration
             (((ptr_CAN_master->FFA1R >> bank) & 1U) == obj_idx)  &&    // If bank has same FIFO assignment as requested
             (ptr_CAN_master->sFilterRegister[bank].FR1 == frx) &&
             (ptr_CAN_master->sFilterRegister[bank].FR2 == fry)) {
            ptr_CAN_master->FA1R &= ~msk;                               // Put filter in inactive mode
            ptr_CAN_master->sFilterRegister[bank].FR1 = 0xFFFFFFFFU;
            ptr_CAN_master->sFilterRegister[bank].FR2 = 0xFFFFFFFFU;
            break;
          }
          msk <<= 1U;
          bank++;
        }
        break;
    }
  } else {                                                              // Standard Identifier
    switch (filter_type) {
      case CAN_FILTER_TYPE_EXACT_ID:
        frx = ((id & 0xFFFFU) <<  5) |                                  // Low 16 bits = id
              ((id & 0xFFFFU) << 21) | CAN_FRx_16BIT_H_RTR;             // High 16 bits = id + RTR
        while (bank <= bank_end) {                                      // Find empty place for id
          if (bank == bank_end) {                                       // If no free found exit
            status = ARM_DRIVER_ERROR;
            break;
          }
          if (((fa1r & msk) != 0U)&&                                    // If filter is active
              ((ptr_CAN_master->FM1R & msk) != 0U) &&                   // If identifier list mode
              ((ptr_CAN_master->FS1R & msk) == 0U) &&                   // If dual 16-bit scale configuration
             (((ptr_CAN_master->FFA1R >> bank) & 1U) == obj_idx)) {     // If bank has same FIFO assignment as requested
            if        (ptr_CAN_master->sFilterRegister[bank].FR1==frx){
              ptr_CAN_master->FA1R &= ~msk;                             // Put filter in inactive mode
              ptr_CAN_master->sFilterRegister[bank].FR1 = 0xFFFFFFFFU;
              break;
            } else if (ptr_CAN_master->sFilterRegister[bank].FR2==frx){
              ptr_CAN_master->FA1R &= ~msk;                             // Put filter in inactive mode
              ptr_CAN_master->sFilterRegister[bank].FR2 = 0xFFFFFFFFU;
              break;
            }
          }
          msk <<= 1U;
          bank++;
        }
        break;
      case CAN_FILTER_TYPE_MASKABLE_ID:
        frx = ((id   & 0xFFFFU) <<  5) |                                // Low 16 bits = id
              ((mask & 0xFFFFU) << 21) ;                                // High 16 bits = mask
        if ((mask & ARM_CAN_ID_IDE_Msk) != 0U) {                        // If IDE masking enabled
          frx |= CAN_FRx_16BIT_H_IDE;                                   // High 16 bits = mask + IDE
        }
        while (bank <= bank_end) {                                      // Find empty place for id
          if (bank == bank_end) {                                       // If no free found exit
            status = ARM_DRIVER_ERROR;
            break;
          }
          if (((fa1r & msk) != 0U) &&                                   // If filter is active
             (((ptr_CAN_master->FM1R|ptr_CAN_master->FS1R)&msk)==0U) && // If identifier mask mode and dual 16-bit scale configuration
             (((ptr_CAN_master->FFA1R >> bank) & 1U) == obj_idx)) {     // If bank has same FIFO assignment as requested
            if        (ptr_CAN_master->sFilterRegister[bank].FR1==frx){
              ptr_CAN_master->FA1R &= ~msk;                             // Put filter in inactive mode
              ptr_CAN_master->sFilterRegister[bank].FR1 = 0xFFFFFFFFU;
              break;
            } else if (ptr_CAN_master->sFilterRegister[bank].FR2==frx){
              ptr_CAN_master->FA1R &= ~msk;                             // Put filter in inactive mode
              ptr_CAN_master->sFilterRegister[bank].FR2 = 0xFFFFFFFFU;
              break;
            }
          }
          msk <<= 1U;
          bank++;
        }
        break;
    }
  }

  if ((status == ARM_DRIVER_OK)                                   && 
     ((ptr_CAN_master->sFilterRegister[bank].FR1 != 0xFFFFFFFFU)  || 
      (ptr_CAN_master->sFilterRegister[bank].FR2 != 0xFFFFFFFFU))) {
    ptr_CAN_master->FA1R |= msk;                                        // Put filter in active mode
  }
  ptr_CAN_master->FMR  &= ~((uint32_t)CAN_FMR_FINIT);                   // Exit filter initialization mode

  return status;
}

// CAN Driver Functions

/**
  \fn          ARM_DRIVER_VERSION CAN_GetVersion (void)
  \brief       Get driver version.
  \return      ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION CAN_GetVersion (void) { return can_driver_version; }

/**
  \fn          ARM_CAN_CAPABILITIES CAN_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      ARM_CAN_CAPABILITIES
*/
static ARM_CAN_CAPABILITIES CAN_GetCapabilities (void) { return can_driver_capabilities; }

/**
  \fn          int32_t CANx_Initialize (ARM_CAN_SignalUnitEvent_t   cb_unit_event,
                                        ARM_CAN_SignalObjectEvent_t cb_object_event,
                                        uint8_t                     x)
  \brief       Initialize CAN interface and register signal (callback) functions.
  \param[in]   cb_unit_event   Pointer to ARM_CAN_SignalUnitEvent callback function
  \param[in]   cb_object_event Pointer to ARM_CAN_SignalObjectEvent callback function
  \param[in]   x               Controller number (0..2)
  \return      execution status
*/
static int32_t CANx_Initialize (ARM_CAN_SignalUnitEvent_t   cb_unit_event,
                                ARM_CAN_SignalObjectEvent_t cb_object_event,
                                uint8_t                     x) {
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
  GPIO_InitTypeDef GPIO_InitStruct;
#endif

  if (x >= CAN_CTRL_NUM)               { return ARM_DRIVER_ERROR; }
  if (can_driver_initialized[x] != 0U) { return ARM_DRIVER_OK;    }

  CAN_SignalUnitEvent  [x] = cb_unit_event;
  CAN_SignalObjectEvent[x] = cb_object_event;

#if (MX_CAN1 == 1U)
  if (x == 0U) {
#if defined (RTE_DEVICE_FRAMEWORK_CLASSIC)
#if defined (MX_CAN1_RX_Pin)
    Enable_GPIO_Clock          (MX_CAN1_RX_GPIOx);
    GPIO_InitStruct.Pin       = MX_CAN1_RX_GPIO_Pin;
    GPIO_InitStruct.Mode      = MX_CAN1_RX_GPIO_Mode;
    GPIO_InitStruct.Pull      = MX_CAN1_RX_GPIO_PuPd;
    GPIO_InitStruct.Speed     = MX_CAN1_RX_GPIO_Speed;
    GPIO_InitStruct.Alternate = MX_CAN1_RX_GPIO_AF;
    HAL_GPIO_Init              (MX_CAN1_RX_GPIOx, &GPIO_InitStruct);
#endif
#if defined (MX_CAN1_TX_Pin)
    Enable_GPIO_Clock          (MX_CAN1_TX_GPIOx);
    GPIO_InitStruct.Pin       = MX_CAN1_TX_GPIO_Pin;
    GPIO_InitStruct.Mode      = MX_CAN1_TX_GPIO_Mode;
    GPIO_InitStruct.Pull      = MX_CAN1_TX_GPIO_PuPd;
    GPIO_InitStruct.Speed     = MX_CAN1_TX_GPIO_Speed;
    GPIO_InitStruct.Alternate = MX_CAN1_TX_GPIO_AF;
    HAL_GPIO_Init              (MX_CAN1_TX_GPIOx, &GPIO_InitStruct);
#endif
#else
    hcan1.Instance = CAN1;
#endif
  }
#endif
#if (MX_CAN2 == 1U)
  if (x == 1U) {
#if defined (RTE_DEVICE_FRAMEWORK_CLASSIC)
#if defined (MX_CAN2_RX_Pin)
    Enable_GPIO_Clock          (MX_CAN2_RX_GPIOx);
    GPIO_InitStruct.Pin       = MX_CAN2_RX_GPIO_Pin;
    GPIO_InitStruct.Mode      = MX_CAN2_RX_GPIO_Mode;
    GPIO_InitStruct.Pull      = MX_CAN2_RX_GPIO_PuPd;
    GPIO_InitStruct.Speed     = MX_CAN2_RX_GPIO_Speed;
    GPIO_InitStruct.Alternate = MX_CAN2_RX_GPIO_AF;
    HAL_GPIO_Init              (MX_CAN2_RX_GPIOx, &GPIO_InitStruct);
#endif
#if defined (MX_CAN2_TX_Pin)
    Enable_GPIO_Clock          (MX_CAN2_TX_GPIOx);
    GPIO_InitStruct.Pin       = MX_CAN2_TX_GPIO_Pin;
    GPIO_InitStruct.Mode      = MX_CAN2_TX_GPIO_Mode;
    GPIO_InitStruct.Pull      = MX_CAN2_TX_GPIO_PuPd;
    GPIO_InitStruct.Speed     = MX_CAN2_TX_GPIO_Speed;
    GPIO_InitStruct.Alternate = MX_CAN2_TX_GPIO_AF;
    HAL_GPIO_Init              (MX_CAN2_TX_GPIOx, &GPIO_InitStruct);
#endif
#else
    hcan2.Instance = CAN2;
#endif
  }
#endif
#if (MX_CAN3 == 1U)
  if (x == 2U) {
#if defined (RTE_DEVICE_FRAMEWORK_CLASSIC)
#if defined (MX_CAN3_RX_Pin)
    Enable_GPIO_Clock          (MX_CAN3_RX_GPIOx);
    GPIO_InitStruct.Pin       = MX_CAN3_RX_GPIO_Pin;
    GPIO_InitStruct.Mode      = MX_CAN3_RX_GPIO_Mode;
    GPIO_InitStruct.Pull      = MX_CAN3_RX_GPIO_PuPd;
    GPIO_InitStruct.Speed     = MX_CAN3_RX_GPIO_Speed;
    GPIO_InitStruct.Alternate = MX_CAN3_RX_GPIO_AF;
    HAL_GPIO_Init              (MX_CAN3_RX_GPIOx, &GPIO_InitStruct);
#endif
#if defined (MX_CAN3_TX_Pin)
    Enable_GPIO_Clock          (MX_CAN3_TX_GPIOx);
    GPIO_InitStruct.Pin       = MX_CAN3_TX_GPIO_Pin;
    GPIO_InitStruct.Mode      = MX_CAN3_TX_GPIO_Mode;
    GPIO_InitStruct.Pull      = MX_CAN3_TX_GPIO_PuPd;
    GPIO_InitStruct.Speed     = MX_CAN3_TX_GPIO_Speed;
    GPIO_InitStruct.Alternate = MX_CAN3_TX_GPIO_AF;
    HAL_GPIO_Init              (MX_CAN3_TX_GPIOx, &GPIO_InitStruct);
#endif
#else
    hcan3.Instance = CAN3;
#endif
  }
#endif

  can_driver_initialized[x] = 1U;

  return ARM_DRIVER_OK;
}
#if (MX_CAN1 == 1U)
static int32_t CAN1_Initialize (ARM_CAN_SignalUnitEvent_t cb_unit_event, ARM_CAN_SignalObjectEvent_t cb_object_event) { return CANx_Initialize (cb_unit_event, cb_object_event, 0U); }
#endif
#if (MX_CAN2 == 1U)
static int32_t CAN2_Initialize (ARM_CAN_SignalUnitEvent_t cb_unit_event, ARM_CAN_SignalObjectEvent_t cb_object_event) { return CANx_Initialize (cb_unit_event, cb_object_event, 1U); }
#endif
#if (MX_CAN3 == 1U)
static int32_t CAN3_Initialize (ARM_CAN_SignalUnitEvent_t cb_unit_event, ARM_CAN_SignalObjectEvent_t cb_object_event) { return CANx_Initialize (cb_unit_event, cb_object_event, 2U); }
#endif

/**
  \fn          int32_t CANx_Uninitialize (uint8_t x)
  \brief       De-initialize CAN interface.
  \param[in]   x      Controller number (0..2)
  \return      execution status
*/
static int32_t CANx_Uninitialize (uint8_t x) {
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
  GPIO_InitTypeDef GPIO_InitStruct;
#endif

  if (x >= CAN_CTRL_NUM) { return ARM_DRIVER_ERROR; }

#if (MX_CAN1 == 1U)
  if (x == 0U) {
#if defined (RTE_DEVICE_FRAMEWORK_CLASSIC)
#if defined (MX_CAN1_RX_Pin)
    GPIO_InitStruct.Pin       = MX_CAN1_RX_GPIO_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = 0U;
    HAL_GPIO_Init              (MX_CAN1_RX_GPIOx, &GPIO_InitStruct);
#endif
#if defined (MX_CAN1_TX_Pin)
    GPIO_InitStruct.Pin       = MX_CAN1_TX_GPIO_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = 0U;
    HAL_GPIO_Init              (MX_CAN1_TX_GPIOx, &GPIO_InitStruct);
#endif
#else
    hcan1.Instance            = NULL;
#endif
  }
#endif
#if (MX_CAN2 == 1U)
  if (x == 1U) {
#if defined (RTE_DEVICE_FRAMEWORK_CLASSIC)
#if defined (MX_CAN2_RX_Pin)
    GPIO_InitStruct.Pin       = MX_CAN2_RX_GPIO_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = 0U;
    HAL_GPIO_Init              (MX_CAN2_RX_GPIOx, &GPIO_InitStruct);
#endif
#if defined (MX_CAN2_TX_Pin)
    GPIO_InitStruct.Pin       = MX_CAN2_TX_GPIO_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = 0U;
    HAL_GPIO_Init              (MX_CAN2_TX_GPIOx, &GPIO_InitStruct);
#endif
#else
    hcan2.Instance            = NULL;
#endif
  }
#endif
#if (MX_CAN3 == 1U)
  if (x == 2U) {
#if defined (RTE_DEVICE_FRAMEWORK_CLASSIC)
#if defined (MX_CAN3_RX_Pin)
    GPIO_InitStruct.Pin       = MX_CAN3_RX_GPIO_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = 0U;
    HAL_GPIO_Init              (MX_CAN3_RX_GPIOx, &GPIO_InitStruct);
#endif
#if defined (MX_CAN3_TX_Pin)
    GPIO_InitStruct.Pin       = MX_CAN3_TX_GPIO_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = 0U;
    HAL_GPIO_Init              (MX_CAN3_TX_GPIOx, &GPIO_InitStruct);
#endif
#else
    hcan3.Instance            = NULL;
#endif
  }
#endif

  can_driver_initialized[x] = 0U;

  return ARM_DRIVER_OK;
}
#if (MX_CAN1 == 1U)
static int32_t CAN1_Uninitialize (void) { return CANx_Uninitialize (0U); }
#endif
#if (MX_CAN2 == 1U)
static int32_t CAN2_Uninitialize (void) { return CANx_Uninitialize (1U); }
#endif
#if (MX_CAN3 == 1U)
static int32_t CAN3_Uninitialize (void) { return CANx_Uninitialize (2U); }
#endif

/**
  \fn          int32_t CANx_PowerControl (ARM_POWER_STATE state, uint8_t x)
  \brief       Control CAN interface power.
  \param[in]   state  Power state
                 - ARM_POWER_OFF :  power off: no operation possible
                 - ARM_POWER_LOW :  low power mode: retain state, detect and signal wake-up events
                 - ARM_POWER_FULL : power on: full operation at maximum performance
  \param[in]   x      Controller number (0..2)
  \return      execution status
*/
static int32_t CANx_PowerControl (ARM_POWER_STATE state, uint8_t x) {
  CAN_TypeDef *ptr_CAN_master;
  CAN_TypeDef *ptr_CAN;
  uint32_t     msk;
  uint8_t      bank, bank_end;

  if (x >= CAN_CTRL_NUM) { return ARM_DRIVER_ERROR;           }
  if ((state != ARM_POWER_OFF)  && 
      (state != ARM_POWER_FULL) && 
      (state != ARM_POWER_LOW)) 
                         { return ARM_DRIVER_ERROR_PARAMETER; }
  if      (x == 2U)      { bank = 0U;                   bank_end = CAN3_FILTER_BANK_NUM;                        }
  else if (x == 1U)      { bank = CAN1_FILTER_BANK_NUM; bank_end = CAN1_FILTER_BANK_NUM + CAN2_FILTER_BANK_NUM; }
  else                   { bank = 0U;                   bank_end = CAN1_FILTER_BANK_NUM;                        }
  if (bank >= bank_end)  { return ARM_DRIVER_ERROR;           }

  if (x <= 1U) {
    ptr_CAN_master = ptr_CANx[0];
  } else {
    ptr_CAN_master = ptr_CANx[2];
  }
  ptr_CAN = ptr_CANx[x];

  switch (state) {
    case ARM_POWER_OFF:
      can_driver_powered[x] = 0U;
#if (MX_CAN1 == 1U)
      if (x == 0U) {
        NVIC_DisableIRQ (CAN1_TX_IRQn);
        NVIC_DisableIRQ (CAN1_RX0_IRQn);
        NVIC_DisableIRQ (CAN1_RX1_IRQn);
        NVIC_DisableIRQ (CAN1_SCE_IRQn);
      }
#endif
#if (MX_CAN2 == 1U)
      if (x == 1U) {
        NVIC_DisableIRQ (CAN2_TX_IRQn);
        NVIC_DisableIRQ (CAN2_RX0_IRQn);
        NVIC_DisableIRQ (CAN2_RX1_IRQn);
        NVIC_DisableIRQ (CAN2_SCE_IRQn);
      }
#endif
#if (MX_CAN3 == 1U)
      if (x == 2U) {
        NVIC_DisableIRQ (CAN3_TX_IRQn);
        NVIC_DisableIRQ (CAN3_RX0_IRQn);
        NVIC_DisableIRQ (CAN3_RX1_IRQn);
        NVIC_DisableIRQ (CAN3_SCE_IRQn);
      }
#endif

#if (MX_CAN1 == 1U)
      if (x == 0U) {
        // Enable clock for CAN1 peripheral
        RCC->APB1ENR  |=  RCC_APB1ENR_CAN1EN;
      }
#endif
#if (MX_CAN2 == 1U)
      if (x == 1U) {
        // Enable clock for CAN1 and CAN2 peripheral
        RCC->APB1ENR  |=  RCC_APB1ENR_CAN1EN;
        RCC->APB1ENR  |=  RCC_APB1ENR_CAN2EN;
      }
#endif
#if (MX_CAN3 == 1U)
      if (x == 2U) {
        // Enable clock for CAN3 peripheral
        RCC->APB1ENR  |=  RCC_APB1ENR_CAN3EN;
      }
#endif

      ptr_CAN->IER = 0U;                        // Disable Interrupts

      ptr_CAN->MCR = CAN_MCR_RESET;             // Reset CAN controller
      while ((ptr_CAN->MCR & CAN_MCR_RESET) != 0U);

      memset((void *)&can_obj_cfg[x][0], 0, CAN_TOT_OBJ_NUM);

#if (MX_CAN1 == 1U)
      if (x == 0U) {
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
        RCC->APB1RSTR |=  RCC_APB1RSTR_CAN1RST;
        __NOP(); __NOP(); __NOP(); __NOP(); 
        RCC->APB1ENR  &= ~RCC_APB1ENR_CAN1EN;
#else
        if (hcan1.Instance != NULL) { HAL_CAN_DeInit (&hcan1); }
#endif
      }
#endif
#if (MX_CAN2 == 1U)
      if (x == 1U) {
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
        RCC->APB1RSTR |=  RCC_APB1RSTR_CAN2RST;
        __NOP(); __NOP(); __NOP(); __NOP(); 
        RCC->APB1ENR  &= ~RCC_APB1ENR_CAN2EN;
#else
        if (hcan2.Instance != NULL) { HAL_CAN_DeInit (&hcan2); }
#endif
#if (MX_CAN1 == 0U)
        RCC->APB1RSTR |=  RCC_APB1RSTR_CAN1RST;
        __NOP(); __NOP(); __NOP(); __NOP(); 
        RCC->APB1ENR  &= ~RCC_APB1ENR_CAN1EN;
#endif
      }
#endif
#if (MX_CAN3 == 1U)
      if (x == 2U) {
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
        RCC->APB1RSTR |=  RCC_APB1RSTR_CAN3RST;
        __NOP(); __NOP(); __NOP(); __NOP(); 
        RCC->APB1ENR  &= ~RCC_APB1ENR_CAN3EN;
#else
        if (hcan3.Instance != NULL) { HAL_CAN_DeInit (&hcan3); }
#endif
      }
#endif
      break;

    case ARM_POWER_FULL:
      if (can_driver_initialized[x] == 0U) { return ARM_DRIVER_ERROR; }
      if (can_driver_powered[x]     != 0U) { return ARM_DRIVER_OK;    }

#if (MX_CAN1 == 1U)
      if (x == 0U) {
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
        RCC->APB1ENR  |=  RCC_APB1ENR_CAN1EN;
        RCC->APB1RSTR |=  RCC_APB1RSTR_CAN1RST;
        __NOP(); __NOP(); __NOP(); __NOP(); 
        RCC->APB1RSTR &= ~RCC_APB1RSTR_CAN1RST;
#else
        HAL_CAN_MspInit(&hcan1);
#endif
      }
#endif
#if (MX_CAN2 == 1U)
      if (x == 1U) {
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
        RCC->APB1ENR  |=  RCC_APB1ENR_CAN2EN;
        RCC->APB1RSTR |=  RCC_APB1RSTR_CAN2RST;
        __NOP(); __NOP(); __NOP(); __NOP(); 
        RCC->APB1RSTR &= ~RCC_APB1RSTR_CAN2RST;
#else
        HAL_CAN_MspInit(&hcan2);
#endif
        if ((RCC->APB1ENR & RCC_APB1ENR_CAN1EN) == 0U) {
          // If CAN1 (master) clock is not enabled, enable it as it is necessary for filter operation
          RCC->APB1ENR  |=  RCC_APB1ENR_CAN1EN;
          RCC->APB1RSTR |=  RCC_APB1RSTR_CAN1RST;
          __NOP(); __NOP(); __NOP(); __NOP(); 
          RCC->APB1RSTR &= ~RCC_APB1RSTR_CAN1RST;
        }
      }
#endif
#if (MX_CAN3 == 1U)
      if (x == 2U) {
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
        RCC->APB1ENR  |=  RCC_APB1ENR_CAN3EN;
        RCC->APB1RSTR |=  RCC_APB1RSTR_CAN3RST;
        __NOP(); __NOP(); __NOP(); __NOP(); 
        RCC->APB1RSTR &= ~RCC_APB1RSTR_CAN3RST;
#else
        HAL_CAN_MspInit(&hcan3);
#endif
      }
#endif

      ptr_CAN->MCR = CAN_MCR_RESET;             // Reset CAN controller
      while ((ptr_CAN->MCR & CAN_MCR_RESET) != 0U);

      // Initialize filter banks
      if (x == 0U) {
        CAN1->FMR =  CAN_FMR_FINIT | ((CAN1_FILTER_BANK_NUM << 8) & CAN_FMR_CAN2SB);
      }
      msk         =  ((uint32_t)1U << bank_end) - ((uint32_t)1U << bank);
      ptr_CAN_master->FMR  |=  CAN_FMR_FINIT;   // Enter filter initialization mode
      ptr_CAN_master->FA1R &= ~msk;             // Put all filters in inactive mode
      ptr_CAN_master->FM1R &= ~msk;             // Initialize all filters mode
      ptr_CAN_master->FS1R &= ~msk;             // Initialize all filters scale configuration
      while (bank <= bank_end) {                // Go through all banks
        if (bank == bank_end) { break; }
        ptr_CAN_master->sFilterRegister[bank].FR1 = 0U;
        ptr_CAN_master->sFilterRegister[bank].FR2 = 0U;
        bank++;
      }

      memset((void *)&can_obj_cfg[x][0], 0, CAN_TOT_OBJ_NUM);

      ptr_CAN->IER =   CAN_IER_TMEIE  |         // Enable Interrupts
                       CAN_IER_FMPIE0 |
                       CAN_IER_FOVIE0 |
                       CAN_IER_FMPIE1 |
                       CAN_IER_FOVIE1 |
                       CAN_IER_EWGIE  |
                       CAN_IER_EPVIE  |
                       CAN_IER_BOFIE  |
                       CAN_IER_ERRIE  ;

      can_driver_powered[x] = 1U;

#if (MX_CAN1 == 1U)
      if (x == 0U) {
        if ((CAN_SignalUnitEvent[0] != NULL) || (CAN_SignalObjectEvent[0] != NULL)) {
          NVIC_ClearPendingIRQ (CAN1_TX_IRQn);
          NVIC_EnableIRQ       (CAN1_TX_IRQn);
          NVIC_ClearPendingIRQ (CAN1_RX0_IRQn);
          NVIC_EnableIRQ       (CAN1_RX0_IRQn);
          NVIC_ClearPendingIRQ (CAN1_RX1_IRQn);
          NVIC_EnableIRQ       (CAN1_RX1_IRQn);
        }
        if (CAN_SignalUnitEvent[0] != NULL) {
          NVIC_ClearPendingIRQ (CAN1_SCE_IRQn);
          NVIC_EnableIRQ       (CAN1_SCE_IRQn);
        }
      }
#endif
#if (MX_CAN2 == 1U)
      if (x == 1U) {
        if ((CAN_SignalUnitEvent[1] != NULL) || (CAN_SignalObjectEvent[1] != NULL)) {
          NVIC_ClearPendingIRQ (CAN2_TX_IRQn);
          NVIC_EnableIRQ       (CAN2_TX_IRQn);
          NVIC_ClearPendingIRQ (CAN2_RX0_IRQn);
          NVIC_EnableIRQ       (CAN2_RX0_IRQn);
          NVIC_ClearPendingIRQ (CAN2_RX1_IRQn);
          NVIC_EnableIRQ       (CAN2_RX1_IRQn);
        }
        if (CAN_SignalUnitEvent[1] != NULL) {
          NVIC_ClearPendingIRQ (CAN2_SCE_IRQn);
          NVIC_EnableIRQ       (CAN2_SCE_IRQn);
        }
      }
#endif
#if (MX_CAN3 == 1U)
      if (x == 2U) {
        if ((CAN_SignalUnitEvent[2] != NULL) || (CAN_SignalObjectEvent[2] != NULL)) {
          NVIC_ClearPendingIRQ (CAN3_TX_IRQn);
          NVIC_EnableIRQ       (CAN3_TX_IRQn);
          NVIC_ClearPendingIRQ (CAN3_RX0_IRQn);
          NVIC_EnableIRQ       (CAN3_RX0_IRQn);
          NVIC_ClearPendingIRQ (CAN3_RX1_IRQn);
          NVIC_EnableIRQ       (CAN3_RX1_IRQn);
        }
        if (CAN_SignalUnitEvent[2] != NULL) {
          NVIC_ClearPendingIRQ (CAN3_SCE_IRQn);
          NVIC_EnableIRQ       (CAN3_SCE_IRQn);
        }
      }
#endif
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}
#if (MX_CAN1 == 1U)
static int32_t CAN1_PowerControl (ARM_POWER_STATE state) { return CANx_PowerControl (state, 0U); }
#endif
#if (MX_CAN2 == 1U)
static int32_t CAN2_PowerControl (ARM_POWER_STATE state) { return CANx_PowerControl (state, 1U); }
#endif
#if (MX_CAN3 == 1U)
static int32_t CAN3_PowerControl (ARM_POWER_STATE state) { return CANx_PowerControl (state, 2U); }
#endif

/**
  \fn          uint32_t CAN_GetClock (void)
  \brief       Retrieve CAN base clock frequency.
  \return      base clock frequency
*/
static uint32_t CAN_GetClock (void) {
  return HAL_RCC_GetPCLK1Freq();
}

/**
  \fn          int32_t CANx_SetBitrate (ARM_CAN_BITRATE_SELECT select, uint32_t bitrate, uint32_t bit_segments, uint8_t x)
  \brief       Set bitrate for CAN interface.
  \param[in]   select       Bitrate selection
                 - ARM_CAN_BITRATE_NOMINAL : nominal (flexible data-rate arbitration) bitrate
                 - ARM_CAN_BITRATE_FD_DATA : flexible data-rate data bitrate
  \param[in]   bitrate      Bitrate
  \param[in]   bit_segments Bit segments settings
  \param[in]   x            Controller number (0..2)
  \return      execution status
*/
static int32_t CANx_SetBitrate (ARM_CAN_BITRATE_SELECT select, uint32_t bitrate, uint32_t bit_segments, uint8_t x) {
  CAN_TypeDef *ptr_CAN;
  uint32_t     mcr, sjw, prop_seg, phase_seg1, phase_seg2, pclk, brp, tq_num;

  if (x >= CAN_CTRL_NUM)                 { return ARM_DRIVER_ERROR;               }
  if (select != ARM_CAN_BITRATE_NOMINAL) { return ARM_CAN_INVALID_BITRATE_SELECT; }
  if (can_driver_powered[x] == 0U)       { return ARM_DRIVER_ERROR;               }

  prop_seg   = (bit_segments & ARM_CAN_BIT_PROP_SEG_Msk  ) >> ARM_CAN_BIT_PROP_SEG_Pos;
  phase_seg1 = (bit_segments & ARM_CAN_BIT_PHASE_SEG1_Msk) >> ARM_CAN_BIT_PHASE_SEG1_Pos;
  phase_seg2 = (bit_segments & ARM_CAN_BIT_PHASE_SEG2_Msk) >> ARM_CAN_BIT_PHASE_SEG2_Pos;
  sjw        = (bit_segments & ARM_CAN_BIT_SJW_Msk       ) >> ARM_CAN_BIT_SJW_Pos;

  if (((prop_seg + phase_seg1) < 1U) || ((prop_seg + phase_seg1) > 16U)) { return ARM_CAN_INVALID_BIT_PROP_SEG;   }
  if (( phase_seg2             < 1U) || ( phase_seg2             >  8U)) { return ARM_CAN_INVALID_BIT_PHASE_SEG2; }
  if (( sjw                    < 1U) || ( sjw                    >  4U)) { return ARM_CAN_INVALID_BIT_SJW;        }

  ptr_CAN = ptr_CANx[x];

  tq_num = 1U + prop_seg + phase_seg1 + phase_seg2;
  pclk   = CAN_GetClock ();           if (pclk == 0U)  { return ARM_DRIVER_ERROR;        }
  brp    = pclk / (tq_num * bitrate); if (brp > 1024U) { return ARM_CAN_INVALID_BITRATE; }
  if (pclk > (brp * tq_num * bitrate)) {
    if ((((pclk - (brp * tq_num * bitrate)) * 1024U) / pclk) > CAN_CLOCK_TOLERANCE) { return ARM_CAN_INVALID_BITRATE; }
  } else if (pclk < (brp * tq_num * bitrate)) {
    if (((((brp * tq_num * bitrate) - pclk) * 1024U) / pclk) > CAN_CLOCK_TOLERANCE) { return ARM_CAN_INVALID_BITRATE; }
  }

  mcr = ptr_CAN->MCR;
  ptr_CAN->MCR = CAN_MCR_INRQ;                  // Activate initialization mode
  while ((ptr_CAN->MSR&CAN_MSR_INAK) == 0U);    // Wait to enter initialization mode

  ptr_CAN->BTR = (ptr_CAN->BTR & (CAN_BTR_LBKM | CAN_BTR_SILM)) | ((brp - 1U) & CAN_BTR_BRP) | ((sjw - 1U) << 24) | ((phase_seg2 - 1U) << 20) | ((prop_seg + phase_seg1 - 1U) << 16);
  ptr_CAN->MCR =  mcr;                          // Return to previous mode

  return ARM_DRIVER_OK;
}
#if (MX_CAN1 == 1U)
static int32_t CAN1_SetBitrate (ARM_CAN_BITRATE_SELECT select, uint32_t bitrate, uint32_t bit_segments) { return CANx_SetBitrate (select, bitrate, bit_segments, 0U); }
#endif
#if (MX_CAN2 == 1U)
static int32_t CAN2_SetBitrate (ARM_CAN_BITRATE_SELECT select, uint32_t bitrate, uint32_t bit_segments) { return CANx_SetBitrate (select, bitrate, bit_segments, 1U); }
#endif
#if (MX_CAN3 == 1U)
static int32_t CAN3_SetBitrate (ARM_CAN_BITRATE_SELECT select, uint32_t bitrate, uint32_t bit_segments) { return CANx_SetBitrate (select, bitrate, bit_segments, 2U); }
#endif

/**
  \fn          int32_t CANx_SetMode (ARM_CAN_MODE mode, uint8_t x)
  \brief       Set operating mode for CAN interface.
  \param[in]   mode   Operating mode
                 - ARM_CAN_MODE_INITIALIZATION :    initialization mode
                 - ARM_CAN_MODE_NORMAL :            normal operation mode
                 - ARM_CAN_MODE_RESTRICTED :        restricted operation mode
                 - ARM_CAN_MODE_MONITOR :           bus monitoring mode
                 - ARM_CAN_MODE_LOOPBACK_INTERNAL : loopback internal mode
                 - ARM_CAN_MODE_LOOPBACK_EXTERNAL : loopback external mode
  \param[in]   x      Controller number (0..2)
  \return      execution status
*/
static int32_t CANx_SetMode (ARM_CAN_MODE mode, uint8_t x) {
  CAN_TypeDef *ptr_CAN_master;
  CAN_TypeDef *ptr_CAN;
  uint32_t     event;

  if (x >= CAN_CTRL_NUM) { return ARM_DRIVER_ERROR;           }
  if ((mode != ARM_CAN_MODE_INITIALIZATION)    &&
      (mode != ARM_CAN_MODE_NORMAL)            &&
      (mode != ARM_CAN_MODE_RESTRICTED)        &&
      (mode != ARM_CAN_MODE_MONITOR)           &&
      (mode != ARM_CAN_MODE_LOOPBACK_INTERNAL) &&
      (mode != ARM_CAN_MODE_LOOPBACK_EXTERNAL)) 
                         { return ARM_DRIVER_ERROR_PARAMETER; }

  if (x <= 1U) {
    ptr_CAN_master = ptr_CANx[0];
  } else {
    ptr_CAN_master = ptr_CANx[2];
  }
  ptr_CAN = ptr_CANx[x];

  event = 0U;
  switch (mode) {
    case ARM_CAN_MODE_INITIALIZATION:
      ptr_CAN_master->FMR |=  CAN_FMR_FINIT;    // Filter initialization mode
      ptr_CAN->MCR  =  CAN_MCR_INRQ;            // Enter initialization mode
      while ((ptr_CAN->MSR&CAN_MSR_INAK)==0U);  // Wait to enter initialization mode
      event = ARM_CAN_EVENT_UNIT_BUS_OFF;
      break;
    case ARM_CAN_MODE_NORMAL:
      ptr_CAN->MCR |=  CAN_MCR_INRQ;            // Enter initialization mode
      while ((ptr_CAN->MSR&CAN_MSR_INAK)==0U);  // Wait to enter initialization mode
      ptr_CAN->BTR &=~(CAN_BTR_LBKM | CAN_BTR_SILM);
      ptr_CAN->MCR  =  CAN_MCR_ABOM |           // Activate automatic bus-off
                       CAN_MCR_AWUM ;           // Enable automatic wakeup mode
      while ((ptr_CAN->MSR&CAN_MSR_INAK)!=0U);  // Wait to exit initialization mode
      ptr_CAN_master->FMR &= ~((uint32_t)CAN_FMR_FINIT);        // Filter active mode
      event = ARM_CAN_EVENT_UNIT_ACTIVE;
      break;
    case ARM_CAN_MODE_RESTRICTED:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
    case ARM_CAN_MODE_MONITOR:
      ptr_CAN->MCR |=  CAN_MCR_INRQ;            // Enter initialization mode
      while ((ptr_CAN->MSR&CAN_MSR_INAK)==0U);  // Wait to enter initialization mode
      ptr_CAN->BTR &= ~CAN_BTR_LBKM;            // Deactivate loopback
      ptr_CAN->BTR |=  CAN_BTR_SILM;            // Activate silent
      ptr_CAN->MCR &= ~CAN_MCR_INRQ;            // Deactivate initialization mode
      while ((ptr_CAN->MSR&CAN_MSR_INAK)!=0U);  // Wait to exit initialization mode
      event = ARM_CAN_EVENT_UNIT_PASSIVE;
      break;
    case ARM_CAN_MODE_LOOPBACK_INTERNAL:
      ptr_CAN->MCR |=  CAN_MCR_INRQ;            // Enter initialization mode
      while ((ptr_CAN->MSR&CAN_MSR_INAK)==0U);  // Wait to enter initialization mode
      ptr_CAN->BTR |=  CAN_BTR_LBKM;            // Activate loopback
      ptr_CAN->BTR |=  CAN_BTR_SILM;            // Activate silent
      ptr_CAN->MCR &= ~CAN_MCR_INRQ;            // Deactivate initialization mode
      while ((ptr_CAN->MSR&CAN_MSR_INAK)!=0U);  // Wait to exit initialization mode
      event = ARM_CAN_EVENT_UNIT_PASSIVE;
      break;
    case ARM_CAN_MODE_LOOPBACK_EXTERNAL:
      ptr_CAN->MCR |=  CAN_MCR_INRQ;            // Enter initialization mode
      while ((ptr_CAN->MSR&CAN_MSR_INAK)==0U);  // Wait to enter initialization mode
      ptr_CAN->BTR &= ~CAN_BTR_SILM;            // Deactivate silent
      ptr_CAN->BTR |=  CAN_BTR_LBKM;            // Activate loopback
      ptr_CAN->MCR &= ~CAN_MCR_INRQ;            // Deactivate initialization mode
      while ((ptr_CAN->MSR&CAN_MSR_INAK)!=0U);  // Wait to exit initialization mode
      event = ARM_CAN_EVENT_UNIT_ACTIVE;
      break;
  }
  if ((CAN_SignalUnitEvent[x] != NULL) && (event != 0U)) { CAN_SignalUnitEvent[x](event); }

  return ARM_DRIVER_OK;
}
#if (MX_CAN1 == 1U)
static int32_t CAN1_SetMode (ARM_CAN_MODE mode) { return CANx_SetMode (mode, 0U); }
#endif
#if (MX_CAN2 == 1U)
static int32_t CAN2_SetMode (ARM_CAN_MODE mode) { return CANx_SetMode (mode, 1U); }
#endif
#if (MX_CAN3 == 1U)
static int32_t CAN3_SetMode (ARM_CAN_MODE mode) { return CANx_SetMode (mode, 2U); }
#endif

/**
  \fn          ARM_CAN_OBJ_CAPABILITIES CANx_ObjectGetCapabilities (uint32_t obj_idx, uint8_t x)
  \brief       Retrieve capabilities of an object.
  \param[in]   obj_idx  Object index
  \param[in]   x        Controller number (0..2)
  \return      ARM_CAN_OBJ_CAPABILITIES
*/
static ARM_CAN_OBJ_CAPABILITIES CANx_ObjectGetCapabilities (uint32_t obj_idx, uint8_t x) {
  ARM_CAN_OBJ_CAPABILITIES obj_cap_null;

  if ((x >= CAN_CTRL_NUM) || (obj_idx >= CAN_TOT_OBJ_NUM)) {
    memset ((void *)&obj_cap_null, 0, sizeof(ARM_CAN_OBJ_CAPABILITIES));
    return obj_cap_null;
  }

  if (obj_idx >= CAN_RX_OBJ_NUM) {
    return can_object_capabilities_tx;
  } else {
    return can_object_capabilities_rx;
  }
}
#if (MX_CAN1 == 1U)
static ARM_CAN_OBJ_CAPABILITIES CAN1_ObjectGetCapabilities (uint32_t obj_idx) { return CANx_ObjectGetCapabilities (obj_idx, 0U); }
#endif
#if (MX_CAN2 == 1U)
static ARM_CAN_OBJ_CAPABILITIES CAN2_ObjectGetCapabilities (uint32_t obj_idx) { return CANx_ObjectGetCapabilities (obj_idx, 1U); }
#endif
#if (MX_CAN3 == 1U)
static ARM_CAN_OBJ_CAPABILITIES CAN3_ObjectGetCapabilities (uint32_t obj_idx) { return CANx_ObjectGetCapabilities (obj_idx, 2U); }
#endif

/**
  \fn          int32_t CANx_ObjectSetFilter (uint32_t obj_idx, ARM_CAN_FILTER_OPERATION operation, uint32_t id, uint32_t arg, uint8_t x)
  \brief       Add or remove filter for message reception.
  \param[in]   obj_idx      Object index of object that filter should be or is assigned to
  \param[in]   operation    Operation on filter
                 - ARM_CAN_FILTER_ID_EXACT_ADD :       add    exact id filter
                 - ARM_CAN_FILTER_ID_EXACT_REMOVE :    remove exact id filter
                 - ARM_CAN_FILTER_ID_RANGE_ADD :       add    range id filter
                 - ARM_CAN_FILTER_ID_RANGE_REMOVE :    remove range id filter
                 - ARM_CAN_FILTER_ID_MASKABLE_ADD :    add    maskable id filter
                 - ARM_CAN_FILTER_ID_MASKABLE_REMOVE : remove maskable id filter
  \param[in]   id           ID or start of ID range (depending on filter type)
  \param[in]   arg          Mask or end of ID range (depending on filter type)
  \param[in]   x            Controller number (0..2)
  \return      execution status
*/
static int32_t CANx_ObjectSetFilter (uint32_t obj_idx, ARM_CAN_FILTER_OPERATION operation, uint32_t id, uint32_t arg, uint8_t x) {
  CAN_TypeDef *ptr_CAN_master;
  int32_t      status;

  if (x >= CAN_CTRL_NUM)           { return ARM_DRIVER_ERROR;           }
  if (obj_idx >= CAN_RX_OBJ_NUM)   { return ARM_DRIVER_ERROR_PARAMETER; }
  if ((operation != ARM_CAN_FILTER_ID_EXACT_ADD)       && 
      (operation != ARM_CAN_FILTER_ID_EXACT_REMOVE)    && 
      (operation != ARM_CAN_FILTER_ID_RANGE_ADD)       && 
      (operation != ARM_CAN_FILTER_ID_RANGE_REMOVE)    && 
      (operation != ARM_CAN_FILTER_ID_MASKABLE_ADD)    && 
      (operation != ARM_CAN_FILTER_ID_MASKABLE_REMOVE)) 
                                   { return ARM_DRIVER_ERROR_PARAMETER; }
  if (can_driver_powered[x] == 0U) { return ARM_DRIVER_ERROR;           }

  if (x <= 1U) {
    ptr_CAN_master = ptr_CANx[0];
  } else {
    ptr_CAN_master = ptr_CANx[2];
  }

  ptr_CAN_master->FMR |=  CAN_FMR_FINIT;        // Filter initialization mode

  switch (operation) {
    case ARM_CAN_FILTER_ID_EXACT_ADD:
      status = CANx_AddFilter   (obj_idx, CAN_FILTER_TYPE_EXACT_ID,    id,  0U, x);
      break;
    case ARM_CAN_FILTER_ID_MASKABLE_ADD:
      status = CANx_AddFilter   (obj_idx, CAN_FILTER_TYPE_MASKABLE_ID, id, arg, x);
      break;
    case ARM_CAN_FILTER_ID_EXACT_REMOVE:
      status = CANx_RemoveFilter(obj_idx, CAN_FILTER_TYPE_EXACT_ID,    id,  0U, x);
      break;
    case ARM_CAN_FILTER_ID_MASKABLE_REMOVE:
      status = CANx_RemoveFilter(obj_idx, CAN_FILTER_TYPE_MASKABLE_ID, id, arg, x);
      break;
    case ARM_CAN_FILTER_ID_RANGE_ADD:
    case ARM_CAN_FILTER_ID_RANGE_REMOVE:
      status = ARM_DRIVER_ERROR_UNSUPPORTED;
      break;
  }
  ptr_CAN_master->FMR &= ~((uint32_t)CAN_FMR_FINIT);    // Filter active mode

  return status;
}
#if (MX_CAN1 == 1U)
static int32_t CAN1_ObjectSetFilter (uint32_t obj_idx, ARM_CAN_FILTER_OPERATION operation, uint32_t id, uint32_t arg) { return CANx_ObjectSetFilter (obj_idx, operation, id, arg, 0U); }
#endif
#if (MX_CAN2 == 1U)
static int32_t CAN2_ObjectSetFilter (uint32_t obj_idx, ARM_CAN_FILTER_OPERATION operation, uint32_t id, uint32_t arg) { return CANx_ObjectSetFilter (obj_idx, operation, id, arg, 1U); }
#endif
#if (MX_CAN3 == 1U)
static int32_t CAN3_ObjectSetFilter (uint32_t obj_idx, ARM_CAN_FILTER_OPERATION operation, uint32_t id, uint32_t arg) { return CANx_ObjectSetFilter (obj_idx, operation, id, arg, 2U); }
#endif

/**
  \fn          int32_t CANx_ObjectConfigure (uint32_t obj_idx, ARM_CAN_OBJ_CONFIG obj_cfg, uint8_t x)
  \brief       Configure object.
  \param[in]   obj_idx  Object index
  \param[in]   obj_cfg  Object configuration state
                 - ARM_CAN_OBJ_INACTIVE :       deactivate object
                 - ARM_CAN_OBJ_RX :             configure object for reception
                 - ARM_CAN_OBJ_TX :             configure object for transmission
                 - ARM_CAN_OBJ_RX_RTR_TX_DATA : configure object that on RTR reception automatically transmits Data Frame
                 - ARM_CAN_OBJ_TX_RTR_RX_DATA : configure object that transmits RTR and automatically receives Data Frame
  \param[in]   x        Controller number (0..2)
  \return      execution status
*/
static int32_t CANx_ObjectConfigure (uint32_t obj_idx, ARM_CAN_OBJ_CONFIG obj_cfg, uint8_t x) {

  if (x >= CAN_CTRL_NUM)           { return ARM_DRIVER_ERROR;           }
  if (obj_idx >= CAN_TOT_OBJ_NUM)  { return ARM_DRIVER_ERROR_PARAMETER; }
  if ((obj_cfg != ARM_CAN_OBJ_INACTIVE)       &&
      (obj_cfg != ARM_CAN_OBJ_TX)             &&
      (obj_cfg != ARM_CAN_OBJ_RX)             &&
      (obj_cfg != ARM_CAN_OBJ_RX_RTR_TX_DATA) &&
      (obj_cfg != ARM_CAN_OBJ_TX_RTR_RX_DATA)) 
                                   { return ARM_DRIVER_ERROR_PARAMETER; }
  if (can_driver_powered[x] == 0U) { return ARM_DRIVER_ERROR;           }

  switch (obj_cfg) {
    case ARM_CAN_OBJ_INACTIVE:
      can_obj_cfg[x][obj_idx] = ARM_CAN_OBJ_INACTIVE;
      break;
    case ARM_CAN_OBJ_RX_RTR_TX_DATA:
    case ARM_CAN_OBJ_TX_RTR_RX_DATA:
      can_obj_cfg[x][obj_idx] = ARM_CAN_OBJ_INACTIVE;
      return ARM_DRIVER_ERROR_UNSUPPORTED;
    case ARM_CAN_OBJ_TX:
      if (obj_idx < CAN_RX_OBJ_NUM)  { return ARM_DRIVER_ERROR_PARAMETER; }
      can_obj_cfg[x][obj_idx] = ARM_CAN_OBJ_TX;
      break;
    case ARM_CAN_OBJ_RX:
      if (obj_idx >= CAN_RX_OBJ_NUM) { return ARM_DRIVER_ERROR_PARAMETER; }
      can_obj_cfg[x][obj_idx] = ARM_CAN_OBJ_RX;
      break;
  }

  return ARM_DRIVER_OK;
}
#if (MX_CAN1 == 1U)
static int32_t CAN1_ObjectConfigure (uint32_t obj_idx, ARM_CAN_OBJ_CONFIG obj_cfg) { return CANx_ObjectConfigure (obj_idx, obj_cfg, 0U); }
#endif
#if (MX_CAN2 == 1U)
static int32_t CAN2_ObjectConfigure (uint32_t obj_idx, ARM_CAN_OBJ_CONFIG obj_cfg) { return CANx_ObjectConfigure (obj_idx, obj_cfg, 1U); }
#endif
#if (MX_CAN3 == 1U)
static int32_t CAN3_ObjectConfigure (uint32_t obj_idx, ARM_CAN_OBJ_CONFIG obj_cfg) { return CANx_ObjectConfigure (obj_idx, obj_cfg, 2U); }
#endif

/**
  \fn          int32_t CANx_MessageSend (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, const uint8_t *data, uint8_t size, uint8_t x)
  \brief       Send message on CAN bus.
  \param[in]   obj_idx  Object index
  \param[in]   msg_info Pointer to CAN message information
  \param[in]   data     Pointer to data buffer
  \param[in]   size     Number of data bytes to send
  \param[in]   x        Controller number (0..2)
  \return      value >= 0  number of data bytes accepted to send
  \return      value < 0   execution status
*/
static int32_t CANx_MessageSend (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, const uint8_t *data, uint8_t size, uint8_t x) {
  CAN_TypeDef *ptr_CAN;
  uint32_t     data_tx[2];
  uint32_t     tir;

  if (x >= CAN_CTRL_NUM)                                          { return ARM_DRIVER_ERROR;           }
  if ((obj_idx < CAN_RX_OBJ_NUM) || (obj_idx >= CAN_TOT_OBJ_NUM)) { return ARM_DRIVER_ERROR_PARAMETER; }
  if (can_driver_powered[x] == 0U)                                { return ARM_DRIVER_ERROR;           }
  if (can_obj_cfg[x][obj_idx] != ARM_CAN_OBJ_TX)                  { return ARM_DRIVER_ERROR;           }

  obj_idx -= CAN_RX_OBJ_NUM;                            // obj_idx origin to 0

  ptr_CAN  = ptr_CANx[x];

  if ((ptr_CAN->sTxMailBox[obj_idx].TIR & CAN_TI0R_TXRQ) != 0U) { return ARM_DRIVER_ERROR_BUSY; }

  if ((msg_info->id & ARM_CAN_ID_IDE_Msk) != 0U) {      // Extended Identifier
    tir = (msg_info->id <<  3) | CAN_TI0R_IDE;
  } else {                                              // Standard Identifier
    tir = (msg_info->id << 21);
  }

  if (size > 8U) { size = 8U; }

  if (msg_info->rtr != 0U) {                            // If send RTR requested
    size = 0U;
    tir |= CAN_TI0R_RTR;

    ptr_CAN->sTxMailBox[obj_idx].TDTR &= ~CAN_TDT0R_DLC;
    ptr_CAN->sTxMailBox[obj_idx].TDTR |=  msg_info->dlc & CAN_TDT0R_DLC;
  } else {
    if (size != 8U) {
      data_tx[0] = 0U;
      data_tx[1] = 0U;
    }
    memcpy((uint8_t *)(&data_tx[0]), data, size);

    ptr_CAN->sTxMailBox[obj_idx].TDLR = data_tx[0];
    ptr_CAN->sTxMailBox[obj_idx].TDHR = data_tx[1];

    ptr_CAN->sTxMailBox[obj_idx].TDTR &= ~CAN_TDT0R_DLC;
    ptr_CAN->sTxMailBox[obj_idx].TDTR |=  size & CAN_TDT0R_DLC;
  }

  ptr_CAN->sTxMailBox[obj_idx].TIR   =  tir | CAN_TI0R_TXRQ;    // Activate transmit

  return ((int32_t)size);
}
#if (MX_CAN1 == 1U)
static int32_t CAN1_MessageSend (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, const uint8_t *data, uint8_t size) { return CANx_MessageSend (obj_idx, msg_info, data, size, 0U); }
#endif
#if (MX_CAN2 == 1U)
static int32_t CAN2_MessageSend (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, const uint8_t *data, uint8_t size) { return CANx_MessageSend (obj_idx, msg_info, data, size, 1U); }
#endif
#if (MX_CAN3 == 1U)
static int32_t CAN3_MessageSend (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, const uint8_t *data, uint8_t size) { return CANx_MessageSend (obj_idx, msg_info, data, size, 2U); }
#endif

/**
  \fn          int32_t CANx_MessageRead (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, uint8_t *data, uint8_t size, uint8_t x)
  \brief       Read message received on CAN bus.
  \param[in]   obj_idx  Object index
  \param[out]  msg_info Pointer to read CAN message information
  \param[out]  data     Pointer to data buffer for read data
  \param[in]   size     Maximum number of data bytes to read
  \param[in]   x        Controller number (0..2)
  \return      value >= 0  number of data bytes read
  \return      value < 0   execution status
*/
static int32_t CANx_MessageRead (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, uint8_t *data, uint8_t size, uint8_t x) {
  CAN_TypeDef *ptr_CAN;
  uint32_t     data_rx[2][2];

  if (x >= CAN_CTRL_NUM)                         { return ARM_DRIVER_ERROR;           }
  if (obj_idx >= CAN_RX_OBJ_NUM)                 { return ARM_DRIVER_ERROR_PARAMETER; }
  if (can_driver_powered[x] == 0U)               { return ARM_DRIVER_ERROR;           }
  if (can_obj_cfg[x][obj_idx] != ARM_CAN_OBJ_RX) { return ARM_DRIVER_ERROR;           }

  ptr_CAN = ptr_CANx[x];

  if (size > 8U) { size = 8U; }

  if ((ptr_CAN->sFIFOMailBox[obj_idx].RIR & CAN_RI0R_IDE) != 0U) {      // Extended Identifier
    msg_info->id = (0x1FFFFFFFUL & (ptr_CAN->sFIFOMailBox[obj_idx].RIR >>  3)) | ARM_CAN_ID_IDE_Msk;
  } else {                                              // Standard Identifier
    msg_info->id = (    0x07FFUL & (ptr_CAN->sFIFOMailBox[obj_idx].RIR >> 21));
  }

  if ((ptr_CAN->sFIFOMailBox[obj_idx].RIR & CAN_RI0R_RTR) != 0U) {
    msg_info->rtr = 1U;
    size          = 0U;
  } else {
    msg_info->rtr = 0U;
  }

  msg_info->dlc = ptr_CAN->sFIFOMailBox[obj_idx].RDTR & CAN_RDT0R_DLC;

  if (size > 0U) {
    data_rx[x][0] = ptr_CAN->sFIFOMailBox[obj_idx].RDLR;
    data_rx[x][1] = ptr_CAN->sFIFOMailBox[obj_idx].RDHR;

    memcpy(data, (uint8_t *)(&data_rx[x][0]), size);
  }

  if (obj_idx == 1U) {
    ptr_CAN->RF1R = CAN_RF1R_RFOM1;                     // Release FIFO 1 output mailbox
  } else {
    ptr_CAN->RF0R = CAN_RF0R_RFOM0;                     // Release FIFO 0 output mailbox
  }

  return ((int32_t)size);
}
#if (MX_CAN1 == 1U)
static int32_t CAN1_MessageRead (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, uint8_t *data, uint8_t size) { return CANx_MessageRead (obj_idx, msg_info, data, size, 0U); }
#endif
#if (MX_CAN2 == 1U)
static int32_t CAN2_MessageRead (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, uint8_t *data, uint8_t size) { return CANx_MessageRead (obj_idx, msg_info, data, size, 1U); }
#endif
#if (MX_CAN3 == 1U)
static int32_t CAN3_MessageRead (uint32_t obj_idx, ARM_CAN_MSG_INFO *msg_info, uint8_t *data, uint8_t size) { return CANx_MessageRead (obj_idx, msg_info, data, size, 2U); }
#endif

/**
  \fn          int32_t CANx_Control (uint32_t control, uint32_t arg, uint8_t x)
  \brief       Control CAN interface.
  \param[in]   control  Operation
                 - ARM_CAN_SET_FD_MODE :            set FD operation mode
                 - ARM_CAN_ABORT_MESSAGE_SEND :     abort sending of CAN message
                 - ARM_CAN_CONTROL_RETRANSMISSION : enable/disable automatic retransmission
                 - ARM_CAN_SET_TRANSCEIVER_DELAY :  set transceiver delay
  \param[in]   arg      Argument of operation
  \param[in]   x        Controller number (0..2)
  \return      execution status
*/
static int32_t CANx_Control (uint32_t control, uint32_t arg, uint8_t x) {
  CAN_TypeDef *ptr_CAN;

  if (x >= CAN_CTRL_NUM)           { return ARM_DRIVER_ERROR; }
  if (can_driver_powered[x] == 0U) { return ARM_DRIVER_ERROR; }

  ptr_CAN = ptr_CANx[x];

  switch (control & ARM_CAN_CONTROL_Msk) {
    case ARM_CAN_ABORT_MESSAGE_SEND:
      if ((arg < CAN_RX_OBJ_NUM) || (arg >= CAN_TOT_OBJ_NUM)) { return ARM_DRIVER_ERROR_PARAMETER; }
      arg -= CAN_RX_OBJ_NUM;
      switch (arg) {
        case 0:
          ptr_CAN->TSR = CAN_TSR_ABRQ0;
          break;
        case 1:
          ptr_CAN->TSR = CAN_TSR_ABRQ1;
          break;
        case 2:
          ptr_CAN->TSR = CAN_TSR_ABRQ2;
          break;
        default:
          return ARM_DRIVER_ERROR_PARAMETER;
      }
      break;
    case ARM_CAN_CONTROL_RETRANSMISSION:
      switch (arg) {
        case 0:
          ptr_CAN->MCR |=  CAN_MCR_NART;
          break;
        case 1:
          ptr_CAN->MCR &= ~CAN_MCR_NART;
          break;
        default:
          return ARM_DRIVER_ERROR_PARAMETER;
      }
      break;
    case ARM_CAN_SET_FD_MODE:
    case ARM_CAN_SET_TRANSCEIVER_DELAY:
    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}
#if (MX_CAN1 == 1U)
static int32_t CAN1_Control (uint32_t control, uint32_t arg) { return CANx_Control (control, arg, 0U); }
#endif
#if (MX_CAN2 == 1U)
static int32_t CAN2_Control (uint32_t control, uint32_t arg) { return CANx_Control (control, arg, 1U); }
#endif
#if (MX_CAN3 == 1U)
static int32_t CAN3_Control (uint32_t control, uint32_t arg) { return CANx_Control (control, arg, 2U); }
#endif

/**
  \fn          ARM_CAN_STATUS CANx_GetStatus (uint8_t x)
  \brief       Get CAN status.
  \param[in]   x      Controller number (0..2)
  \return      CAN status ARM_CAN_STATUS
*/
static ARM_CAN_STATUS CANx_GetStatus (uint8_t x) {
  CAN_TypeDef    *ptr_CAN;
  ARM_CAN_STATUS  can_status;
  uint32_t        esr;

  ptr_CAN      = ptr_CANx[x];
  esr          = ptr_CAN->ESR;
  ptr_CAN->ESR = CAN_ESR_LEC;           // Software set last error code to unused value

  if       ((ptr_CAN->MSR & CAN_MSR_INAK) != 0U)  { can_status.unit_state = ARM_CAN_UNIT_STATE_INACTIVE; }
  else if (((ptr_CAN->BTR & CAN_BTR_LBKM) != 0U) ||
           ((ptr_CAN->BTR & CAN_BTR_SILM) != 0U)) { can_status.unit_state = ARM_CAN_UNIT_STATE_PASSIVE;  }
  else if ((esr & CAN_ESR_BOFF) != 0U)            { can_status.unit_state = ARM_CAN_UNIT_STATE_INACTIVE; }
  else if ((esr & CAN_ESR_EPVF) != 0U)            { can_status.unit_state = ARM_CAN_UNIT_STATE_PASSIVE;  }
  else                                            { can_status.unit_state = ARM_CAN_UNIT_STATE_ACTIVE;   }

  switch ((esr & CAN_ESR_LEC) >> 4) {
    case 0:
      can_status.last_error_code = ARM_CAN_LEC_NO_ERROR;
      break;
    case 1:
      can_status.last_error_code = ARM_CAN_LEC_STUFF_ERROR;
      break;
    case 2:
      can_status.last_error_code = ARM_CAN_LEC_FORM_ERROR;
      break;
    case 3:
      can_status.last_error_code = ARM_CAN_LEC_ACK_ERROR;
      break;
    case 4:
    case 5:
      can_status.last_error_code = ARM_CAN_LEC_BIT_ERROR;
      break;
    case 6:
      can_status.last_error_code = ARM_CAN_LEC_CRC_ERROR;
      break;
    case 7:
      can_status.last_error_code = ARM_CAN_LEC_NO_ERROR;
      break;
  }

  can_status.tx_error_count = (uint8_t)((esr & CAN_ESR_TEC) >> 16);
  can_status.rx_error_count = (uint8_t)((esr & CAN_ESR_REC) >> 24);

  return can_status;
}
#if (MX_CAN1 == 1U)
static ARM_CAN_STATUS CAN1_GetStatus (void) { return CANx_GetStatus (0); }
#endif
#if (MX_CAN2 == 1U)
static ARM_CAN_STATUS CAN2_GetStatus (void) { return CANx_GetStatus (1); }
#endif
#if (MX_CAN3 == 1U)
static ARM_CAN_STATUS CAN3_GetStatus (void) { return CANx_GetStatus (2); }
#endif


#if (MX_CAN1 == 1U)
/**
  \fn          void CAN1_TX_IRQHandler (void)
  \brief       CAN1 Transmit Interrupt Routine (IRQ).
*/
void CAN1_TX_IRQHandler (void) {
  uint32_t esr, ier;

  if ((CAN1->TSR & CAN_TSR_RQCP0) != 0U) {
    if ((CAN1->TSR & CAN_TSR_TXOK0) != 0U) {
      if (can_obj_cfg[0][CAN_RX_OBJ_NUM] == ARM_CAN_OBJ_TX) {
        if (CAN_SignalObjectEvent[0] != NULL) { CAN_SignalObjectEvent[0](CAN_RX_OBJ_NUM, ARM_CAN_EVENT_SEND_COMPLETE); }
      }
    }
    CAN1->TSR = CAN_TSR_RQCP0;          // Request completed on transmit mailbox 0
  }
  if ((CAN1->TSR & CAN_TSR_RQCP1) != 0U) {
    if ((CAN1->TSR & CAN_TSR_TXOK1) != 0U) {
      if (can_obj_cfg[0][CAN_RX_OBJ_NUM+1U] == ARM_CAN_OBJ_TX) {
        if (CAN_SignalObjectEvent[0] != NULL) { CAN_SignalObjectEvent[0](CAN_RX_OBJ_NUM+1U, ARM_CAN_EVENT_SEND_COMPLETE); }
      }
    }
    CAN1->TSR = CAN_TSR_RQCP1;          // Request completed on transmit mailbox 1
  }
  if ((CAN1->TSR & CAN_TSR_RQCP2) != 0U) {
    if ((CAN1->TSR & CAN_TSR_TXOK2) != 0U) {
      if (can_obj_cfg[0][CAN_RX_OBJ_NUM+2U] == ARM_CAN_OBJ_TX) {
        if (CAN_SignalObjectEvent[0] != NULL) { CAN_SignalObjectEvent[0](CAN_RX_OBJ_NUM+2U, ARM_CAN_EVENT_SEND_COMPLETE); }
      }
    }
    CAN1->TSR = CAN_TSR_RQCP2;          // Request completed on transmit mailbox 2
  }

  // Handle transition from from 'bus off', ' error active' state, or re-enable warning interrupt
  esr = CAN1->ESR;
  ier = CAN1->IER;
  if (((esr & CAN_ESR_BOFF) == 0U) && ((ier & CAN_IER_BOFIE) == 0U)) { 
    CAN1->IER |= CAN_IER_BOFIE;
    if (CAN_SignalUnitEvent[0] != NULL) { CAN_SignalUnitEvent[0](ARM_CAN_EVENT_UNIT_PASSIVE); }
  }
  if (((esr & CAN_ESR_EPVF) == 0U) && ((ier & CAN_IER_EPVIE) == 0U)) {
    CAN1->IER |= CAN_IER_EPVIE;
    if (CAN_SignalUnitEvent[0] != NULL) { CAN_SignalUnitEvent[0](ARM_CAN_EVENT_UNIT_ACTIVE); }
  }
  if (((esr & CAN_ESR_EWGF) == 0U) && ((ier & CAN_IER_EWGIE) == 0U) && (((esr & CAN_ESR_TEC) >> 16) == 95U) && (((esr & CAN_ESR_REC) >> 24) < 95U)) {
    CAN1->IER |= CAN_IER_EWGIE;
  }
}

/**
  \fn          void CAN1_RX0_IRQHandler (void)
  \brief       CAN1 Receive on FIFO0 Interrupt Routine (IRQ).
*/
void CAN1_RX0_IRQHandler (void) {
  uint32_t esr, ier;

  if (can_obj_cfg[0][0] == ARM_CAN_OBJ_RX) {
    if ((CAN1->RF0R & CAN_RF0R_FOVR0) != 0U) {
      CAN1->RF0R = CAN_RF0R_FOVR0;      // Clear overrun flag
      if (CAN_SignalObjectEvent[0] != NULL) { CAN_SignalObjectEvent[0](0U, ARM_CAN_EVENT_RECEIVE | ARM_CAN_EVENT_RECEIVE_OVERRUN); }
    } else if ((CAN1->RF0R & CAN_RF0R_FMP0) != 0U) {
      if (CAN_SignalObjectEvent[0] != NULL) { CAN_SignalObjectEvent[0](0U, ARM_CAN_EVENT_RECEIVE); }
    }
  } else {
    CAN1->RF0R = CAN_RF0R_RFOM0;        // Release FIFO 0 output mailbox if object not enabled for reception
  }

  // Handle transition from from 'bus off', ' error active' state, or re-enable warning interrupt
  esr = CAN1->ESR;
  ier = CAN1->IER;
  if (((esr & CAN_ESR_BOFF) == 0U) && ((ier & CAN_IER_BOFIE) == 0U)) { 
    CAN1->IER |= CAN_IER_BOFIE;
    if (CAN_SignalUnitEvent[0] != NULL) { CAN_SignalUnitEvent[0](ARM_CAN_EVENT_UNIT_PASSIVE); }
  }
  if (((esr & CAN_ESR_EPVF) == 0U) && ((ier & CAN_IER_EPVIE) == 0U)) {
    CAN1->IER |= CAN_IER_EPVIE;
    if (CAN_SignalUnitEvent[0] != NULL) { CAN_SignalUnitEvent[0](ARM_CAN_EVENT_UNIT_ACTIVE); }
  }
  if (((esr & CAN_ESR_EWGF) == 0U) && ((ier & CAN_IER_EWGIE) == 0U) && (((esr & CAN_ESR_TEC) >> 16) < 95U) && (((esr & CAN_ESR_REC) >> 24) == 95U)) {
    CAN1->IER |= CAN_IER_EWGIE;
  }
}

/**
  \fn          void CAN1_RX1_IRQHandler (void)
  \brief       CAN1 Receive on FIFO1 Interrupt Routine (IRQ).
*/
void CAN1_RX1_IRQHandler (void) {
  uint32_t esr, ier;

  if (can_obj_cfg[0][1] == ARM_CAN_OBJ_RX) {
    if ((CAN1->RF1R & CAN_RF1R_FOVR1) != 0U) {
      CAN1->RF1R = CAN_RF1R_FOVR1;      // Clear overrun flag
      if (CAN_SignalObjectEvent[0] != NULL) { CAN_SignalObjectEvent[0](1U, ARM_CAN_EVENT_RECEIVE | ARM_CAN_EVENT_RECEIVE_OVERRUN); }
    } else if ((CAN1->RF1R & CAN_RF1R_FMP1) != 0U) {
      if (CAN_SignalObjectEvent[0] != NULL) { CAN_SignalObjectEvent[0](1U, ARM_CAN_EVENT_RECEIVE); }
    }
  } else {
    CAN1->RF1R = CAN_RF1R_RFOM1;        // Release FIFO 1 output mailbox if object not enabled for reception
  }

  // Handle transition from from 'bus off', ' error active' state, or re-enable warning interrupt
  esr = CAN1->ESR;
  ier = CAN1->IER;
  if (((esr & CAN_ESR_BOFF) == 0U) && ((ier & CAN_IER_BOFIE) == 0U)) { 
    CAN1->IER |= CAN_IER_BOFIE;
    if (CAN_SignalUnitEvent[0] != NULL) { CAN_SignalUnitEvent[0](ARM_CAN_EVENT_UNIT_PASSIVE); }
  }
  if (((esr & CAN_ESR_EPVF) == 0U) && ((ier & CAN_IER_EPVIE) == 0U)) {
    CAN1->IER |= CAN_IER_EPVIE;
    if (CAN_SignalUnitEvent[0] != NULL) { CAN_SignalUnitEvent[0](ARM_CAN_EVENT_UNIT_ACTIVE); }
  }
  if (((esr & CAN_ESR_EWGF) == 0U) && ((ier & CAN_IER_EWGIE) == 0U) && (((esr & CAN_ESR_TEC) >> 16) < 95U) && (((esr & CAN_ESR_REC) >> 24) == 95U)) {
    CAN1->IER |= CAN_IER_EWGIE;
  }
}

/**
  \fn          void CAN1_SCE_IRQHandler (void)
  \brief       CAN1 Error and Status Change Interrupt Routine (IRQ).
*/
void CAN1_SCE_IRQHandler (void) {
  uint32_t esr, ier;

  if (CAN_SignalUnitEvent[0] != NULL) {
    esr = CAN1->ESR;
    ier = CAN1->IER;
    CAN1->MSR = CAN_MSR_ERRI;             // Clear error interrupt
    if      (((esr & CAN_ESR_BOFF) != 0U) && ((ier & CAN_IER_BOFIE) != 0U)) { CAN1->IER &= ~CAN_IER_BOFIE; CAN_SignalUnitEvent[0](ARM_CAN_EVENT_UNIT_BUS_OFF); }
    else if (((esr & CAN_ESR_EPVF) != 0U) && ((ier & CAN_IER_EPVIE) != 0U)) { CAN1->IER &= ~CAN_IER_EPVIE; CAN_SignalUnitEvent[0](ARM_CAN_EVENT_UNIT_PASSIVE); }
    else if (((esr & CAN_ESR_EWGF) != 0U) && ((ier & CAN_IER_EWGIE) != 0U)) { CAN1->IER &= ~CAN_IER_EWGIE; CAN_SignalUnitEvent[0](ARM_CAN_EVENT_UNIT_WARNING); }
  }
}
#endif

#if (MX_CAN2 == 1U)
/**
  \fn          void CAN2_TX_IRQHandler (void)
  \brief       CAN2 Transmit Interrupt Routine (IRQ).
*/
void CAN2_TX_IRQHandler (void) {
  uint32_t esr, ier;

  if ((CAN2->TSR & CAN_TSR_RQCP0) != 0U) {
    if ((CAN2->TSR & CAN_TSR_TXOK0) != 0U) {
      if (can_obj_cfg[1][CAN_RX_OBJ_NUM] == ARM_CAN_OBJ_TX) {
        if (CAN_SignalObjectEvent[1] != NULL) { CAN_SignalObjectEvent[1](CAN_RX_OBJ_NUM, ARM_CAN_EVENT_SEND_COMPLETE); }
      }
    }
    CAN2->TSR = CAN_TSR_RQCP0;          // Request completed on transmit mailbox 0
  }
  if ((CAN2->TSR & CAN_TSR_RQCP1) != 0U) {
    if ((CAN2->TSR & CAN_TSR_TXOK1) != 0U) {
      if (can_obj_cfg[1][CAN_RX_OBJ_NUM+1U] == ARM_CAN_OBJ_TX) {
        if (CAN_SignalObjectEvent[1] != NULL) { CAN_SignalObjectEvent[1](CAN_RX_OBJ_NUM+1U, ARM_CAN_EVENT_SEND_COMPLETE); }
      }
    }
    CAN2->TSR = CAN_TSR_RQCP1;          // Request completed on transmit mailbox 1
  }
  if ((CAN2->TSR & CAN_TSR_RQCP2) != 0U) {
    if ((CAN2->TSR & CAN_TSR_TXOK2) != 0U) {
      if (can_obj_cfg[1][CAN_RX_OBJ_NUM+2U] == ARM_CAN_OBJ_TX) {
        if (CAN_SignalObjectEvent[1] != NULL) { CAN_SignalObjectEvent[1](CAN_RX_OBJ_NUM+2U, ARM_CAN_EVENT_SEND_COMPLETE); }
      }
    }
    CAN2->TSR = CAN_TSR_RQCP2;          // Request completed on transmit mailbox 2
  }

  // Handle transition from from 'bus off', ' error active' state, or re-enable warning interrupt
  esr = CAN2->ESR;
  ier = CAN2->IER;
  if (((esr & CAN_ESR_BOFF) == 0U) && ((ier & CAN_IER_BOFIE) == 0U)) { 
    CAN2->IER |= CAN_IER_BOFIE;
    if (CAN_SignalUnitEvent[1] != NULL) { CAN_SignalUnitEvent[1](ARM_CAN_EVENT_UNIT_PASSIVE); }
  }
  if (((esr & CAN_ESR_EPVF) == 0U) && ((ier & CAN_IER_EPVIE) == 0U)) {
    CAN2->IER |= CAN_IER_EPVIE;
    if (CAN_SignalUnitEvent[1] != NULL) { CAN_SignalUnitEvent[1](ARM_CAN_EVENT_UNIT_ACTIVE); }
  }
  if (((esr & CAN_ESR_EWGF) == 0U) && ((ier & CAN_IER_EWGIE) == 0U) && (((esr & CAN_ESR_TEC) >> 16) == 95U) && (((esr & CAN_ESR_REC) >> 24) < 95U)) {
    CAN2->IER |= CAN_IER_EWGIE;
  }
}

/**
  \fn          void CAN2_RX0_IRQHandler (void)
  \brief       CAN2 Receive on FIFO0 Interrupt Routine (IRQ).
*/
void CAN2_RX0_IRQHandler (void) {
  uint32_t esr, ier;

  if (can_obj_cfg[1][0] == ARM_CAN_OBJ_RX) {
    if ((CAN2->RF0R & CAN_RF0R_FOVR0) != 0U) {
      CAN2->RF0R = CAN_RF0R_FOVR0;      // Clear overrun flag
      if (CAN_SignalObjectEvent[1] != NULL) { CAN_SignalObjectEvent[1](0U, ARM_CAN_EVENT_RECEIVE | ARM_CAN_EVENT_RECEIVE_OVERRUN); }
    } else if ((CAN2->RF0R & CAN_RF0R_FMP0) != 0U) {
      if (CAN_SignalObjectEvent[1] != NULL) { CAN_SignalObjectEvent[1](0U, ARM_CAN_EVENT_RECEIVE); }
    }
  } else {
    CAN2->RF0R = CAN_RF0R_RFOM0;        // Release FIFO 0 output mailbox if object not enabled for reception
  }

  // Handle transition from from 'bus off', ' error active' state, or re-enable warning interrupt
  esr = CAN2->ESR;
  ier = CAN2->IER;
  if (((esr & CAN_ESR_BOFF) == 0U) && ((ier & CAN_IER_BOFIE) == 0U)) { 
    CAN2->IER |= CAN_IER_BOFIE;
    if (CAN_SignalUnitEvent[1] != NULL) { CAN_SignalUnitEvent[1](ARM_CAN_EVENT_UNIT_PASSIVE); }
  }
  if (((esr & CAN_ESR_EPVF) == 0U) && ((ier & CAN_IER_EPVIE) == 0U)) {
    CAN2->IER |= CAN_IER_EPVIE;
    if (CAN_SignalUnitEvent[1] != NULL) { CAN_SignalUnitEvent[1](ARM_CAN_EVENT_UNIT_ACTIVE); }
  }
  if (((esr & CAN_ESR_EWGF) == 0U) && ((ier & CAN_IER_EWGIE) == 0U) && (((esr & CAN_ESR_TEC) >> 16) < 95U) && (((esr & CAN_ESR_REC) >> 24) == 95U)) {
    CAN2->IER |= CAN_IER_EWGIE;
  }
}

/**
  \fn          void CAN2_RX1_IRQHandler (void)
  \brief       CAN2 Receive on FIFO1 Interrupt Routine (IRQ).
*/
void CAN2_RX1_IRQHandler (void) {
  uint32_t esr, ier;

  if (can_obj_cfg[1][1] == ARM_CAN_OBJ_RX) {
    if ((CAN2->RF1R & CAN_RF1R_FOVR1) != 0U) {
      CAN2->RF1R = CAN_RF1R_FOVR1;      // Clear overrun flag
      if (CAN_SignalObjectEvent[1] != NULL) { CAN_SignalObjectEvent[1](1U, ARM_CAN_EVENT_RECEIVE | ARM_CAN_EVENT_RECEIVE_OVERRUN); }
    } else if ((CAN2->RF1R & CAN_RF1R_FMP1) != 0U) {
      if (CAN_SignalObjectEvent[1] != NULL) { CAN_SignalObjectEvent[1](1U, ARM_CAN_EVENT_RECEIVE); }
    }
  } else {
    CAN2->RF1R = CAN_RF1R_RFOM1;        // Release FIFO 1 output mailbox if object not enabled for reception
  }

  // Handle transition from from 'bus off', ' error active' state, or re-enable warning interrupt
  esr = CAN2->ESR;
  ier = CAN2->IER;
  if (((esr & CAN_ESR_BOFF) == 0U) && ((ier & CAN_IER_BOFIE) == 0U)) { 
    CAN2->IER |= CAN_IER_BOFIE;
    if (CAN_SignalUnitEvent[1] != NULL) { CAN_SignalUnitEvent[1](ARM_CAN_EVENT_UNIT_PASSIVE); }
  }
  if (((esr & CAN_ESR_EPVF) == 0U) && ((ier & CAN_IER_EPVIE) == 0U)) {
    CAN2->IER |= CAN_IER_EPVIE;
    if (CAN_SignalUnitEvent[1] != NULL) { CAN_SignalUnitEvent[1](ARM_CAN_EVENT_UNIT_ACTIVE); }
  }
  if (((esr & CAN_ESR_EWGF) == 0U) && ((ier & CAN_IER_EWGIE) == 0U) && (((esr & CAN_ESR_TEC) >> 16) < 95U) && (((esr & CAN_ESR_REC) >> 24) == 95U)) {
    CAN2->IER |= CAN_IER_EWGIE;
  }
}

/**
  \fn          void CAN2_SCE_IRQHandler (void)
  \brief       CAN2 Error and Status Change Interrupt Routine (IRQ).
*/
void CAN2_SCE_IRQHandler (void) {
  uint32_t esr, ier;

  if (CAN_SignalUnitEvent[1] != NULL) {
    esr = CAN2->ESR;
    ier = CAN2->IER;
    CAN2->MSR = CAN_MSR_ERRI;             // Clear error interrupt
    if      (((esr & CAN_ESR_BOFF) != 0U) && ((ier & CAN_IER_BOFIE) != 0U)) { CAN2->IER &= ~CAN_IER_BOFIE; CAN_SignalUnitEvent[1](ARM_CAN_EVENT_UNIT_BUS_OFF); }
    else if (((esr & CAN_ESR_EPVF) != 0U) && ((ier & CAN_IER_EPVIE) != 0U)) { CAN2->IER &= ~CAN_IER_EPVIE; CAN_SignalUnitEvent[1](ARM_CAN_EVENT_UNIT_PASSIVE); }
    else if (((esr & CAN_ESR_EWGF) != 0U) && ((ier & CAN_IER_EWGIE) != 0U)) { CAN2->IER &= ~CAN_IER_EWGIE; CAN_SignalUnitEvent[1](ARM_CAN_EVENT_UNIT_WARNING); }
  }
}
#endif

#if (MX_CAN3 == 1U)
/**
  \fn          void CAN3_TX_IRQHandler (void)
  \brief       CAN3 Transmit Interrupt Routine (IRQ).
*/
void CAN3_TX_IRQHandler (void) {
  uint32_t esr, ier;

  if ((CAN3->TSR & CAN_TSR_RQCP0) != 0U) {
    if ((CAN3->TSR & CAN_TSR_TXOK0) != 0U) {
      if (can_obj_cfg[2][CAN_RX_OBJ_NUM] == ARM_CAN_OBJ_TX) {
        if (CAN_SignalObjectEvent[2] != NULL) { CAN_SignalObjectEvent[2](CAN_RX_OBJ_NUM, ARM_CAN_EVENT_SEND_COMPLETE); }
      }
    }
    CAN3->TSR = CAN_TSR_RQCP0;          // Request completed on transmit mailbox 0
  }
  if ((CAN3->TSR & CAN_TSR_RQCP1) != 0U) {
    if ((CAN3->TSR & CAN_TSR_TXOK1) != 0U) {
      if (can_obj_cfg[2][CAN_RX_OBJ_NUM+1U] == ARM_CAN_OBJ_TX) {
        if (CAN_SignalObjectEvent[2] != NULL) { CAN_SignalObjectEvent[2](CAN_RX_OBJ_NUM+1U, ARM_CAN_EVENT_SEND_COMPLETE); }
      }
    }
    CAN3->TSR = CAN_TSR_RQCP1;          // Request completed on transmit mailbox 1
  }
  if ((CAN3->TSR & CAN_TSR_RQCP2) != 0U) {
    if ((CAN3->TSR & CAN_TSR_TXOK2) != 0U) {
      if (can_obj_cfg[2][CAN_RX_OBJ_NUM+2U] == ARM_CAN_OBJ_TX) {
        if (CAN_SignalObjectEvent[2] != NULL) { CAN_SignalObjectEvent[2](CAN_RX_OBJ_NUM+2U, ARM_CAN_EVENT_SEND_COMPLETE); }
      }
    }
    CAN3->TSR = CAN_TSR_RQCP2;          // Request completed on transmit mailbox 2
  }

  // Handle transition from from 'bus off', ' error active' state, or re-enable warning interrupt
  esr = CAN3->ESR;
  ier = CAN3->IER;
  if (((esr & CAN_ESR_BOFF) == 0U) && ((ier & CAN_IER_BOFIE) == 0U)) { 
    CAN3->IER |= CAN_IER_BOFIE;
    if (CAN_SignalUnitEvent[2] != NULL) { CAN_SignalUnitEvent[2](ARM_CAN_EVENT_UNIT_PASSIVE); }
  }
  if (((esr & CAN_ESR_EPVF) == 0U) && ((ier & CAN_IER_EPVIE) == 0U)) {
    CAN3->IER |= CAN_IER_EPVIE;
    if (CAN_SignalUnitEvent[2] != NULL) { CAN_SignalUnitEvent[2](ARM_CAN_EVENT_UNIT_ACTIVE); }
  }
  if (((esr & CAN_ESR_EWGF) == 0U) && ((ier & CAN_IER_EWGIE) == 0U) && (((esr & CAN_ESR_TEC) >> 16) == 95U) && (((esr & CAN_ESR_REC) >> 24) < 95U)) {
    CAN3->IER |= CAN_IER_EWGIE;
  }
}

/**
  \fn          void CAN3_RX0_IRQHandler (void)
  \brief       CAN3 Receive on FIFO0 Interrupt Routine (IRQ).
*/
void CAN3_RX0_IRQHandler (void) {
  uint32_t esr, ier;

  if (can_obj_cfg[2][0] == ARM_CAN_OBJ_RX) {
    if ((CAN3->RF0R & CAN_RF0R_FOVR0) != 0U) {
      CAN3->RF0R = CAN_RF0R_FOVR0;      // Clear overrun flag
      if (CAN_SignalObjectEvent[2] != NULL) { CAN_SignalObjectEvent[2](0U, ARM_CAN_EVENT_RECEIVE | ARM_CAN_EVENT_RECEIVE_OVERRUN); }
    } else if ((CAN3->RF0R & CAN_RF0R_FMP0) != 0U) {
      if (CAN_SignalObjectEvent[2] != NULL) { CAN_SignalObjectEvent[2](0U, ARM_CAN_EVENT_RECEIVE); }
    }
  } else {
    CAN3->RF0R = CAN_RF0R_RFOM0;        // Release FIFO 0 output mailbox if object not enabled for reception
  }

  // Handle transition from from 'bus off', ' error active' state, or re-enable warning interrupt
  esr = CAN3->ESR;
  ier = CAN3->IER;
  if (((esr & CAN_ESR_BOFF) == 0U) && ((ier & CAN_IER_BOFIE) == 0U)) { 
    CAN3->IER |= CAN_IER_BOFIE;
    if (CAN_SignalUnitEvent[2] != NULL) { CAN_SignalUnitEvent[2](ARM_CAN_EVENT_UNIT_PASSIVE); }
  }
  if (((esr & CAN_ESR_EPVF) == 0U) && ((ier & CAN_IER_EPVIE) == 0U)) {
    CAN3->IER |= CAN_IER_EPVIE;
    if (CAN_SignalUnitEvent[2] != NULL) { CAN_SignalUnitEvent[2](ARM_CAN_EVENT_UNIT_ACTIVE); }
  }
  if (((esr & CAN_ESR_EWGF) == 0U) && ((ier & CAN_IER_EWGIE) == 0U) && (((esr & CAN_ESR_TEC) >> 16) < 95U) && (((esr & CAN_ESR_REC) >> 24) == 95U)) {
    CAN3->IER |= CAN_IER_EWGIE;
  }
}

/**
  \fn          void CAN3_RX1_IRQHandler (void)
  \brief       CAN3 Receive on FIFO1 Interrupt Routine (IRQ).
*/
void CAN3_RX1_IRQHandler (void) {
  uint32_t esr, ier;

  if (can_obj_cfg[2][1] == ARM_CAN_OBJ_RX) {
    if ((CAN3->RF1R & CAN_RF1R_FOVR1) != 0U) {
      CAN3->RF1R = CAN_RF1R_FOVR1;      // Clear overrun flag
      if (CAN_SignalObjectEvent[2] != NULL) { CAN_SignalObjectEvent[2](1U, ARM_CAN_EVENT_RECEIVE | ARM_CAN_EVENT_RECEIVE_OVERRUN); }
    } else if ((CAN3->RF1R & CAN_RF1R_FMP1) != 0U) {
      if (CAN_SignalObjectEvent[2] != NULL) { CAN_SignalObjectEvent[2](1U, ARM_CAN_EVENT_RECEIVE); }
    }
  } else {
    CAN3->RF1R = CAN_RF1R_RFOM1;        // Release FIFO 1 output mailbox if object not enabled for reception
  }

  // Handle transition from from 'bus off', ' error active' state, or re-enable warning interrupt
  esr = CAN3->ESR;
  ier = CAN3->IER;
  if (((esr & CAN_ESR_BOFF) == 0U) && ((ier & CAN_IER_BOFIE) == 0U)) { 
    CAN3->IER |= CAN_IER_BOFIE;
    if (CAN_SignalUnitEvent[2] != NULL) { CAN_SignalUnitEvent[2](ARM_CAN_EVENT_UNIT_PASSIVE); }
  }
  if (((esr & CAN_ESR_EPVF) == 0U) && ((ier & CAN_IER_EPVIE) == 0U)) {
    CAN3->IER |= CAN_IER_EPVIE;
    if (CAN_SignalUnitEvent[2] != NULL) { CAN_SignalUnitEvent[2](ARM_CAN_EVENT_UNIT_ACTIVE); }
  }
  if (((esr & CAN_ESR_EWGF) == 0U) && ((ier & CAN_IER_EWGIE) == 0U) && (((esr & CAN_ESR_TEC) >> 16) < 95U) && (((esr & CAN_ESR_REC) >> 24) == 95U)) {
    CAN3->IER |= CAN_IER_EWGIE;
  }
}

/**
  \fn          void CAN3_SCE_IRQHandler (void)
  \brief       CAN3 Error and Status Change Interrupt Routine (IRQ).
*/
void CAN3_SCE_IRQHandler (void) {
  uint32_t esr, ier;

  if (CAN_SignalUnitEvent[2] != NULL) {
    esr = CAN3->ESR;
    ier = CAN3->IER;
    CAN3->MSR = CAN_MSR_ERRI;             // Clear error interrupt
    if      (((esr & CAN_ESR_BOFF) != 0U) && ((ier & CAN_IER_BOFIE) != 0U)) { CAN3->IER &= ~CAN_IER_BOFIE; CAN_SignalUnitEvent[2](ARM_CAN_EVENT_UNIT_BUS_OFF); }
    else if (((esr & CAN_ESR_EPVF) != 0U) && ((ier & CAN_IER_EPVIE) != 0U)) { CAN3->IER &= ~CAN_IER_EPVIE; CAN_SignalUnitEvent[2](ARM_CAN_EVENT_UNIT_PASSIVE); }
    else if (((esr & CAN_ESR_EWGF) != 0U) && ((ier & CAN_IER_EWGIE) != 0U)) { CAN3->IER &= ~CAN_IER_EWGIE; CAN_SignalUnitEvent[2](ARM_CAN_EVENT_UNIT_WARNING); }
  }
}
#endif


#if (MX_CAN1 == 1U)
ARM_DRIVER_CAN Driver_CAN1 = {
  CAN_GetVersion,
  CAN_GetCapabilities,
  CAN1_Initialize,
  CAN1_Uninitialize,
  CAN1_PowerControl,
  CAN_GetClock,
  CAN1_SetBitrate,
  CAN1_SetMode,
  CAN1_ObjectGetCapabilities,
  CAN1_ObjectSetFilter,
  CAN1_ObjectConfigure,
  CAN1_MessageSend,
  CAN1_MessageRead,
  CAN1_Control,
  CAN1_GetStatus
};
#endif

#if (MX_CAN2 == 1U)
ARM_DRIVER_CAN Driver_CAN2 = {
  CAN_GetVersion,
  CAN_GetCapabilities,
  CAN2_Initialize,
  CAN2_Uninitialize,
  CAN2_PowerControl,
  CAN_GetClock,
  CAN2_SetBitrate,
  CAN2_SetMode,
  CAN2_ObjectGetCapabilities,
  CAN2_ObjectSetFilter,
  CAN2_ObjectConfigure,
  CAN2_MessageSend,
  CAN2_MessageRead,
  CAN2_Control,
  CAN2_GetStatus
};
#endif

#if (MX_CAN3 == 1U)
ARM_DRIVER_CAN Driver_CAN3 = {
  CAN_GetVersion,
  CAN_GetCapabilities,
  CAN3_Initialize,
  CAN3_Uninitialize,
  CAN3_PowerControl,
  CAN_GetClock,
  CAN3_SetBitrate,
  CAN3_SetMode,
  CAN3_ObjectGetCapabilities,
  CAN3_ObjectSetFilter,
  CAN3_ObjectConfigure,
  CAN3_MessageSend,
  CAN3_MessageRead,
  CAN3_Control,
  CAN3_GetStatus
};
#endif

/*! \endcond */
