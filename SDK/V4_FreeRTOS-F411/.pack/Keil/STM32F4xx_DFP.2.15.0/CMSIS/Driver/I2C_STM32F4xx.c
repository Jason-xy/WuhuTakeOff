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
 * $Date:        2. March 2020
 * $Revision:    V2.10
 *
 * Driver:       Driver_I2C1, Driver_I2C2, Driver_I2C3
 * Configured:   via RTE_Device.h configuration file
 * Project:      I2C Driver for ST STM32F4xx
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                 Value   I2C Interface
 *   ---------------------                 -----   -------------
 *   Connect to hardware via Driver_I2C# = 1       use I2C1
 *   Connect to hardware via Driver_I2C# = 2       use I2C2
 *   Connect to hardware via Driver_I2C# = 3       use I2C3
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 2.10
 *    Corrected 2 byte reception in master mode
 *  Version 2.9
 *    Corrected 10-bit addressing mode issues
 *    Corrected ARM_I2C_EVENT_BUS_CLEAR event signaling
 *    Eliminated wait dead loops and replaced them with wait with timeout
 *  Version 2.8
 *    Added support for ARM Compiler 6
 *  Version 2.7
 *    Slave event handling reworked.
 *  Version 2.6
 *    Corrected rise time setting when using ARM_I2C_BUS_SPEED_FAST
 *  Version 2.5
 *    Corrected PowerControl function for:
 *      - Unconditional Power Off
 *      - Conditional Power full (driver must be initialized)
 *  Version 2.4
 *    Added support for STM32F410xx
 *    Corrected 3 byte reception and POS bit handling in master mode 
 *    Corrected acknowledge handling in slave mode
 *  Version 2.3
 *    Updated initialization, uninitialization and power procedures
 *    Added support for STM32F446xx
 *    Limitation of FREQ[5:0] bits in I2C->CR2 set to 50MHz
 *  Version 2.2
 *    Corrected transfer issues after ARM_I2C_EVENT_ADDRESS_NACK.
 *    Corrected slave address parameter checking.
 *  Version 2.1
 *    Corrected 10-bit addressing mode
 *    Slave operation mode issues fixed
 *    STM32CubeMX generated code can also be used to configure the driver.
 *  Version 2.0
 *    Updated to the CMSIS Driver API V2.02
 *  Version 1.2
 *    Bugfix (corrected I2C register access)
 *  Version 1.1
 *    Based on API V1.10 (namespace prefix ARM_ added)
 *  Version 1.0
 *    Initial release
 */

/*! \page stm32f4_i2c CMSIS-Driver I2C Setup 

The CMSIS-Driver I2C requires:
  - Setup of I2Cx input clock
  - Setup of I2Cx in I2C mode with optional DMA for Rx and Tx transfers
 
Valid settings for various evaluation boards are listed in the table below:

Peripheral Resource | MCBSTM32F400      | STM32F4-Discovery | 32F401C-Discovery | 32F429I-Discovery
:-------------------|:------------------|:------------------|:------------------|:------------------
I2C Mode            | I2C1: <b>I2C</b>  | I2C1: <b>I2C</b>  | I2C1: <b>I2C</b>  | I2C3: <b>I2C</b>
SCL pin             | PB8               | PB6               | PB6               | PA8
SDA pin             | PB9               | PB9               | PB9               | PC9

For different boards, refer to the hardware schematics to reflect correct setup values.

The STM32CubeMX configuration for MCBSTM32F400 with steps for Pinout, Clock, and System Configuration are 
listed below. Enter the values that are marked \b bold.
   
Pinout tab
----------
  1. Configure mode
    - Peripherals \b I2C1: Mode=<b>I2C</b>

Clock Configuration tab
-----------------------
  1. Configure APB1 clock
    - Setup "APB1 peripheral clocks (MHz)" to match application requirements

Configuration tab
-----------------
  1. Under Connectivity open \b I2C1 Configuration:
     - \e optional <b>DMA Settings</b>: setup DMA transfers for Rx and Tx\n
       \b Add - Select \b I2C1_RX: Stream=DMA1 Stream 0, Direction=Peripheral to Memory, Priority=Low
          DMA Request Settings         | Label             | Peripheral | Memory
          :----------------------------|:------------------|:-----------|:-------------
          Mode: Normal                 | Increment Address | OFF        |\b ON
          Use Fifo OFF Threshold: Full | Data Width        |\b Byte     | Byte
          .                            | Burst Size        | Single     | Single
       \b Add - Select \b I2C1_TX: Stream=DMA1 Stream 6, Direction=Memory to Peripheral, Priority=Low
          DMA Request Settings         | Label             | Peripheral | Memory
          :----------------------------|:------------------|:-----------|:-------------
          Mode: Normal                 | Increment Address | OFF        |\b ON
          Use Fifo OFF Threshold: Full | Data Width        |\b Byte     | Byte
          .                            | Burst Size        | Single     | Single

     - <b>GPIO Settings</b>: review settings, no changes required
          Pin Name | Signal on Pin | GPIO mode | GPIO Pull-up/Pull..| Maximum out | User Label
          :--------|:--------------|:----------|:-------------------|:------------|:----------
          PB8      | I2C1_SCL      | Alternate | Pull-up            | High        |.
          PB9      | I2C1_SDA      | Alternate | Pull-up            | High        |.

     - <b>NVIC Settings</b>: enable interrupts
          Interrupt Table                      | Enable | Preemption Priority | Sub Priority
          :------------------------------------|:-------|:--------------------|:--------------
          DMA1 stream0 global interrupt        |   ON   | 0                   | 0
          DMA1 stream6 global interrupt        |   ON   | 0                   | 0
          I2C1 event interrupt                 |\b ON   | 0                   | 0
          I2C1 error interrupt                 |\b ON   | 0                   | 0

     - Parameter Settings: not used
     - User Constants: not used
   
     Click \b OK to close the I2C1 Configuration dialog
*/

/*! \cond */

#include "I2C_STM32F4xx.h"

#define ARM_I2C_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(2,10)    /* driver version */


#ifndef ARM_I2C_BUSY_TIMEOUT
#define ARM_I2C_BUSY_TIMEOUT      1000  /* I2C Bus Busy Timeout in HAL ticks */
#endif


#if defined(MX_I2C1_RX_DMA_Instance) && defined(MX_I2C1_TX_DMA_Instance)
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
static DMA_HandleTypeDef hdma_i2c1_rx;
#else
extern DMA_HandleTypeDef hdma_i2c1_rx;
#endif

#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
static DMA_HandleTypeDef hdma_i2c1_tx;
#else
extern DMA_HandleTypeDef hdma_i2c1_tx;
#endif

static void I2C1_RX_DMA_Complete(DMA_HandleTypeDef *hdma);
static void I2C1_RX_DMA_Error   (DMA_HandleTypeDef *hdma);
static void I2C1_TX_DMA_Complete(DMA_HandleTypeDef *hdma);
static void I2C1_TX_DMA_Error   (DMA_HandleTypeDef *hdma);
#endif

#if defined(MX_I2C2_RX_DMA_Instance) && defined(MX_I2C2_TX_DMA_Instance)
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
static DMA_HandleTypeDef hdma_i2c2_rx;
#else
extern DMA_HandleTypeDef hdma_i2c2_rx;
#endif

#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
static DMA_HandleTypeDef hdma_i2c2_tx;
#else
extern DMA_HandleTypeDef hdma_i2c2_tx;
#endif

static void I2C2_RX_DMA_Complete(DMA_HandleTypeDef *hdma);
static void I2C2_RX_DMA_Error   (DMA_HandleTypeDef *hdma);
static void I2C2_TX_DMA_Complete(DMA_HandleTypeDef *hdma);
static void I2C2_TX_DMA_Error   (DMA_HandleTypeDef *hdma);
#endif

#if defined(MX_I2C3_RX_DMA_Instance) && defined(MX_I2C3_TX_DMA_Instance)
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
static DMA_HandleTypeDef hdma_i2c3_rx;
#else
extern DMA_HandleTypeDef hdma_i2c3_rx;
#endif

#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
static DMA_HandleTypeDef hdma_i2c3_tx;
#else
extern DMA_HandleTypeDef hdma_i2c3_tx;
#endif

static void I2C3_RX_DMA_Complete(DMA_HandleTypeDef *hdma);
static void I2C3_RX_DMA_Error   (DMA_HandleTypeDef *hdma);
static void I2C3_TX_DMA_Complete(DMA_HandleTypeDef *hdma);
static void I2C3_TX_DMA_Error   (DMA_HandleTypeDef *hdma);
#endif


/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
  ARM_I2C_API_VERSION,
  ARM_I2C_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_I2C_CAPABILITIES DriverCapabilities = { 
  0U                    // Does not support 10-bit addressing
#if (defined(ARM_I2C_API_VERSION) && (ARM_I2C_API_VERSION >= 0x203U))
, 0U                    // Reserved bits
#endif
};


#if defined(MX_I2C1)
/* Function prototypes */
void I2C1_EV_IRQHandler (void);
void I2C1_ER_IRQHandler (void);

#if defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
extern I2C_HandleTypeDef hi2c1;
#endif

/* I2C1 DMA */
#if defined(MX_I2C1_RX_DMA_Instance) && defined(MX_I2C1_TX_DMA_Instance)
static const I2C_DMA I2C1_RX_DMA = {
  &hdma_i2c1_rx,
  &I2C1_RX_DMA_Complete,
  &I2C1_RX_DMA_Error,
  #if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
  MX_I2C1_RX_DMA_Instance,
  MX_I2C1_RX_DMA_IRQn,
  { 0U, 0U, 0U },
  MX_I2C1_RX_DMA_Channel,
  MX_I2C1_RX_DMA_Priority
  #endif
};
static const I2C_DMA I2C1_TX_DMA = {
  &hdma_i2c1_tx,
  &I2C1_TX_DMA_Complete,
  &I2C1_TX_DMA_Error,
  #if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
  MX_I2C1_TX_DMA_Instance,
  MX_I2C1_TX_DMA_IRQn,
  { 0U, 0U, 0U },
  MX_I2C1_TX_DMA_Channel,
  MX_I2C1_TX_DMA_Priority
  #endif
};
#endif

/* I2C1 Information (Run-Time) */
static I2C_INFO I2C1_Info;

