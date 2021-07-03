#include "i2c.h"

void I2C3_Configuration(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    /*I2C2外设时钟使能 */
    OLED_I2C_CLK_INIT(OLED_I2C_CLK, ENABLE);

    /*I2C2外设GPIO时钟使能 */
    RCC_AHB1PeriphClockCmd(OLED_I2C_SCL_GPIO_CLK | OLED_I2C_SDA_GPIO_CLK,
                           ENABLE);

    /*!< GPIO 配置 */
    GPIO_PinAFConfig(OLED_I2C_SCL_GPIO_PORT, OLED_I2C_SCL_SOURCE,
                     OLED_I2C_SCL_AF);
    GPIO_PinAFConfig(OLED_I2C_SDA_GPIO_PORT, OLED_I2C_SDA_SOURCE,
                     OLED_I2C_SDA_AF);

    /*!< 配置OLED_I2C引脚: SCL */
    
    
    GPIO_InitStructure.GPIO_Pin = OLED_I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  // I2C必须开漏输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(OLED_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

    /*!<配置OLED_I2C引脚: SDA */
    GPIO_InitStructure.GPIO_Pin = OLED_I2C_SDA_PIN;
    GPIO_Init(OLED_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

    /*!< GPIO 配置 */
    GPIO_SetBits(OLED_I2C_SDA_GPIO_PORT, OLED_I2C_SCL_PIN | OLED_I2C_SDA_PIN);

    /* I2C 配置 */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle =
        I2C_DutyCycle_2; /* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
    I2C_InitStructure.I2C_OwnAddress1 = OLED_ADDRESS;  //主机的I2C地址
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress =
        I2C_AcknowledgedAddress_7bit;             /* I2C的寻址模式 */
    I2C_InitStructure.I2C_ClockSpeed = I2C_Speed; /* 通信速率 */

    I2C_Init(OLED_I2C, &I2C_InitStructure); /* I2C2 初始化 */
    I2C_Cmd(OLED_I2C, ENABLE);              /* 使能 I2C2 */

    GPIO_PinAFConfig(OLED_I2C_SCL_GPIO_PORT, OLED_I2C_SCL_SOURCE,
                     OLED_I2C_SCL_AF);
    GPIO_PinAFConfig(OLED_I2C_SDA_GPIO_PORT, OLED_I2C_SDA_SOURCE,
                     OLED_I2C_SDA_AF);
}

/**
 * @brief  初始化I2C总线，使用I2C前需要调用
 * @param  无
 * @retval 无
 */
void I2C1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure1;

    /* Enable I2Cx clock */
    RCC_APB1PeriphClockCmd(SENSORS_I2C_RCC_CLK, ENABLE);

    /* Enable I2C GPIO clock */
    RCC_AHB1PeriphClockCmd(SENSORS_I2C_SCL_GPIO_CLK | SENSORS_I2C_SDA_GPIO_CLK,
                           ENABLE);

    /* Configure I2Cx pin: SCL ----------------------------------------*/
    GPIO_InitStructure.GPIO_Pin = SENSORS_I2C_SCL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    /* Connect pins to Periph */
    GPIO_PinAFConfig(SENSORS_I2C_SCL_GPIO_PORT, SENSORS_I2C_SCL_GPIO_PINSOURCE,
                     SENSORS_I2C_AF);
    GPIO_Init(SENSORS_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

    /* Configure I2Cx pin: SDA ----------------------------------------*/
    GPIO_InitStructure.GPIO_Pin = SENSORS_I2C_SDA_GPIO_PIN;

    /* Connect pins to Periph */
    GPIO_PinAFConfig(SENSORS_I2C_SDA_GPIO_PORT, SENSORS_I2C_SDA_GPIO_PINSOURCE,
                     SENSORS_I2C_AF);
    GPIO_Init(SENSORS_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

    I2C_DeInit(SENSORS_I2C);
    I2C_InitStructure1.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure1.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure1.I2C_OwnAddress1 = I2C_OWN_ADDRESS;
    I2C_InitStructure1.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure1.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure1.I2C_ClockSpeed = I2C_SPEED;

    /* Enable the I2C peripheral */
    I2C_Cmd(SENSORS_I2C, ENABLE);

    /* Initialize the I2C peripheral */
    I2C_Init(SENSORS_I2C, &I2C_InitStructure1);

    return;
}

void I2C_WriteByte(I2C_TypeDef * I2Cx,uint8_t slave_addr,uint8_t reg_addr, uint8_t data) {
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        ;

    I2C_GenerateSTART(I2Cx, ENABLE);  //开启I2Cx
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
        ; /*EV5,主模式*/

    I2C_Send7bitAddress(I2Cx, slave_addr,
                        I2C_Direction_Transmitter);  //器件地址
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;

    I2C_SendData(I2Cx, reg_addr);  //寄存器地址
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    I2C_SendData(I2Cx, data);  //发送数据
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    I2C_GenerateSTOP(I2Cx, ENABLE);  //关闭I2Cx总线
}

void I2C_ReadData(I2C_TypeDef * I2Cx,
    uint8_t slave_addr, uint8_t reg_addr, uint8_t *pBuffer,
    uint16_t NumByteToRead) {  // I2C读取数据串（器件地址，寄存器，内部地址，数量）
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        ;
    I2C_GenerateSTART(I2Cx, ENABLE);  //开启信号
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
        ;  //清除 EV5
    I2C_Send7bitAddress(I2Cx, slave_addr,
                        I2C_Direction_Transmitter);  //写入器件地址
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;  //清除 EV6
    I2C_Cmd(I2Cx, ENABLE);
    I2C_SendData(I2Cx, reg_addr);  //发送读的地址
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;                             //清除 EV8
    I2C_GenerateSTART(I2Cx, ENABLE);  //开启信号
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
        ;  //清除 EV5
    I2C_Send7bitAddress(I2Cx, slave_addr,
                        I2C_Direction_Receiver);  //将器件地址传出，主机为读
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        ;  //清除EV6
    while (NumByteToRead) {
        if (NumByteToRead == 1) {  //只剩下最后一个数据时进入 if 语句
            I2C_AcknowledgeConfig(I2Cx, DISABLE);  //最后有一个数据时关闭应答位
            I2C_GenerateSTOP(I2Cx, ENABLE);  //最后一个数据时使能停止位
        }
        if (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {  //读取数据
            *pBuffer = I2C_ReceiveData(I2Cx);  //调用库函数将数据取出到 pBuffer
            pBuffer++;                         //指针移位
            NumByteToRead--;                   //字节数减 1
        }
    }
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
}
	