/* I2C1 Resources */
static I2C_RESOURCES I2C1_Resources = {
#if defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
  &hi2c1,
#endif
  I2C1,
#if defined(MX_I2C1_RX_DMA_Instance) && defined(MX_I2C1_TX_DMA_Instance)
  &I2C1_RX_DMA,
  &I2C1_TX_DMA,
#else
  NULL,
  NULL,
#endif
  {
    MX_I2C1_SCL_GPIOx,
    MX_I2C1_SDA_GPIOx,
    MX_I2C1_SCL_GPIO_Pin,
    MX_I2C1_SDA_GPIO_Pin,
    MX_I2C1_SCL_GPIO_PuPdOD,
    MX_I2C1_SDA_GPIO_PuPdOD,
    MX_I2C1_SCL_GPIO_AF,
    MX_I2C1_SDA_GPIO_AF
  },
  I2C1_EV_IRQn,
  I2C1_ER_IRQn,
  0U,
  &I2C1_Info
};

#endif /* MX_I2C1 */

#if defined(MX_I2C2)
/* Function prototypes */
void I2C2_EV_IRQHandler (void);
void I2C2_ER_IRQHandler (void);

#if defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
extern I2C_HandleTypeDef hi2c2;
#endif

/* I2C2 DMA */
#if defined(MX_I2C2_RX_DMA_Instance) && defined(MX_I2C2_TX_DMA_Instance)
static const I2C_DMA I2C2_RX_DMA = {
  &hdma_i2c2_rx,
  I2C2_RX_DMA_Complete,
  I2C2_RX_DMA_Error,
  #if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
  MX_I2C2_RX_DMA_Instance,
  MX_I2C2_RX_DMA_IRQn,
  { 0U, 0U, 0U },
  MX_I2C2_RX_DMA_Channel,
  MX_I2C2_RX_DMA_Priority
  #endif
};
static const I2C_DMA I2C2_TX_DMA = {
  &hdma_i2c2_tx,
  I2C2_TX_DMA_Complete,
  I2C2_TX_DMA_Error,
  #if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
  MX_I2C2_TX_DMA_Instance,
  MX_I2C2_TX_DMA_IRQn,
  { 0U, 0U, 0U },
  MX_I2C2_TX_DMA_Channel,
  MX_I2C2_TX_DMA_Priority
  #endif
};
#endif

/* I2C2 Information (Run-Time) */
static I2C_INFO I2C2_Info;

/* I2C2 Resources */
static I2C_RESOURCES I2C2_Resources = {
#if defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
  &hi2c2,
#endif
  I2C2,
#if defined(MX_I2C2_RX_DMA_Instance) && defined(MX_I2C2_TX_DMA_Instance)
  &I2C2_RX_DMA,
  &I2C2_TX_DMA,
#else
  NULL,
  NULL,
#endif
  {
    MX_I2C2_SCL_GPIOx,
    MX_I2C2_SDA_GPIOx,
    MX_I2C2_SCL_GPIO_Pin,
    MX_I2C2_SDA_GPIO_Pin,
    MX_I2C2_SCL_GPIO_PuPdOD,
    MX_I2C2_SDA_GPIO_PuPdOD,
    MX_I2C2_SCL_GPIO_AF,
    MX_I2C2_SDA_GPIO_AF
  },
  I2C2_EV_IRQn,
  I2C2_ER_IRQn,
  0U,
  &I2C2_Info
};

#endif /* MX_I2C2 */

#if defined(MX_I2C3)
/* Function prototypes */
void I2C3_EV_IRQHandler (void);
void I2C3_ER_IRQHandler (void);

#if defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
extern I2C_HandleTypeDef hi2c3;
#endif

/* I2C3 DMA */
#if defined(MX_I2C3_RX_DMA_Instance) && defined(MX_I2C3_TX_DMA_Instance)
static const I2C_DMA I2C3_RX_DMA = {
  &hdma_i2c3_rx,
  I2C3_RX_DMA_Complete,
  I2C3_RX_DMA_Error,
  #if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
  MX_I2C3_RX_DMA_Instance,
  MX_I2C3_RX_DMA_IRQn,
  { 0U, 0U, 0U },
  MX_I2C3_RX_DMA_Channel,
  MX_I2C3_RX_DMA_Priority
  #endif
};
static const I2C_DMA I2C3_TX_DMA = {
  &hdma_i2c3_tx,
  I2C3_TX_DMA_Complete,
  I2C3_TX_DMA_Error,
  #if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
  MX_I2C3_TX_DMA_Instance,
  MX_I2C3_TX_DMA_IRQn,
  { 0U, 0U, 0U },
  MX_I2C3_TX_DMA_Channel,
  MX_I2C3_TX_DMA_Priority
  #endif
};
#endif

/* I2C3 Information (Run-Time) */
static I2C_INFO I2C3_Info;

/* I2C3 Resources */
static I2C_RESOURCES I2C3_Resources = {
#if defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
  &hi2c3,
#endif
  I2C3,
#if defined(MX_I2C3_RX_DMA_Instance) && defined(MX_I2C3_TX_DMA_Instance)
  &I2C3_RX_DMA,
  &I2C3_TX_DMA,
#else
  NULL,
  NULL,
#endif
  {
    MX_I2C3_SCL_GPIOx,
    MX_I2C3_SDA_GPIOx,
    MX_I2C3_SCL_GPIO_Pin,
    MX_I2C3_SDA_GPIO_Pin,
    MX_I2C3_SCL_GPIO_PuPdOD,
    MX_I2C3_SDA_GPIO_PuPdOD,
    MX_I2C3_SCL_GPIO_AF,
    MX_I2C3_SDA_GPIO_AF
  },
  I2C3_EV_IRQn,
  I2C3_ER_IRQn,
  0U,
  &I2C3_Info
};

#endif /* MX_I2C3 */


#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
/**
  \fn          void Enable_GPIO_Clock (const GPIO_TypeDef *port)
  \brief       Enable GPIO clock
*/
static void Enable_GPIO_Clock (const GPIO_TypeDef *GPIOx) {
  if      (GPIOx == GPIOA) { __HAL_RCC_GPIOA_CLK_ENABLE(); }
  else if (GPIOx == GPIOB) { __HAL_RCC_GPIOB_CLK_ENABLE(); }
  else if (GPIOx == GPIOC) { __HAL_RCC_GPIOC_CLK_ENABLE(); }
#if defined(GPIOD)
  else if (GPIOx == GPIOD) { __HAL_RCC_GPIOD_CLK_ENABLE(); }
#endif
#if defined(GPIOE)
  else if (GPIOx == GPIOE) { __HAL_RCC_GPIOE_CLK_ENABLE(); }
#endif
#if defined(GPIOF)
  else if (GPIOx == GPIOF) { __HAL_RCC_GPIOF_CLK_ENABLE(); }
#endif
#if defined(GPIOG)
  else if (GPIOx == GPIOG) { __HAL_RCC_GPIOG_CLK_ENABLE(); }
#endif
#if defined(GPIOH)
  else if (GPIOx == GPIOH) { __HAL_RCC_GPIOH_CLK_ENABLE(); }
#endif
#if defined(GPIOI)
  else if (GPIOx == GPIOI) { __HAL_RCC_GPIOI_CLK_ENABLE(); }
#endif
#if defined(GPIOJ)
  else if (GPIOx == GPIOJ) { __HAL_RCC_GPIOJ_CLK_ENABLE(); }
#endif
#if defined(GPIOK)
  else if (GPIOx == GPIOK) { __HAL_RCC_GPIOK_CLK_ENABLE(); }
#endif
}
#endif


/**
  \fn          ARM_DRV_VERSION I2C_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRV_VERSION
*/
static ARM_DRIVER_VERSION I2CX_GetVersion (void) {
  return DriverVersion;
}


/**
  \fn          ARM_I2C_CAPABILITIES I2C_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_I2C_CAPABILITIES
*/
static ARM_I2C_CAPABILITIES I2CX_GetCapabilities (void) {
  return DriverCapabilities;
}


/**
  \fn          int32_t I2C_Initialize (ARM_I2C_SignalEvent_t cb_event, I2C_RESOURCES *i2c)
  \brief       Initialize I2C Interface.
  \param[in]   cb_event  Pointer to \ref ARM_I2C_SignalEvent
  \param[in]   i2c   Pointer to I2C resources
  \return      \ref ARM_I2C_STATUS
*/
static int32_t I2C_Initialize (ARM_I2C_SignalEvent_t cb_event, I2C_RESOURCES *i2c) {
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
  GPIO_InitTypeDef GPIO_InitStruct;
#endif
  uint8_t flags;

  if (i2c->info->flags & I2C_INIT) { return ARM_DRIVER_OK; }

  flags = I2C_INIT;

  #if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
    /* Setup I2C pin configuration */
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;

    /* Configure SCL Pin */
    Enable_GPIO_Clock (i2c->io.scl_port);

    GPIO_InitStruct.Pin       = i2c->io.scl_pin;
    GPIO_InitStruct.Pull      = i2c->io.scl_pull;
    GPIO_InitStruct.Alternate = i2c->io.scl_af;

    HAL_GPIO_Init (i2c->io.scl_port, &GPIO_InitStruct);

    /* Configure SDA Pin */
    Enable_GPIO_Clock (i2c->io.sda_port);

    GPIO_InitStruct.Pin       = i2c->io.sda_pin;
    GPIO_InitStruct.Pull      = i2c->io.sda_pull;
    GPIO_InitStruct.Alternate = i2c->io.sda_af;

    HAL_GPIO_Init (i2c->io.sda_port, &GPIO_InitStruct);

    if ((i2c->dma_rx != NULL) && (i2c->dma_tx != NULL)) {
      i2c->dma_rx->h->Instance = i2c->dma_rx->stream;
      i2c->dma_tx->h->Instance = i2c->dma_tx->stream;

      /* DMA controller clock enable */
      __HAL_RCC_DMA1_CLK_ENABLE();

      /* Configure DMA receive stream */
      i2c->dma_rx->h->Init.Channel             = i2c->dma_rx->channel;
      i2c->dma_rx->h->Init.Direction           = DMA_PERIPH_TO_MEMORY;
      i2c->dma_rx->h->Init.PeriphInc           = DMA_PINC_DISABLE;
      i2c->dma_rx->h->Init.MemInc              = DMA_MINC_ENABLE;
      i2c->dma_rx->h->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      i2c->dma_rx->h->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
      i2c->dma_rx->h->Init.Mode                = DMA_NORMAL;
      i2c->dma_rx->h->Init.Priority            = i2c->dma_rx->priority;
      i2c->dma_rx->h->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
      i2c->dma_rx->h->Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
      i2c->dma_rx->h->Init.MemBurst            = DMA_MBURST_SINGLE;
      i2c->dma_rx->h->Init.PeriphBurst         = DMA_PBURST_SINGLE;

      /* Configure stream */
      if (HAL_DMA_Init (i2c->dma_rx->h) != HAL_OK) {
        return ARM_DRIVER_ERROR;
      }

      /* Configure DMA transmit stream */
      i2c->dma_tx->h->Init.Channel             = i2c->dma_tx->channel;
      i2c->dma_tx->h->Init.Direction           = DMA_MEMORY_TO_PERIPH;
      i2c->dma_tx->h->Init.PeriphInc           = DMA_PINC_DISABLE;
      i2c->dma_tx->h->Init.MemInc              = DMA_MINC_ENABLE;
      i2c->dma_tx->h->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      i2c->dma_tx->h->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
      i2c->dma_tx->h->Init.Mode                = DMA_NORMAL;
      i2c->dma_tx->h->Init.Priority            = i2c->dma_tx->priority;
      i2c->dma_tx->h->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
      i2c->dma_tx->h->Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
      i2c->dma_tx->h->Init.MemBurst            = DMA_MBURST_SINGLE;
      i2c->dma_tx->h->Init.PeriphBurst         = DMA_PBURST_SINGLE;

      /* Configure stream */
      if (HAL_DMA_Init (i2c->dma_tx->h) != HAL_OK) {
        return ARM_DRIVER_ERROR;
      }
    }
  #else
    i2c->h->Instance = i2c->reg;
  #endif

  if ((i2c->dma_rx != NULL) && (i2c->dma_tx != NULL)) {
    i2c->dma_rx->h->XferCpltCallback  = i2c->dma_rx->cb_complete;
    i2c->dma_rx->h->XferErrorCallback = i2c->dma_rx->cb_error;

    i2c->dma_tx->h->XferCpltCallback  = i2c->dma_tx->cb_complete;
    i2c->dma_tx->h->XferErrorCallback = i2c->dma_tx->cb_error;

    flags |= I2C_DMA_XFER;
  }

  /* Reset Run-Time information structure */
  memset (i2c->info, 0x00, sizeof (I2C_INFO));

  i2c->info->cb_event = cb_event;
  i2c->info->flags    = flags;

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t I2C_Uninitialize (I2C_RESOURCES *i2c)
  \brief       De-initialize I2C Interface.
  \param[in]   i2c  Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_Uninitialize (I2C_RESOURCES *i2c) {

  #if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
    /* Unconfigure SCL and SDA Pins */
    HAL_GPIO_DeInit(i2c->io.scl_port, i2c->io.scl_pin);
    HAL_GPIO_DeInit(i2c->io.sda_port, i2c->io.sda_pin);

    if ((i2c->dma_rx != NULL) && (i2c->dma_tx != NULL)) {
      i2c->dma_rx->h->Instance = NULL;
      i2c->dma_tx->h->Instance = NULL;
    }
  #else
    i2c->h->Instance = NULL;
  #endif

  i2c->info->flags = 0U;

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t ARM_I2C_PowerControl (ARM_POWER_STATE state, I2C_RESOURCES *i2c)
  \brief       Control I2C Interface Power.
  \param[in]   state  Power state
  \param[in]   i2c  Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_PowerControl (ARM_POWER_STATE state, I2C_RESOURCES *i2c) {

  switch (state) {
    case ARM_POWER_OFF:
      /* Enable I2C clock */
      if (i2c->reg == I2C1)      { __HAL_RCC_I2C1_CLK_ENABLE(); }
      #if (defined (I2C2) && (RTE_I2C2 != 0))
      else if (i2c->reg == I2C2) { __HAL_RCC_I2C2_CLK_ENABLE(); }
      #endif
      #if (defined (I2C3) && (RTE_I2C3 != 0))
      else if (i2c->reg == I2C3) { __HAL_RCC_I2C3_CLK_ENABLE(); }
      #endif
      else { return ARM_DRIVER_ERROR; }

      /* Disable I2C peripheral */
      i2c->reg->CR1 = 0;

      #if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
        /* Disable I2C IRQ */
        HAL_NVIC_DisableIRQ(i2c->ev_irq_num);
        HAL_NVIC_DisableIRQ(i2c->er_irq_num);

        /* Abort DMA streams and disable stream IRQs in NVIC */
        if ((i2c->info->flags & I2C_DMA_XFER) != 0U) {
          if (i2c->dma_rx->h->Instance != NULL) {
            HAL_DMA_Abort (i2c->dma_rx->h);
            HAL_NVIC_DisableIRQ (i2c->dma_rx->irq_num);
          }
          if (i2c->dma_tx->h->Instance != NULL) {
            HAL_DMA_Abort (i2c->dma_tx->h);
            HAL_NVIC_DisableIRQ (i2c->dma_tx->irq_num);
          }
        }

        /* Disable peripheral clock */
        if (i2c->reg == I2C1)      { __HAL_RCC_I2C1_CLK_DISABLE(); }
        #if (defined (I2C2) && (RTE_I2C2 != 0))
        else if (i2c->reg == I2C2) { __HAL_RCC_I2C2_CLK_DISABLE(); }
        #endif
        #if (defined (I2C3) && (RTE_I2C3 != 0))
        else if (i2c->reg == I2C3) { __HAL_RCC_I2C3_CLK_DISABLE(); }
        #endif
        else { return ARM_DRIVER_ERROR; }
      #else
        if (i2c->h->Instance != NULL) { HAL_I2C_MspDeInit (i2c->h);}
      #endif

      i2c->info->status.busy             = 0U;
      i2c->info->status.mode             = 0U;
      i2c->info->status.direction        = 0U;
      i2c->info->status.general_call     = 0U;
      i2c->info->status.arbitration_lost = 0U;
      i2c->info->status.bus_error        = 0U;

      i2c->info->flags &= ~I2C_POWER;
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if ((i2c->info->flags & I2C_INIT)  == 0U) {
        return ARM_DRIVER_ERROR;
      }
      if ((i2c->info->flags & I2C_POWER) != 0U) {
        return ARM_DRIVER_OK;
      }
      #if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
        /* Enable I2C clock */
        if (i2c->reg == I2C1)      { __HAL_RCC_I2C1_CLK_ENABLE(); }
        #if (defined (I2C2) && (RTE_I2C2 != 0))
        else if (i2c->reg == I2C2) { __HAL_RCC_I2C2_CLK_ENABLE(); }
        #endif
        #if (defined (I2C3) && (RTE_I2C3 != 0))
        else if (i2c->reg == I2C3) { __HAL_RCC_I2C3_CLK_ENABLE(); }
        #endif
        else { return ARM_DRIVER_ERROR; }

        /* Enable DMA IRQs in NVIC */
        if ((i2c->info->flags & I2C_DMA_XFER) != 0U) {
          HAL_NVIC_EnableIRQ (i2c->dma_rx->irq_num);
          HAL_NVIC_EnableIRQ (i2c->dma_tx->irq_num);
        }

        /* Clear and Enable I2C IRQ */
        HAL_NVIC_ClearPendingIRQ(i2c->ev_irq_num);
        HAL_NVIC_ClearPendingIRQ(i2c->er_irq_num);
        HAL_NVIC_EnableIRQ(i2c->ev_irq_num);
        HAL_NVIC_EnableIRQ(i2c->er_irq_num);
      #else
        HAL_I2C_MspInit (i2c->h);
      #endif

      /* Reset the peripheral */
      if (i2c->reg == I2C1) {
        __HAL_RCC_I2C1_FORCE_RESET();
        __NOP(); __NOP(); __NOP(); __NOP(); 
        __HAL_RCC_I2C1_RELEASE_RESET();
      }
      #if defined (I2C2)
      else if (i2c->reg == I2C2) {
        __HAL_RCC_I2C2_FORCE_RESET();
        __NOP(); __NOP(); __NOP(); __NOP(); 
        __HAL_RCC_I2C2_RELEASE_RESET();
      }
      #endif
      #if defined (I2C3)
      else {
        __HAL_RCC_I2C3_FORCE_RESET();
        __NOP(); __NOP(); __NOP(); __NOP(); 
        __HAL_RCC_I2C3_RELEASE_RESET();
      }
      #endif

      /* Enable event and error interrupts */
      i2c->reg->CR2 |= I2C_CR2_ITEVTEN | I2C_CR2_ITERREN;
      /* Disable buffer interrupts */
      i2c->reg->CR2 &= ~I2C_CR2_ITBUFEN;

      /* Enable clock stretching */
      i2c->reg->CR1 &= ~I2C_CR1_NOSTRETCH;

      /* Enable I2C peripheral */
      i2c->reg->CR1 |= I2C_CR1_PE;

      /* Enable acknowledge */
      i2c->reg->CR1 |= I2C_CR1_ACK;

      /* Ready for operation */
      i2c->info->flags |= I2C_POWER;

      break;
  }

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t I2C_MasterTransmit (uint32_t       addr,
                                           const uint8_t *data,
                                           uint32_t       num,
                                           bool           xfer_pending,
                                           I2C_RESOURCES *i2c)
  \brief       Start transmitting data as I2C Master.
  \param[in]   addr          Slave address (7-bit or 10-bit)
  \param[in]   data          Pointer to buffer with data to send to I2C Slave
  \param[in]   num           Number of data bytes to send
  \param[in]   xfer_pending  Transfer operation is pending - Stop condition will not be generated
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_MasterTransmit (uint32_t       addr,
                                   const uint8_t *data,
                                   uint32_t       num,
                                   bool           xfer_pending,
                                   I2C_RESOURCES *i2c) {
  uint32_t tick;

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((addr & ~((uint32_t)ARM_I2C_ADDRESS_10BIT | (uint32_t)ARM_I2C_ADDRESS_GC)) > 0x3FFU) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (i2c->info->status.busy) {
    return (ARM_DRIVER_ERROR_BUSY);
  }

  if ((i2c->info->ctrl & XFER_CTRL_XPENDING) == 0U) {
    /* New transfer */
    tick = HAL_GetTick();
    do {
      if ((i2c->reg->SR2 & I2C_SR2_BUSY) == 0U) {
        break;
      }
    } while ((HAL_GetTick() - tick) < ARM_I2C_BUSY_TIMEOUT);

    if ((i2c->reg->SR2 & I2C_SR2_BUSY) != 0U) {
      /* Bus is busy or locked */
      return (ARM_DRIVER_ERROR_BUSY);
    }
  }

  i2c->info->status.busy             = 1U;
  i2c->info->status.mode             = 1U;
  i2c->info->status.direction        = 0U;
  i2c->info->status.bus_error        = 0U;
  i2c->info->status.arbitration_lost = 0U;

  i2c->info->addr = (uint16_t)(addr);

  i2c->info->tx.cnt  = 0U;
  i2c->info->tx.data = (uint8_t *)((uint32_t)data);
  i2c->info->tx.num  = num;

  i2c->info->ctrl = 0U;

  if (xfer_pending) {
    i2c->info->ctrl |= XFER_CTRL_XPENDING;
  }

  if (i2c->dma_tx) {
    /* Enable stream */
    if (HAL_DMA_Start_IT (i2c->dma_tx->h, (uint32_t)data, (uint32_t)&(i2c->reg->DR), num) != HAL_OK) {
      return ARM_DRIVER_ERROR;
    }
  }

  /* Generate start and enable event interrupts */
  i2c->reg->CR2 &= ~I2C_CR2_ITEVTEN;
  i2c->reg->CR1 |=  I2C_CR1_START;
  i2c->reg->CR2 |=  I2C_CR2_ITEVTEN;

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t I2C_MasterReceive (uint32_t       addr,
                                          uint8_t       *data,
                                          uint32_t       num,
                                          bool           xfer_pending,
                                          I2C_RESOURCES *i2c)
  \brief       Start receiving data as I2C Master.
  \param[in]   addr          Slave address (7-bit or 10-bit)
  \param[out]  data          Pointer to buffer for data to receive from I2C Slave
  \param[in]   num           Number of data bytes to receive
  \param[in]   xfer_pending  Transfer operation is pending - Stop condition will not be generated
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_MasterReceive (uint32_t       addr,
                                  uint8_t       *data,
                                  uint32_t       num,
                                  bool           xfer_pending,
                                  I2C_RESOURCES *i2c) {
  uint32_t tick;

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }
  if ((addr & ~((uint32_t)ARM_I2C_ADDRESS_10BIT | (uint32_t)ARM_I2C_ADDRESS_GC)) > 0x3FFU) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (i2c->info->status.busy) {
    return (ARM_DRIVER_ERROR_BUSY);
  }

  if ((i2c->info->ctrl & XFER_CTRL_XPENDING) == 0U) {
    /* New transfer */
    tick = HAL_GetTick();
    do {
      if ((i2c->reg->SR2 & I2C_SR2_BUSY) == 0U) {
        break;
      }
    } while ((HAL_GetTick() - tick) < ARM_I2C_BUSY_TIMEOUT);

    if ((i2c->reg->SR2 & I2C_SR2_BUSY) != 0U) {
      /* Bus is busy or locked */
      return (ARM_DRIVER_ERROR_BUSY);
    }
  }

  i2c->info->status.busy             = 1U;
  i2c->info->status.mode             = 1U;
  i2c->info->status.direction        = 1U;
  i2c->info->status.bus_error        = 0U;
  i2c->info->status.arbitration_lost = 0U;

  i2c->info->addr = (uint16_t)(addr);

  i2c->info->rx.cnt  = 0U;
  i2c->info->rx.data = (uint8_t *)data;
  i2c->info->rx.num  = num;

  i2c->info->ctrl = 0U;

  if (xfer_pending) {
    i2c->info->ctrl |= XFER_CTRL_XPENDING;
  }

  /* Enable acknowledge generation */
  i2c->reg->CR1 |= I2C_CR1_ACK;

  if (i2c->dma_rx) {
    /* Enable stream */
    if (HAL_DMA_Start_IT (i2c->dma_rx->h, (uint32_t)&(i2c->reg->DR), (uint32_t)data, num) != HAL_OK) {
      return ARM_DRIVER_ERROR;
    }
    /* Permit generation of a NACK on the last received data */
    i2c->reg->CR2 |= I2C_CR2_LAST;
  }
  
  /* Generate start and enable event interrupts */
  i2c->reg->CR2 &= ~I2C_CR2_ITEVTEN;
  i2c->reg->CR1 |=  I2C_CR1_START;
  i2c->reg->CR2 |=  I2C_CR2_ITEVTEN;

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t I2C_SlaveTransmit (const uint8_t *data, uint32_t num, I2C_RESOURCES *i2c)
  \brief       Start transmitting data as I2C Slave.
  \param[in]   data          Pointer to buffer with data to send to I2C Master
  \param[in]   num           Number of data bytes to send
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_SlaveTransmit (const uint8_t *data, uint32_t num, I2C_RESOURCES *i2c) {

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (i2c->info->status.busy) {
    return (ARM_DRIVER_ERROR_BUSY);
  }

  i2c->info->status.bus_error    = 0U;
  i2c->info->status.general_call = 0U;

  i2c->info->tx.cnt  = 0;
  i2c->info->tx.data = (uint8_t *)((uint32_t)data);
  i2c->info->tx.num  = num;

  i2c->info->ctrl = XFER_CTRL_XSET;

  if (i2c->dma_tx) {
    /* Enable stream */
    if (HAL_DMA_Start_IT (i2c->dma_tx->h, (uint32_t)data, (uint32_t)&(i2c->reg->DR), num) != HAL_OK) {
      return ARM_DRIVER_ERROR;
    }
  }

  /* Enable acknowledge */
  i2c->reg->CR1 |= I2C_CR1_ACK;

  /* Enable event interrupts */
  i2c->reg->CR2 |= I2C_CR2_ITEVTEN;

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t I2C_SlaveReceive (uint8_t *data, uint32_t num, I2C_RESOURCES *i2c)
  \brief       Start receiving data as I2C Slave.
  \param[out]  data          Pointer to buffer for data to receive from I2C Master
  \param[in]   num           Number of data bytes to receive
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_SlaveReceive (uint8_t *data, uint32_t num, I2C_RESOURCES *i2c) {

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (i2c->info->status.busy) {
    return (ARM_DRIVER_ERROR_BUSY);
  }

  i2c->info->status.bus_error    = 0U;
  i2c->info->status.general_call = 0U;

  i2c->info->rx.cnt  = 0;
  i2c->info->rx.data = data;
  i2c->info->rx.num  = num;

  i2c->info->ctrl = XFER_CTRL_XSET;

  if (i2c->dma_rx) {
    /* Enable stream */
    if (HAL_DMA_Start_IT (i2c->dma_rx->h, (uint32_t)&(i2c->reg->DR), (uint32_t)data, num) != HAL_OK) {
      return ARM_DRIVER_ERROR;
    }
  }

  /* Enable acknowledge */
  i2c->reg->CR1 |= I2C_CR1_ACK;

  /* Enable event interrupts */
  i2c->reg->CR2 |= I2C_CR2_ITEVTEN;

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t I2C_GetDataCount (void)
  \brief       Get transferred data count.
  \return      number of data bytes transferred; -1 when Slave is not addressed by Master
*/
static int32_t I2C_GetDataCount (I2C_RESOURCES *i2c) {
  int32_t val;

  if ((i2c->info->status.mode == I2C_MODE_SLAVE) && ((i2c->info->ctrl & XFER_CTRL_ADDR_DONE) == 0U)) {
    /* Slave is not addressed */
    val = -1;
  }
  else {
    if (i2c->info->status.direction == I2C_DIR_TX) {
      val = (int32_t)i2c->info->tx.cnt;
    } else {
      val = (int32_t)i2c->info->rx.cnt;
    }
  }

  return (val);
}


/**
  \fn          int32_t I2C_Control (uint32_t control, uint32_t arg, I2C_RESOURCES *i2c)
  \brief       Control I2C Interface.
  \param[in]   control  operation
  \param[in]   arg      argument of operation (optional)
  \param[in]   i2c      pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2C_Control (uint32_t control, uint32_t arg, I2C_RESOURCES *i2c) {
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_PinState state;
  uint32_t i, pclk;
  uint32_t ccr;
  uint32_t trise;

  if ((i2c->info->flags & I2C_POWER) == 0U) {
    /* I2C not powered */
    return ARM_DRIVER_ERROR;
  }

  switch (control) {
    case ARM_I2C_OWN_ADDRESS:
      /* Enable/Disable General call */
      if (arg & ARM_I2C_ADDRESS_GC) {
        i2c->reg->CR1 |=  I2C_CR1_ENGC;
      } else {
        i2c->reg->CR1 &= ~I2C_CR1_ENGC;
      }
      /* Set own address and its length */
      if (arg & ARM_I2C_ADDRESS_10BIT) {
        i2c->reg->OAR1 = (arg & 0xFFU) |
                         (1U << 14)    |
                         (1U << 15)    |
                         ((arg << 1) & 0x0300U);
      } else {
        i2c->reg->OAR1 = ((arg << 1) & 0x03FFU) | (1U << 14);
      }
      break;

    case ARM_I2C_BUS_SPEED:
      pclk = HAL_RCC_GetPCLK1Freq();
      switch (arg) {
        case ARM_I2C_BUS_SPEED_STANDARD:
          /* Clock = 100kHz,  Rise Time = 1000ns */
          if (pclk > 50000000U) { return ARM_DRIVER_ERROR_UNSUPPORTED; }
          if (pclk <  2000000U) { return ARM_DRIVER_ERROR_UNSUPPORTED; }
          ccr   = (pclk /  100000U) / 2U;
          trise = (pclk / 1000000U) + 1U;
          break;
        case ARM_I2C_BUS_SPEED_FAST:
          /* Clock = 400kHz,  Rise Time = 300ns */
          if (pclk > 50000000U) { return ARM_DRIVER_ERROR_UNSUPPORTED; }
          if (pclk <  4000000U) { return ARM_DRIVER_ERROR_UNSUPPORTED; }
          if ((pclk >= 10000000U) && ((pclk % 10000000U) == 0U)) {
            ccr = I2C_CCR_FS | I2C_CCR_DUTY | ((pclk / 400000U) / 25U);
          } else {
            ccr = I2C_CCR_FS |                ((pclk / 400000U) / 3U);
          }
          trise = (((pclk / 1000000U) * 300U) / 1000U) + 1U;
          break;
        default:
          return ARM_DRIVER_ERROR_UNSUPPORTED;
      }

      i2c->reg->CR1   &= ~I2C_CR1_PE;           /* Disable I2C peripheral */
      i2c->reg->CR2   &= ~I2C_CR2_FREQ;
      i2c->reg->CR2   |=  pclk / 1000000U;
      i2c->reg->CCR    =  ccr;
      i2c->reg->TRISE  =  trise;
      i2c->reg->CR1   |=  I2C_CR1_PE;           /* Enable I2C peripheral */
      i2c->reg->CR1   |=  I2C_CR1_ACK;          /* Enable acknowledge    */
      break;

    case ARM_I2C_BUS_CLEAR:
      /* Configure SCl and SDA pins as GPIO pin */
      GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
      GPIO_InitStruct.Pull  = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;

      GPIO_InitStruct.Pin = i2c->io.scl_pin;
      HAL_GPIO_Init(i2c->io.scl_port, &GPIO_InitStruct);
      GPIO_InitStruct.Pin = i2c->io.sda_pin;
      HAL_GPIO_Init(i2c->io.sda_port, &GPIO_InitStruct);

      /* Pull SCL and SDA high */
      HAL_GPIO_WritePin (i2c->io.scl_port, i2c->io.scl_pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (i2c->io.sda_port, i2c->io.sda_pin, GPIO_PIN_SET);

      HAL_Delay (I2C_BUS_CLEAR_CLOCK_PERIOD);

      for (i = 0U; i < 9U; i++) {
        if (HAL_GPIO_ReadPin (i2c->io.sda_port, i2c->io.sda_pin) == GPIO_PIN_SET) {
          /* Break if slave released SDA line */
          break;
        }
        /* Clock high */
        HAL_GPIO_WritePin (i2c->io.scl_port, i2c->io.scl_pin, GPIO_PIN_SET);
        HAL_Delay (I2C_BUS_CLEAR_CLOCK_PERIOD/2);

        /* Clock low */
        HAL_GPIO_WritePin (i2c->io.scl_port, i2c->io.scl_pin, GPIO_PIN_RESET);
        HAL_Delay (I2C_BUS_CLEAR_CLOCK_PERIOD/2);
      }

      /* Check SDA state */
      state = HAL_GPIO_ReadPin (i2c->io.sda_port, i2c->io.sda_pin);

      /* Configure SDA and SCL pins as I2C peripheral pins */
      GPIO_InitStruct.Mode  = GPIO_MODE_AF_OD;
      GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;

      GPIO_InitStruct.Pin       = i2c->io.scl_pin;
      GPIO_InitStruct.Pull      = i2c->io.scl_pull;
      GPIO_InitStruct.Alternate = i2c->io.scl_af;

      HAL_GPIO_Init (i2c->io.scl_port, &GPIO_InitStruct);

      GPIO_InitStruct.Pin       = i2c->io.sda_pin;
      GPIO_InitStruct.Pull      = i2c->io.sda_pull;
      GPIO_InitStruct.Alternate = i2c->io.sda_af;

      HAL_GPIO_Init (i2c->io.sda_port, &GPIO_InitStruct);

      if (i2c->info->cb_event != NULL) {
        /* Control operation has finished */
        i2c->info->cb_event (ARM_I2C_EVENT_BUS_CLEAR);
      }

      return (state == GPIO_PIN_SET) ? ARM_DRIVER_OK : ARM_DRIVER_ERROR;

    case ARM_I2C_ABORT_TRANSFER:
      /* Disable DMA requests and I2C interrupts */
      i2c->reg->CR2 &= ~(I2C_CR2_DMAEN | I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN);

      if ((i2c->dma_rx != NULL) && (i2c->dma_tx != NULL)) {
        /* Disable DMA Streams */
        if (HAL_DMA_Abort (i2c->dma_rx->h) != HAL_OK) {
          return ARM_DRIVER_ERROR;
        }
        if (HAL_DMA_Abort (i2c->dma_tx->h) != HAL_OK) {
          return ARM_DRIVER_ERROR;
        }
      }
      /* Generate stop */
      /* Master generates stop after the current byte transfer */
      /* Slave releases SCL and SDA after the current byte transfer */
      i2c->reg->CR1 |= I2C_CR1_STOP;

      i2c->info->rx.num  = 0U;
      i2c->info->rx.cnt  = 0U;
      i2c->info->tx.num  = 0U;
      i2c->info->tx.cnt  = 0U;
      
      i2c->info->addr = 0U;
      i2c->info->ctrl = 0U;

      i2c->info->status.busy             = 0U;
      i2c->info->status.mode             = 0U;
      i2c->info->status.direction        = 0U;
      i2c->info->status.general_call     = 0U;
      i2c->info->status.arbitration_lost = 0U;
      i2c->info->status.bus_error        = 0U;

      /* Disable and reenable peripheral to clear some flags */
      i2c->reg->CR1 &= ~I2C_CR1_PE;
      i2c->reg->CR1 |=  I2C_CR1_PE;
      /* Enable acknowledge */
      i2c->reg->CR1 |=  I2C_CR1_ACK;
      break;

    default: return ARM_DRIVER_ERROR;
  }
  return ARM_DRIVER_OK;
}


/**
  \fn          ARM_I2C_STATUS I2C_GetStatus (I2C_RESOURCES *i2c)
  \brief       Get I2C status.
  \param[in]   i2c      pointer to I2C resources
  \return      I2C status \ref ARM_I2C_STATUS
*/
static ARM_I2C_STATUS I2C_GetStatus (I2C_RESOURCES *i2c) {
  return (i2c->info->status);
}


/**
  \fn          void I2C_EV_IRQHandler (I2C_RESOURCES *i2c)
  \brief       I2C Event Interrupt handler.
  \param[in]   i2c  Pointer to I2C resources
*/
static void I2C_EV_IRQHandler (I2C_RESOURCES *i2c) {
  I2C_TRANSFER_INFO volatile *rx = &i2c->info->rx;
  I2C_TRANSFER_INFO volatile *tx = &i2c->info->tx;
  I2C_INFO          volatile *tr = i2c->info;
  uint8_t  data;
  uint16_t sr1, sr2;
  uint32_t event;

  sr1 = (uint16_t)i2c->reg->SR1;

  if (sr1 & I2C_SR1_SB) {
    /* (EV5): start bit generated, send address */

    if (tr->addr & ARM_I2C_ADDRESS_10BIT) {
      /* 10-bit addressing mode */
      data = (uint8_t)(0xF0U | ((tr->addr >> 6) & 0x0EU));

      if (tr->ctrl & XFER_CTRL_RDIR) {
        tr->ctrl &= ~XFER_CTRL_RDIR;

        data |= (uint8_t)tr->status.direction;
      }
    }
    else {
      /* 7-bit addressing mode */
      data  = (uint8_t)(tr->addr << 1);
      data |= (uint8_t)tr->status.direction;
    }
    i2c->reg->DR = data;
  }
  else if (sr1 & I2C_SR1_ADD10) {
    /* (EV9): 10-bit address header sent, send device address LSB */
    i2c->reg->DR = (uint8_t)tr->addr;

    if (tr->status.direction == I2C_DIR_RX) {
      /* Master receiver generates repeated start in 10-bit addressing mode */
      tr->ctrl |= XFER_CTRL_RSTART;
    }
  }
  else if (sr1 & I2C_SR1_ADDR) {
    /* (EV6): addressing complete */

    if (tr->status.mode == I2C_MODE_MASTER) {
      if (tr->status.direction == I2C_DIR_RX) {
        /* Master receiver */
        if (rx->num == 1U) {
          i2c->reg->CR1 &= ~I2C_CR1_ACK;
        }

        /* Clear ADDR flag */
        i2c->reg->SR1;
        i2c->reg->SR2;

        if (tr->ctrl & XFER_CTRL_RSTART) {
          tr->ctrl &= ~XFER_CTRL_RSTART;
          /* Generate repeated start */
          i2c->reg->CR1 |= I2C_CR1_START;
          /* Re-send direction bit */
          tr->ctrl |= XFER_CTRL_RDIR;
        }
        else {
          if (rx->num == 1U) {
            if ((tr->ctrl & XFER_CTRL_XPENDING) == 0U) {
              i2c->reg->CR1 |= I2C_CR1_STOP;
            }
          }
          else if (rx->num == 2U) {
            i2c->reg->CR1 &= ~I2C_CR1_ACK;
            i2c->reg->CR1 |=  I2C_CR1_POS;

            /* Wait until BTF == 1 */
            tr->ctrl |= XFER_CTRL_WAIT_BTF;
          }
          else {
            if (rx->num == 3U) {
              /* Wait until BTF == 1 */
              tr->ctrl |= XFER_CTRL_WAIT_BTF;
            }
          }

          tr->ctrl |= XFER_CTRL_ADDR_DONE;
        }
      }
      else {
        /* Master transmitter */

        /* Clear ADDR flag */
        i2c->reg->SR1;
        i2c->reg->SR2;

        tr->ctrl |= XFER_CTRL_ADDR_DONE;
      }

      if (tr->ctrl & XFER_CTRL_ADDR_DONE) {
        /* Slave is addressed, activate transfer */
        if ((i2c->dma_rx != NULL) && (i2c->dma_tx != NULL)) {
          /* Enable DMA data transfer */
          i2c->reg->CR2 |= I2C_CR2_DMAEN;
        }
        else {
          /* Enable IRQ data transfer */
          i2c->reg->CR2 |= I2C_CR2_ITBUFEN;
        }
      }
    }
    else {
      /* Addressed as a Slave */
      tr->state = XFER_STATE_ADDR;

      /* Clear ADDR flag */
      sr1 = (uint16_t)i2c->reg->SR1;
      sr2 = (uint16_t)i2c->reg->SR2;

      if ((sr2 & I2C_SR2_TRA) != 0U) {
        /* Slave transmitter */
        tr->status.direction = I2C_DIR_TX;
      } else {
        /* Slave receiver */
        tr->status.direction = I2C_DIR_RX;
      }

      if (sr2 & I2C_SR2_GENCALL) {
        tr->status.general_call = 1U;
      } else {
        tr->status.general_call = 0U;
      }

      if (tr->cb_event != NULL) {
        event = 0U;

        if (tr->status.general_call) {
          event |= ARM_I2C_EVENT_GENERAL_CALL;
        }

        if (tr->status.direction == I2C_DIR_TX) {
          if (tx->cnt == tx->num) {
            tr->status.busy = 0U;

            tr->cb_event (event | ARM_I2C_EVENT_SLAVE_TRANSMIT);
          }
        }
        else {
          if (rx->cnt == rx->num) {
            tr->status.busy = 0U;

            tr->cb_event (event | ARM_I2C_EVENT_SLAVE_RECEIVE);
          }
        }
      }

      /* Driver is busy */
      tr->status.busy = 1U;

      if (tr->ctrl == XFER_CTRL_XSET) {
        /* Transfer operation is set */
        if ((i2c->info->flags & I2C_DMA_XFER) != 0U) {
          /* Enable DMA data transfer */
          i2c->reg->CR2 |= I2C_CR2_DMAEN;
        }
        else {
          /* Enable IRQ data transfer */
          i2c->reg->CR2 |= I2C_CR2_ITBUFEN;
        }
      }
      else {
        /* Transfer operation was not registered */
        if (tr->status.direction == I2C_DIR_RX) {
          if ((i2c->reg->OAR1 & I2C_OAR1_ADDMODE_Msk) == 0U) {
            /* 7-bit addressing mode */
            if (tr->ctrl != XFER_CTRL_XSET) {
              /* Nothing to receive, disable ACK */
              i2c->reg->CR1 &= ~I2C_CR1_ACK;
              /* ACK will be re-enabled on STOP */
            }
          }
        }
        else {
          /* Enable buffer interrupts as BTF will not be triggered */
          i2c->reg->CR2 |= I2C_CR2_ITBUFEN;
        }
      }
    }
  }
  else if (sr1 & I2C_SR1_STOPF) {
    /* Slave receiver: STOP condition detected */
    /* Set after ACK, not set after NACK       */
    tr->state = XFER_STATE_IDLE;

    /* Reenable ACK */
    i2c->reg->CR1 |= I2C_CR1_ACK;

    tr->status.busy = 0U;

    if (tr->cb_event != NULL) {
      if ((tr->ctrl & (XFER_CTRL_XSET | XFER_CTRL_XDONE)) == XFER_CTRL_XSET) {
        event = ARM_I2C_EVENT_TRANSFER_DONE | ARM_I2C_EVENT_TRANSFER_INCOMPLETE;

        if (tr->status.general_call) {
          event |= ARM_I2C_EVENT_GENERAL_CALL;
        }

        tr->cb_event (event);
      }
    }
  }
  else if ((i2c->reg->CR2 & I2C_CR2_ITBUFEN) != 0U) {
    /* BTF, RxNE or TxE interrupt */
    if (tr->status.mode == I2C_MODE_MASTER) {
      /* Master transfer */

      if (sr1 & I2C_SR1_TXE) {
        /* Master transmitter */
        i2c->reg->DR = tx->data[tx->cnt];

        tx->cnt++;
        if (tx->cnt == tx->num) {
          tr->ctrl |= XFER_CTRL_XDONE;

          i2c->reg->CR2 &= ~I2C_CR2_ITBUFEN;
          /* Wait for BTF */
        }
      }
      else {
        if (sr1 & I2C_SR1_RXNE) {
          /* Master receiver */

          if ((rx->num == 1U) || (rx->num > 3U)) {
            rx->data[rx->cnt++] = (uint8_t)i2c->reg->DR;
          }

          if (rx->num == 1U) {
            /* Single byte transfer completed */
            i2c->reg->CR2 &= ~I2C_CR2_ITBUFEN;

            if (tr->ctrl & XFER_CTRL_XPENDING) {
              i2c->reg->CR2 &= ~I2C_CR2_ITEVTEN;
            }
            /* (STOP was already sent during ADDR phase) */

            tr->status.busy = 0U;
            tr->status.mode = 0U;

            if (tr->cb_event) {
              tr->cb_event (ARM_I2C_EVENT_TRANSFER_DONE);
            }
          }
          else {
            if ((rx->num == 2U) || (rx->cnt == (rx->num - 3U))) {
              /* N > 2 byte reception, begin N-2 data reception */
              i2c->reg->CR2 &= ~I2C_CR2_ITBUFEN;
              /* Wait for BTF */
            }
          }
        }
      }
    }
    else {
      /* Slave transfer */
      if (sr1 & I2C_SR1_TXE) {
        /* Slave transmitter */
        tr->state = XFER_STATE_SLAVE_TX;

        if (tx->cnt < tx->num) {
          i2c->reg->DR = tx->data[tx->cnt];

          tx->cnt++;
          if (tx->cnt == tx->num) {
            tr->ctrl |=  XFER_CTRL_XDONE;
            
            i2c->reg->CR2 &= ~I2C_CR2_ITBUFEN;

            tr->status.busy = 0U;

            /* Transfer completed */
            if (tr->cb_event != NULL) {
              event = ARM_I2C_EVENT_TRANSFER_DONE;

              if (i2c->info->status.general_call) {
                /* This was general call address */
                event |= ARM_I2C_EVENT_GENERAL_CALL;
              }

              tr->cb_event (event);
            }
          }
        }
        else {
          /* Nothing to send, but keep communication going */
          i2c->reg->DR = (uint8_t)0xFF;
        }
      }
      else {
        if (sr1 & I2C_SR1_RXNE) {
          /* Slave receiver */
          tr->state = XFER_STATE_SLAVE_RX;

          data = (uint8_t)i2c->reg->DR;

          if (rx->cnt < rx->num) {
            /* Receive data */
            rx->data[rx->cnt] = data;

            rx->cnt++;
            if (rx->cnt == rx->num) {
              tr->ctrl |= XFER_CTRL_XDONE;
              
              i2c->reg->CR2 &= ~I2C_CR2_ITBUFEN;

              tr->status.busy = 0U;

              if (tr->cb_event != NULL) {
                event = ARM_I2C_EVENT_TRANSFER_DONE;

                if (tr->status.general_call) {
                  event |= ARM_I2C_EVENT_GENERAL_CALL;
                }
                tr->cb_event (event);
              }
            }
          }
        }
      }
    }
  }
  else if (((sr1 & I2C_SR1_BTF) != 0U) && ((i2c->reg->CR2 & I2C_CR2_ITBUFEN) == 0U)) {
    /* Byte Transfer Finished */
    if (tr->ctrl & XFER_CTRL_DMA_DONE) {
      if (tr->status.mode == I2C_MODE_MASTER) {
        if (tr->ctrl & XFER_CTRL_XPENDING) {
          /* Disable event interrupt */
          i2c->reg->CR2 &= ~I2C_CR2_ITEVTEN;
        }
        else {
          /* Generate stop condition */
          i2c->reg->CR1 |= I2C_CR1_STOP;
        }
        /* Dummy read, to clear BTF */
        i2c->reg->DR;

        tr->status.busy = 0U;
        tr->status.mode = 0U;

        if (tr->cb_event) {
          tr->cb_event (ARM_I2C_EVENT_TRANSFER_DONE);
        }
      }
      else {
        /* Master requests more data */
        tr->status.busy = 0U;
        tr->status.mode = 0U;

        /* Dummy read, to clear BTF */
        i2c->reg->DR;

        if (tr->cb_event != NULL) {
          event = ARM_I2C_EVENT_SLAVE_TRANSMIT;

          if (tr->status.general_call) {
            event |= ARM_I2C_EVENT_GENERAL_CALL;
          }

          tr->cb_event (event);

          if (tr->ctrl == XFER_CTRL_XSET) {
            /* Transfer was set, re-enable DMA */
            i2c->reg->CR2 |= I2C_CR2_DMAEN;
          }
        }
        
        if (tr->ctrl != XFER_CTRL_XSET) {
          /* New transfer is not set */
          i2c->reg->CR1 |= I2C_CR1_STOP;
        }
        /* Re-set transfer control flags */
        tr->ctrl |= XFER_CTRL_ADDR_DONE;
      }
    }
    else {
      if (tr->status.mode == I2C_MODE_MASTER) {
        if (sr1 & I2C_SR1_TXE) {
          /* End master transmit operation */
          if (tr->ctrl & XFER_CTRL_XPENDING) {
            i2c->reg->CR2 &= ~I2C_CR2_ITEVTEN;
          }
          else {
            i2c->reg->CR1 |= I2C_CR1_STOP;
          }
          /* Dummy read, to clear BTF */
          i2c->reg->DR;

          tr->status.busy = 0U;
          tr->status.mode = 0U;

          if (tr->cb_event) {
            tr->cb_event (ARM_I2C_EVENT_TRANSFER_DONE);
          }
        }
        else {
          if (sr1 & I2C_SR1_RXNE) {
            /* End master receiver operation */
            if ((rx->num == 2U) || (rx->cnt == (rx->num - 2U))) {
              /* Two bytes remaining */
              if (tr->ctrl & XFER_CTRL_XPENDING) {
                i2c->reg->CR2 &= ~I2C_CR2_ITEVTEN;
              }
              else {
                i2c->reg->CR1 |= I2C_CR1_STOP;
              }

              /* Read data N-1 and N */
              rx->data[rx->cnt++] = (uint8_t)i2c->reg->DR;
              rx->data[rx->cnt++] = (uint8_t)i2c->reg->DR;

              tr->status.busy = 0U;
              tr->status.mode = 0U;

              i2c->reg->CR1 &= ~I2C_CR1_POS;

              if (tr->cb_event) {
                tr->cb_event (ARM_I2C_EVENT_TRANSFER_DONE);
              }
            }
            else {
              /* Three bytes remaining */
              i2c->reg->CR1 &= ~I2C_CR1_ACK;
              /* Read data N-2 */
              rx->data[rx->cnt++] = (uint8_t)i2c->reg->DR;
            }
          }
        }
      }
      else {
        if (sr1 & I2C_SR1_TXE) {
          /* Slave transmitter */
          if ((tr->state == XFER_STATE_SLAVE_TX) && ((tr->ctrl & XFER_CTRL_XDONE) != 0U)) {
            /* Master requests mode data */
            if (tr->cb_event != NULL) {
              event = ARM_I2C_EVENT_SLAVE_TRANSMIT;

              if (tr->status.general_call) {
                event |= ARM_I2C_EVENT_GENERAL_CALL;
              }

              tr->cb_event (event);

              if (tr->ctrl == XFER_CTRL_XSET) {
                /* Transfer was set, re-enable buffer interrupts */
                i2c->reg->CR2 |= I2C_CR2_ITBUFEN;

                /* Re-set transfer control flags */
                tr->ctrl |= XFER_CTRL_ADDR_DONE;
              }
            }
          }
        }
        else {
          if (sr1 & I2C_SR1_RXNE) {
            if ((i2c->reg->OAR1 & I2C_OAR1_ADDMODE_Msk) != 0U) {
              /* 10-bit addressing mode, disable ACK */
              i2c->reg->CR1 &= ~I2C_CR1_ACK;
              /* ACK will be re-enabled on STOP */
            }
            /* Keep communication going */
            data = (uint8_t)i2c->reg->DR;
          }
        }
      }
    }
  }
}


/**
  \fn          void I2C_ER_IRQHandler (I2C_RESOURCES *i2c)
  \brief       I2C Error Interrupt handler.
  \param[in]   i2c  Pointer to I2C resources
*/
static void I2C_ER_IRQHandler (I2C_RESOURCES *i2c) {
  I2C_INFO *tr = i2c->info;
  uint32_t sr1 = i2c->reg->SR1;
  uint32_t evt = 0U;
  uint32_t err = 0U;

  if (sr1 & I2C_SR1_SMBALERT) {
    /* SMBus alert */
    err |= I2C_SR1_SMBALERT;
  }
  if (sr1 & I2C_SR1_TIMEOUT) {
    /* Timeout - SCL remained LOW for 25ms */
    err |= I2C_SR1_TIMEOUT;
  }
  if (sr1 & I2C_SR1_PECERR) {
    /* PEC Error in reception */
    err |= I2C_SR1_PECERR;
  }
  if (sr1 & I2C_SR1_OVR) {
    /* Overrun/Underrun */
    err |= I2C_SR1_OVR;
  }

  if (sr1 & I2C_SR1_AF) {
    /* Acknowledge failure - NACK received */
    err |= I2C_SR1_AF;

    if (tr->status.mode == I2C_MODE_MASTER) {
      /* Disable buffer interrupts */
      i2c->reg->CR2 &= ~I2C_CR2_ITBUFEN;

      /* Resets the communication */
      i2c->reg->CR1 |= I2C_CR1_STOP;

      evt = ARM_I2C_EVENT_TRANSFER_DONE;

      if ((i2c->info->ctrl & XFER_CTRL_ADDR_DONE) == 0U) {
        /* Addressing not done */
        evt |= ARM_I2C_EVENT_ADDRESS_NACK;
      }

      if ((i2c->info->ctrl & XFER_CTRL_XDONE) == 0U) {
        /* Transfer did not complete */
        evt |= ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
      }

      if (i2c->info->status.general_call) {
        /* This was generall call address */
        evt |= ARM_I2C_EVENT_GENERAL_CALL;
      }

      tr->tx.num = 0U;
      tr->ctrl   = 0U;

      tr->status.busy = 0U;
      tr->status.mode = 0U;
    }
    else {
      /* Slave received NACK */
      if ((tr->flags & I2C_DMA_XFER) == 0U) {
        /* Disable buffer interrupts */
        i2c->reg->CR2 &= ~I2C_CR2_ITBUFEN;

        if ((tr->ctrl & (XFER_CTRL_XSET | XFER_CTRL_XDONE)) == XFER_CTRL_XSET) {
          /* Transfer was set, but did not complete */
          evt = ARM_I2C_EVENT_TRANSFER_DONE | ARM_I2C_EVENT_TRANSFER_INCOMPLETE;

          if (i2c->info->status.general_call) {
            /* This was general call address */
            evt |= ARM_I2C_EVENT_GENERAL_CALL;
          }

          /* Release the bus */
          i2c->reg->CR1 |= I2C_CR1_STOP;

          tr->tx.num = 0U;
          tr->ctrl   = 0U;

          tr->state = XFER_STATE_IDLE;

          tr->status.busy = 0U;
        }
      }
      else {
        if ((tr->ctrl & (XFER_CTRL_XSET | XFER_CTRL_DMA_DONE | XFER_CTRL_XDONE)) == XFER_CTRL_XSET) {
          /* Transfer was set, but did not complete */
          evt = ARM_I2C_EVENT_TRANSFER_DONE | ARM_I2C_EVENT_TRANSFER_INCOMPLETE;

          if (i2c->info->status.general_call) {
            /* This was general call address */
            evt |= ARM_I2C_EVENT_GENERAL_CALL;
          }

          /* Release the bus */
          i2c->reg->CR1 |= I2C_CR1_STOP;

          tr->tx.num = 0U;
          tr->ctrl   = 0U;

          tr->state = XFER_STATE_IDLE;

          tr->status.busy = 0U;
        }
      }
    }
  }

  if (sr1 & I2C_SR1_ARLO) {
    /* Arbitration lost */
    err |= I2C_SR1_ARLO;

    /* Switch to slave mode */
    i2c->info->status.busy             = 0U;
    i2c->info->status.mode             = 0U;
    i2c->info->status.arbitration_lost = 1U;

    evt = ARM_I2C_EVENT_TRANSFER_DONE | ARM_I2C_EVENT_ARBITRATION_LOST;

    if ((i2c->info->ctrl & XFER_CTRL_XDONE) == 0U) {
      /* Transfer did not complete */
      evt |= ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
    }

    i2c->info->ctrl = 0U;
  }

  if (sr1 & I2C_SR1_BERR) {
    /* Bus error - misplaced start/stop */
    err |= I2C_SR1_BERR;

    i2c->info->status.bus_error = 1U;

    evt = ARM_I2C_EVENT_TRANSFER_DONE | ARM_I2C_EVENT_BUS_ERROR;

    if ((i2c->info->ctrl & XFER_CTRL_XDONE) == 0U) {
      /* Transfer did not complete */
      evt |= ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
    }

    if (i2c->info->status.mode == 0U) {
      /* Lines are released in slave mode */
      i2c->info->status.busy = 0U;

      i2c->info->ctrl = 0U;
    }
  }
  /* Abort DMA streams */
  if ((i2c->dma_tx != NULL) && (i2c->dma_rx != NULL)) {
    HAL_DMA_Abort (i2c->dma_tx->h);
    HAL_DMA_Abort (i2c->dma_rx->h);
  }

  /* Clear error flags */
  i2c->reg->SR1 &= ~err;

  if ((evt != 0) && (i2c->info->cb_event != NULL)) {
    i2c->info->cb_event (evt);
  }
}


#if (defined(MX_I2C1_TX_DMA_Instance) || \
     defined(MX_I2C2_TX_DMA_Instance) || \
     defined(MX_I2C3_TX_DMA_Instance))
/**
  \fn          void I2C_DMA_TxEvent (uint32_t event, I2C_RESOURCES *i2c)
  \brief       I2C DMA Transmit Event handler
  \param[in]   i2c  Pointer to I2C resources
*/
static void I2C_DMA_TxEvent (uint32_t event, I2C_RESOURCES *i2c) {
  uint32_t evt;
  (void) event;

  i2c->reg->CR2 &= ~I2C_CR2_DMAEN;

  i2c->info->tx.cnt = i2c->info->tx.num - __HAL_DMA_GET_COUNTER(i2c->dma_tx->h);

  /* Wait for BTF in I2C EV IRQ handler */
  i2c->info->ctrl |= XFER_CTRL_DMA_DONE | XFER_CTRL_XDONE;

  if (i2c->info->status.mode == I2C_MODE_SLAVE) {
    /* Slave Transmit transfer completed */
    i2c->info->status.busy = 0U;

    if (i2c->info->cb_event) {
      evt = ARM_I2C_EVENT_TRANSFER_DONE;

      if (i2c->info->status.general_call) {
        evt |= ARM_I2C_EVENT_GENERAL_CALL;
      }

      i2c->info->cb_event (evt);

      if (i2c->info->ctrl == XFER_CTRL_XSET) {
        /* New transfer set in callback, re-enable DMA */
        i2c->reg->CR2 |= I2C_CR2_DMAEN;
      }
    }
    /* If master requests more data we get BTF otherwise we get NACK */
  }
}
#endif


#if (defined(MX_I2C1_RX_DMA_Instance) || \
     defined(MX_I2C2_RX_DMA_Instance) || \
     defined(MX_I2C3_RX_DMA_Instance))
/**
  \fn          void I2C_DMA_RxEvent (uint32_t event, I2C_RESOURCES *i2c)
  \brief       I2C DMA Receive Event handler
  \param[in]   i2c  Pointer to I2C resources
*/
static void I2C_DMA_RxEvent (uint32_t event, I2C_RESOURCES *i2c) {
  (void) event;

  i2c->reg->CR2 &= ~I2C_CR2_DMAEN;

  i2c->info->rx.cnt = i2c->info->rx.num - __HAL_DMA_GET_COUNTER(i2c->dma_rx->h);

  if (i2c->info->status.mode == I2C_MODE_MASTER) {
    /* Master mode */
    if (i2c->info->ctrl & XFER_CTRL_XPENDING) {
      /* Transfer pending */
      i2c->reg->CR2 &= ~I2C_CR2_ITEVTEN;
    }
    else {
      if (i2c->info->rx.num != 1U) {
        i2c->reg->CR1 |= I2C_CR1_STOP;
      }
    }

    i2c->info->status.busy = 0U;
    i2c->info->status.mode = 0U;

    if (i2c->info->cb_event) {
      i2c->info->cb_event (ARM_I2C_EVENT_TRANSFER_DONE);
    }
  }
  else {
    i2c->info->ctrl |= XFER_CTRL_DMA_DONE | XFER_CTRL_XDONE;

    /* Slave mode */
    i2c->info->status.busy = 0U;

    if (i2c->info->cb_event) {
      i2c->info->cb_event (ARM_I2C_EVENT_TRANSFER_DONE);
    }
  }
}
#endif


#if defined(MX_I2C1)
/* I2C1 Driver wrapper functions */
static int32_t I2C1_Initialize (ARM_I2C_SignalEvent_t cb_event) {
  return I2C_Initialize(cb_event, &I2C1_Resources);
}
static int32_t I2C1_Uninitialize (void) {
  return I2C_Uninitialize(&I2C1_Resources);
}
static int32_t I2C1_PowerControl (ARM_POWER_STATE state) {
  return I2C_PowerControl(state, &I2C1_Resources);
}
static int32_t I2C1_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterTransmit(addr, data, num, xfer_pending, &I2C1_Resources);
}
static int32_t I2C1_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterReceive(addr, data, num, xfer_pending, &I2C1_Resources);
}
static int32_t I2C1_SlaveTransmit (const uint8_t *data, uint32_t num) {
  return I2C_SlaveTransmit(data, num, &I2C1_Resources);
}
static int32_t I2C1_SlaveReceive (uint8_t *data, uint32_t num) {
  return I2C_SlaveReceive(data, num, &I2C1_Resources);
}
static int32_t I2C1_GetDataCount (void) {
  return I2C_GetDataCount(&I2C1_Resources);
}
static int32_t I2C1_Control (uint32_t control, uint32_t arg) {
  return I2C_Control(control, arg, &I2C1_Resources);
}
static ARM_I2C_STATUS I2C1_GetStatus (void) {
  return I2C_GetStatus(&I2C1_Resources);
}
void I2C1_EV_IRQHandler (void) {
  I2C_EV_IRQHandler(&I2C1_Resources);
}
void I2C1_ER_IRQHandler (void) {
  I2C_ER_IRQHandler(&I2C1_Resources);
}

#if defined(MX_I2C1_RX_DMA_Instance) && defined(MX_I2C1_TX_DMA_Instance)
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
void I2C1_RX_DMA_Handler (void) {
  HAL_NVIC_ClearPendingIRQ(MX_I2C1_RX_DMA_IRQn);
  HAL_DMA_IRQHandler(&hdma_i2c1_rx);
}
void I2C1_TX_DMA_Handler (void) {
  HAL_NVIC_ClearPendingIRQ(MX_I2C1_TX_DMA_IRQn);
  HAL_DMA_IRQHandler(&hdma_i2c1_tx);
}
#endif
void I2C1_RX_DMA_Complete(DMA_HandleTypeDef *hdma) {
  (void)hdma;
  I2C_DMA_RxEvent (DMA_COMPLETED, &I2C1_Resources);
}
void I2C1_RX_DMA_Error(DMA_HandleTypeDef *hdma) {
  (void)hdma;
  I2C_DMA_RxEvent (DMA_ERROR, &I2C1_Resources);
}
void I2C1_TX_DMA_Complete(DMA_HandleTypeDef *hdma) {
  (void)hdma;
  I2C_DMA_TxEvent (DMA_COMPLETED, &I2C1_Resources);
}
void I2C1_TX_DMA_Error(DMA_HandleTypeDef *hdma) {
  (void)hdma;
  I2C_DMA_TxEvent (DMA_ERROR, &I2C1_Resources);
}
#endif

/* I2C1 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C1 = {
  I2CX_GetVersion,
  I2CX_GetCapabilities,
  I2C1_Initialize,
  I2C1_Uninitialize,
  I2C1_PowerControl,
  I2C1_MasterTransmit,
  I2C1_MasterReceive,
  I2C1_SlaveTransmit,
  I2C1_SlaveReceive,
  I2C1_GetDataCount,
  I2C1_Control,
  I2C1_GetStatus
};
#endif


#if defined(MX_I2C2)
/* I2C2 Driver wrapper functions */
static int32_t I2C2_Initialize (ARM_I2C_SignalEvent_t cb_event) {
  return I2C_Initialize(cb_event, &I2C2_Resources);
}
static int32_t I2C2_Uninitialize (void) {
  return I2C_Uninitialize(&I2C2_Resources);
}
static int32_t I2C2_PowerControl (ARM_POWER_STATE state) {
  return I2C_PowerControl(state, &I2C2_Resources);
}
static int32_t I2C2_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterTransmit(addr, data, num, xfer_pending, &I2C2_Resources);
}
static int32_t I2C2_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterReceive(addr, data, num, xfer_pending, &I2C2_Resources);
}
static int32_t I2C2_SlaveTransmit (const uint8_t *data, uint32_t num) {
  return I2C_SlaveTransmit(data, num, &I2C2_Resources);
}
static int32_t I2C2_SlaveReceive (uint8_t *data, uint32_t num) {
  return I2C_SlaveReceive(data, num, &I2C2_Resources);
}
static int32_t I2C2_GetDataCount (void) {
  return I2C_GetDataCount(&I2C2_Resources);
}
static int32_t I2C2_Control (uint32_t control, uint32_t arg) {
  return I2C_Control(control, arg, &I2C2_Resources);
}
static ARM_I2C_STATUS I2C2_GetStatus (void) {
  return I2C_GetStatus(&I2C2_Resources);
}
void I2C2_EV_IRQHandler (void) {
  I2C_EV_IRQHandler(&I2C2_Resources);
}
void I2C2_ER_IRQHandler (void) {
  I2C_ER_IRQHandler(&I2C2_Resources);
}

#if defined(MX_I2C2_RX_DMA_Instance) && defined(MX_I2C2_TX_DMA_Instance)
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
void I2C2_RX_DMA_Handler (void) {
  HAL_NVIC_ClearPendingIRQ(MX_I2C2_RX_DMA_IRQn);
  HAL_DMA_IRQHandler(&hdma_i2c2_rx);
}
void I2C2_TX_DMA_Handler (void) {
  HAL_NVIC_ClearPendingIRQ(MX_I2C2_TX_DMA_IRQn);
  HAL_DMA_IRQHandler(&hdma_i2c2_tx);
}
#endif
void I2C2_RX_DMA_Complete(DMA_HandleTypeDef *hdma) {
  (void)hdma;
  I2C_DMA_RxEvent (DMA_COMPLETED, &I2C2_Resources);
}
void I2C2_RX_DMA_Error(DMA_HandleTypeDef *hdma) {
  (void)hdma;
  I2C_DMA_RxEvent (DMA_ERROR, &I2C2_Resources);
}
void I2C2_TX_DMA_Complete(DMA_HandleTypeDef *hdma) {
  (void)hdma;
  I2C_DMA_TxEvent (DMA_COMPLETED, &I2C2_Resources);
}
void I2C2_TX_DMA_Error(DMA_HandleTypeDef *hdma) {
  (void)hdma;
  I2C_DMA_TxEvent (DMA_ERROR, &I2C2_Resources);
}
#endif


/* I2C2 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C2 = {
  I2CX_GetVersion,
  I2CX_GetCapabilities,
  I2C2_Initialize,
  I2C2_Uninitialize,
  I2C2_PowerControl,
  I2C2_MasterTransmit,
  I2C2_MasterReceive,
  I2C2_SlaveTransmit,
  I2C2_SlaveReceive,
  I2C2_GetDataCount,
  I2C2_Control,
  I2C2_GetStatus
};
#endif


#if defined(MX_I2C3)
/* I2C3 Driver wrapper functions */
static int32_t I2C3_Initialize (ARM_I2C_SignalEvent_t cb_event) {
  return I2C_Initialize(cb_event, &I2C3_Resources);
}
static int32_t I2C3_Uninitialize (void) {
  return I2C_Uninitialize(&I2C3_Resources);
}
static int32_t I2C3_PowerControl (ARM_POWER_STATE state) {
  return I2C_PowerControl(state, &I2C3_Resources);
}
static int32_t I2C3_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterTransmit(addr, data, num, xfer_pending, &I2C3_Resources);
}
static int32_t I2C3_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) {
  return I2C_MasterReceive(addr, data, num, xfer_pending, &I2C3_Resources);
}
static int32_t I2C3_SlaveTransmit (const uint8_t *data, uint32_t num) {
  return I2C_SlaveTransmit(data, num, &I2C3_Resources);
}
static int32_t I2C3_SlaveReceive (uint8_t *data, uint32_t num) {
  return I2C_SlaveReceive(data, num, &I2C3_Resources);
}
static int32_t I2C3_GetDataCount (void) {
  return I2C_GetDataCount(&I2C3_Resources);
}
static int32_t I2C3_Control (uint32_t control, uint32_t arg) {
  return I2C_Control(control, arg, &I2C3_Resources);
}
static ARM_I2C_STATUS I2C3_GetStatus (void) {
  return I2C_GetStatus(&I2C3_Resources);
}
void I2C3_EV_IRQHandler (void) {
  I2C_EV_IRQHandler(&I2C3_Resources);
}
void I2C3_ER_IRQHandler (void) {
  I2C_ER_IRQHandler(&I2C3_Resources);
}

#if defined(MX_I2C3_RX_DMA_Instance) && defined(MX_I2C3_TX_DMA_Instance)
#if defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
void I2C3_RX_DMA_Handler (void) {
  HAL_NVIC_ClearPendingIRQ(MX_I2C3_RX_DMA_IRQn);
  HAL_DMA_IRQHandler(&hdma_i2c3_rx);
}
void I2C3_TX_DMA_Handler (void) {
  HAL_NVIC_ClearPendingIRQ(MX_I2C3_TX_DMA_IRQn);
  HAL_DMA_IRQHandler(&hdma_i2c3_tx);
}
#endif
void I2C3_RX_DMA_Complete(DMA_HandleTypeDef *hdma) {
  (void)hdma;
  I2C_DMA_RxEvent (DMA_COMPLETED, &I2C3_Resources);
}
void I2C3_RX_DMA_Error(DMA_HandleTypeDef *hdma) {
  (void)hdma;
  I2C_DMA_RxEvent (DMA_ERROR, &I2C3_Resources);
}
void I2C3_TX_DMA_Complete(DMA_HandleTypeDef *hdma) {
  (void)hdma;
  I2C_DMA_TxEvent (DMA_COMPLETED, &I2C3_Resources);
}
void I2C3_TX_DMA_Error(DMA_HandleTypeDef *hdma) {
  (void)hdma;
  I2C_DMA_TxEvent (DMA_ERROR, &I2C3_Resources);
}
#endif


/* I2C3 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C3 = {
  I2CX_GetVersion,
  I2CX_GetCapabilities,
  I2C3_Initialize,
  I2C3_Uninitialize,
  I2C3_PowerControl,
  I2C3_MasterTransmit,
  I2C3_MasterReceive,
  I2C3_SlaveTransmit,
  I2C3_SlaveReceive,
  I2C3_GetDataCount,
  I2C3_Control,
  I2C3_GetStatus
};
#endif

/*! \endcond */
