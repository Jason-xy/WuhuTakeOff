/**
 ******************************************************************************
 * �ļ�����: gy-86.h
 * ��    ��: Jason_xy
 * ���˲��ͣ�https://jason-xy.cn
 * ��    ��: V1.2.2
 * ��д����: 2020-10-2
 * ��    ��: GY-86��ʼ��
 ******************************************************************************
 * ˵����
 * 1.ʹ����GY-86ģ�顣
 * 2.��Ҫʵ�����ú�I2C������
 * 3.��Ҫ�������ú�MPU6050��
 * 4.��Ҫ�������ú�HMC5883L��
 * 5.������У׼���ݼ��㹫ʽ Gx=Gyro_x-Gyro_xFix��
 * 6.�ش���У׼���ݼ��㹫ʽ Mx=MagScaleX*Mag_x-offsetMagX��
 *
 * ���ܣ�
 * 1.MPU6050��ʼ����
 * 2.hmc5883l�ش��ǳ�ʼ����
 * 3.GY-86��ʼ����
 * 4.I2C������д��
 * 5.���������ݻ�ȡ��
 * 6.���ٶȼ����ݻ�ȡ��
 * 7.�¶ȼ����ݻ�ȡ�ͽ�����
 * 8.�ش������ݻ�ȡ��
 * 9.�궨������Ĵ�����ַ��
 * 10.��������ƫ������
 * 11.������У׼��
 * 12.��������ʽ���������
 * 13.��������ʽ���������
 *
 * ���£�
 * 2020-12-15
 * 1.���ݻ�ȡ�޸�uint16_tΪshort��
 * 2.�����������ƫ������
 * 2020-12-16
 * 1.�¶ȴ���������Bug�޸���
 * 2020-12-17
 * 1.��Ӵ�����У׼��8��У׼��ʽ��
 * 2.��������ʽ���ݶ�����
 * 3.��������ʽ���ݶ�����
 * 2020-12-20
 * 1.�޸�I2C��д��ڲ����������ṩDMP�ӿڡ�
 * 2021-9-12
 * 1.�����������ķ����ع�����
 * 2.��Ӹ�˹ţ�ٵ���У׼�㷨��
 * 2021-11-27
 * 1.���ȥ��ֵ�Ļ��������˲��㷨
 * 2.������ֶ��׵�ͨ�˲�
 ******************************************************************************
 */
#include "gy-86.h"

// GY-86
GY_86 *GY86;
Vector3f_t Accel_raw[6], Gyro_raw[6], Mag_raw[6];

// window filter
float Accel_x_buffer[WINDOW_SIZE];
int Accel_x_head;
float Accel_y_buffer[WINDOW_SIZE];
int Accel_y_head;
float Accel_z_buffer[WINDOW_SIZE];
int Accel_z_head;
float Gyro_x_buffer[WINDOW_SIZE];
int Gyro_x_head;
float Gyro_y_buffer[WINDOW_SIZE];
int Gyro_y_head;
float Gyro_z_buffer[WINDOW_SIZE];
int Gyro_z_head;

// PT2 filter
pt2Filter_t Accel_x_PT2, Accel_y_PT2, Accel_z_PT2, Gyro_x_PT2, Gyro_y_PT2, Gyro_z_PT2;

// GY-86��ʼ������
void GY86_Init(void)
{
  MPU6050_Init();
  MPU_Write_Byte(MPU_ADDR, MPU_CFG, 0x02);  //��MPU��CFG�Ĵ����ĵڶ�λ����Ϊ1������λ��ʹ��MPUʱ����
  MPU_Write_Byte(MPU_ADDR, MPU_CTRL, 0x00); //��MPU��CTRL�Ĵ����ĵ���λ����Ϊ0��������һ����ͬ����bypassģʽ
  HAL_Delay(200);
  HMC_Init(); // HMC��ʼ��
  GY86_OOPinit(GY86);
  GY86_Offset();
  pt2init();
}

//����ʵ����
void GY86_OOPinit(GY_86 *GY86)
{
  GY86->Accel = (Accelerometer *)malloc(sizeof(Accelerometer));
  GY86->Gyro = (Gyroscope *)malloc(sizeof(Gyroscope));
  GY86->Mag = (Magnetic *)malloc(sizeof(Magnetic));
  GY86->MS = (MS561101BA *)malloc(sizeof(MS561101BA));
  // dataMem
  GY86->Accel->data = (AccelData *)malloc(sizeof(AccelData));
  GY86->Gyro->data = (GyroData *)malloc(sizeof(GyroData));
  GY86->Mag->data = (MagData *)malloc(sizeof(MagData));
  // data init
  GY86->Accel->data->Accel_offset.x = 0;
  GY86->Accel->data->Accel_offset.x = 0;
  GY86->Accel->data->Accel_offset.x = 0;
  GY86->Accel->data->Accel_scale.x = 1;
  GY86->Accel->data->Accel_scale.y = 1;
  GY86->Accel->data->Accel_scale.z = 1;
  GY86->Accel->data->Accel_raw.x = 0;
  GY86->Accel->data->Accel_raw.y = 0;
  GY86->Accel->data->Accel_raw.z = 0;
  GY86->Accel->data->Accel_ms2.x = 0;
  GY86->Accel->data->Accel_ms2.y = 0;
  GY86->Accel->data->Accel_ms2.z = 0;
  GY86->Accel->getAccelerometer_raw = MPU_Get_Accelerometer;

  GY86->Gyro->data->Gyro_offset.x = 0;
  GY86->Gyro->data->Gyro_offset.y = 0;
  GY86->Gyro->data->Gyro_offset.z = 0;
  GY86->Gyro->data->Gyro_scale.x = 1;
  GY86->Gyro->data->Gyro_scale.y = 1;
  GY86->Gyro->data->Gyro_scale.z = 1;
  GY86->Gyro->data->Gyro_raw.x = 0;
  GY86->Gyro->data->Gyro_raw.y = 0;
  GY86->Gyro->data->Gyro_raw.z = 0;
  GY86->Gyro->data->Gyro_ds.x = 0;
  GY86->Gyro->data->Gyro_ds.y = 0;
  GY86->Gyro->data->Gyro_ds.z = 0;
  GY86->Gyro->getGyroscope_raw = MPU_Get_Gyroscope;

  GY86->Mag->data->Mag_offset.x = 0;
  GY86->Mag->data->Mag_offset.y = 0;
  GY86->Mag->data->Mag_offset.z = 0;
  GY86->Mag->data->Mag_scale.x = 1;
  GY86->Mag->data->Mag_scale.y = 1;
  GY86->Mag->data->Mag_scale.z = 1;
  GY86->Mag->data->Mag_raw.x = 0;
  GY86->Mag->data->Mag_raw.y = 0;
  GY86->Mag->data->Mag_raw.z = 0;
  GY86->Mag->data->Mag_d.x = 0;
  GY86->Mag->data->Mag_d.y = 0;
  GY86->Mag->data->Mag_d.z = 0;
}

//�õ�GY-86ֵ(ԭʼֵ)
void GY86_RawDataUpdate(void)
{
  //��������������
  MPU_Get_Gyroscope(GY86->Gyro->data);
#ifdef PT2_FILTER
  pt2update();
  GY86->Gyro->data->Gyro_ds.x = pt2FilterApply(&Gyro_x_PT2, (GY86->Gyro->data->Gyro_raw.x - GY86->Gyro->data->Gyro_offset.x) * GY86->Gyro->data->Gyro_scale.x / GYRO_250DPS);
  GY86->Gyro->data->Gyro_ds.y = pt2FilterApply(&Gyro_y_PT2, (GY86->Gyro->data->Gyro_raw.y - GY86->Gyro->data->Gyro_offset.y) * GY86->Gyro->data->Gyro_scale.y / GYRO_250DPS);
  GY86->Gyro->data->Gyro_ds.z = pt2FilterApply(&Gyro_z_PT2, (GY86->Gyro->data->Gyro_raw.z - GY86->Gyro->data->Gyro_offset.z) * GY86->Gyro->data->Gyro_scale.z / GYRO_250DPS);
#endif
#ifdef WINDOW_FILTER
  GY86->Gyro->data->Gyro_ds.x = window_filter((GY86->Gyro->data->Gyro_raw.x - GY86->Gyro->data->Gyro_offset.x) * GY86->Gyro->data->Gyro_scale.x / GYRO_250DPS, &Gyro_x_head, Gyro_x_buffer);
  GY86->Gyro->data->Gyro_ds.y = window_filter((GY86->Gyro->data->Gyro_raw.y - GY86->Gyro->data->Gyro_offset.y) * GY86->Gyro->data->Gyro_scale.y / GYRO_250DPS, &Gyro_y_head, Gyro_y_buffer);
  GY86->Gyro->data->Gyro_ds.z = window_filter((GY86->Gyro->data->Gyro_raw.z - GY86->Gyro->data->Gyro_offset.z) * GY86->Gyro->data->Gyro_scale.z / GYRO_250DPS, &Gyro_z_head, Gyro_z_buffer);
#endif
#ifdef NONE_FILTER
  GY86->Gyro->data->Gyro_ds.x = (GY86->Gyro->data->Gyro_raw.x - GY86->Gyro->data->Gyro_offset.x) * GY86->Gyro->data->Gyro_scale.x / GYRO_250DPS;
  GY86->Gyro->data->Gyro_ds.y = (GY86->Gyro->data->Gyro_raw.y - GY86->Gyro->data->Gyro_offset.y) * GY86->Gyro->data->Gyro_scale.y / GYRO_250DPS;
  GY86->Gyro->data->Gyro_ds.z = (GY86->Gyro->data->Gyro_raw.z - GY86->Gyro->data->Gyro_offset.z) * GY86->Gyro->data->Gyro_scale.z / GYRO_250DPS;
#endif
  //���¼��ٶȼ�����
  MPU_Get_Accelerometer(GY86->Accel->data);
#ifdef PT2_FILTER
  GY86->Accel->data->Accel_ms2.x = pt2FilterApply(&Accel_x_PT2, (GY86->Accel->data->Accel_raw.x - GY86->Accel->data->Accel_offset.x) * GY86->Accel->data->Accel_scale.x / ACCEL_2G);
  GY86->Accel->data->Accel_ms2.y = pt2FilterApply(&Accel_y_PT2, (GY86->Accel->data->Accel_raw.y - GY86->Accel->data->Accel_offset.y) * GY86->Accel->data->Accel_scale.y / ACCEL_2G);
  GY86->Accel->data->Accel_ms2.z = pt2FilterApply(&Accel_z_PT2, (GY86->Accel->data->Accel_raw.z - GY86->Accel->data->Accel_offset.z) * GY86->Accel->data->Accel_scale.z / ACCEL_2G);
#endif
#ifdef WINDOW_FILTER
  GY86->Accel->data->Accel_ms2.x = window_filter((GY86->Accel->data->Accel_raw.x - GY86->Accel->data->Accel_offset.x) * GY86->Accel->data->Accel_scale.x / ACCEL_2G, &Accel_x_head, Accel_x_buffer);
  GY86->Accel->data->Accel_ms2.y = window_filter((GY86->Accel->data->Accel_raw.y - GY86->Accel->data->Accel_offset.y) * GY86->Accel->data->Accel_scale.y / ACCEL_2G, &Accel_y_head, Accel_y_buffer);
  GY86->Accel->data->Accel_ms2.z = window_filter((GY86->Accel->data->Accel_raw.z - GY86->Accel->data->Accel_offset.z) * GY86->Accel->data->Accel_scale.z / ACCEL_2G, &Accel_z_head, Accel_z_buffer);
#endif
#ifdef NONE_FILTER
  GY86->Accel->data->Accel_ms2.x = (GY86->Accel->data->Accel_raw.x - GY86->Accel->data->Accel_offset.x) * GY86->Accel->data->Accel_scale.x / ACCEL_2G;
  GY86->Accel->data->Accel_ms2.y = (GY86->Accel->data->Accel_raw.y - GY86->Accel->data->Accel_offset.y) * GY86->Accel->data->Accel_scale.y / ACCEL_2G;
  GY86->Accel->data->Accel_ms2.z = (GY86->Accel->data->Accel_raw.z - GY86->Accel->data->Accel_offset.z) * GY86->Accel->data->Accel_scale.z / ACCEL_2G;
#endif
  //���´���������
  READ_HMCALL(GY86->Mag->data);
  //�����¶ȼ�����
  MPU_Get_Temperature(GY86->MS);
}

//�õ�������ֵ(ԭʼֵ)
// gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU_Get_Gyroscope(GyroData *data)
{
  uint8_t buf[6], res;

  res = MPU_Read_Len(MPU_ADDR, MPU6050_RA_GYRO_XOUT_H, 6, buf);
  if (res == 0)
  {
    data->Gyro_raw.x = (short)(((int16_t)buf[0] << 8) | buf[1]);
    data->Gyro_raw.y = (short)(((int16_t)buf[2] << 8) | buf[3]);
    data->Gyro_raw.z = (short)(((int16_t)buf[4] << 8) | buf[5]);
  }
  return res;
}

//�õ����ٶ�ֵ(ԭʼֵ)
// gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU_Get_Accelerometer(AccelData *data)
{
  uint8_t buf[6], res;
  res = MPU_Read_Len(MPU_ADDR, MPU6050_RA_ACCEL_XOUT_H, 6, buf);
  if (res == 0)
  {
    data->Accel_raw.x = (short)(((int16_t)buf[0] << 8) | buf[1]);
    data->Accel_raw.y = (short)(((int16_t)buf[2] << 8) | buf[3]);
    data->Accel_raw.z = (short)(((int16_t)buf[4] << 8) | buf[5]);
  }
  return res;
}

//�õ�������ֵ(ԭʼֵ)
// mx,my,mz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t READ_HMCALL(MagData *data)
{
  uint8_t buf[6], res;
  res = HMC_Read_Len(HMC_DATA_XMSB, 6, buf);
  if (res == 0)
  {
    data->Mag_raw.x = (short)(((int16_t)buf[0] << 8) | buf[1]);
    data->Mag_raw.y = (short)(((int16_t)buf[4] << 8) | buf[5]);
    data->Mag_raw.z = (short)(((int16_t)buf[2] << 8) | buf[3]);
    // ��ȡ��ԭ����Ϊ������ʽ���������ת��
    if (data->Mag_raw.x > 0x7fff)
      data->Mag_raw.x -= 0xffff;
    if (data->Mag_raw.y > 0x7fff)
      data->Mag_raw.y -= 0xffff;
    if (data->Mag_raw.z > 0x7fff)
      data->Mag_raw.z -= 0xffff;
  }
  return res;
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
uint8_t MPU_Get_Temperature(MS561101BA *MS)
{
  unsigned char buf[2];
  int16_t raw, res;

  res = MPU_Read_Len(MPU_ADDR, MPU6050_RA_TEMP_OUT_H, 2, buf);
  if (res == 0)
  {
    raw = (buf[0] << 8) | buf[1];
    MS->Temperature = (36.53f + ((double)raw) / 340.0f);
  }
  return res;
}

// GY-86��ƫУ��
void GY86_Offset(void)
{
  static float ACC_X = 0, ACC_Y = 0, ACC_Z = 0;
  static float GYRO_X = 0, GYRO_Y = 0, GYRO_Z = 0;
  int i = 0;
  for (i = 0; i < 100; i++)
  {
    GY86_RawDataUpdate();
    ACC_X += GY86->Accel->data->Accel_raw.x;
    ACC_Y += GY86->Accel->data->Accel_raw.y;
    ACC_Z += GY86->Accel->data->Accel_raw.z;

    GYRO_X += GY86->Gyro->data->Gyro_raw.x;
    GYRO_Y += GY86->Gyro->data->Gyro_raw.y;
    GYRO_Z += GY86->Gyro->data->Gyro_raw.z;
  }
  GY86->Accel->data->Accel_offset.x = ACC_X / 100;
  GY86->Accel->data->Accel_offset.y = ACC_Y / 100;
  GY86->Accel->data->Accel_offset.z = ACC_Z / 100 - 16384; //+-2g����

  GY86->Gyro->data->Gyro_offset.x = GYRO_X / 100;
  GY86->Gyro->data->Gyro_offset.y = GYRO_Y / 100;
  GY86->Gyro->data->Gyro_offset.z = GYRO_Z / 100;
}

// HMC��ʼ������
void HMC_Init(void)
{
  HMC_Write_Byte(HMC_CONFIGA, 0x58); // 01011000/����ƽ����4���������75Hz��������������
  HMC_Write_Byte(HMC_CONFIGB, 0xE0); // 11100000/�����������С
  HMC_Write_Byte(HMC_MODE, 0x00);    // 00000000/����Ϊ����ģʽ
}

//��ʼ��MPU6050
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU6050_Init(void)
{
  uint8_t res;
  extern I2C_HandleTypeDef MPU_I2C;
  MPU_Write_Byte(MPU_ADDR, MPU6050_RA_PWR_MGMT_1, 0X00); //����MPU6050
  HAL_Delay(100);
  MPU_Set_Gyro_Fsr(0); //�����Ǵ�����,��250dps
  HAL_Delay(100);
  MPU_Set_Accel_Fsr(0); //���ٶȴ�����,��2g
  HAL_Delay(100);
  MPU_Set_Rate(1000); //���ò�����1000Hz
  HAL_Delay(100);
  MPU_Write_Byte(MPU_ADDR, MPU6050_RA_USER_CTRL, 0X00); // I2C��ģʽ�ر�
  HAL_Delay(100);
  MPU_Write_Byte(MPU_ADDR, MPU6050_RA_FIFO_EN, 0X00); //�ر�FIFO
  HAL_Delay(100);
  MPU_Read_Byte(MPU_ADDR, MPU6050_RA_WHO_AM_I, &res);
  if (res == MPU_ADDR) //����ID��ȷ
  {
    MPU_Write_Byte(MPU_ADDR, MPU6050_RA_PWR_MGMT_1, 0X01); //����CLKSEL,PLL X��Ϊ�ο�
    MPU_Write_Byte(MPU_ADDR, MPU6050_RA_PWR_MGMT_2, 0X00); //���ٶ��������Ƕ�����
  }
  else
    return 1;
  return 0;
}

//����MPU6050�����Ǵ����������̷�Χ
// fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ��
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
  return MPU_Write_Byte(MPU_ADDR, MPU6050_RA_GYRO_CONFIG, fsr << 3); //���������������̷�Χ
}

//����MPU6050���ٶȴ����������̷�Χ
// fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ��
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
  return MPU_Write_Byte(MPU_ADDR, MPU6050_RA_ACCEL_CONFIG, fsr << 3); //���ü��ٶȴ����������̷�Χ
}

//����MPU6050�����ֵ�ͨ�˲���
// lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ��
uint8_t MPU_Set_LPF(uint16_t lpf)
{
  uint8_t data = 0;
  if (lpf >= 188)
    data = 1;
  else if (lpf >= 98)
    data = 2;
  else if (lpf >= 42)
    data = 3;
  else if (lpf >= 20)
    data = 4;
  else if (lpf >= 10)
    data = 5;
  else
    data = 6;
  return MPU_Write_Byte(MPU_ADDR, MPU6050_RA_CONFIG, data); //�������ֵ�ͨ�˲���
}

//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
// rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ��
uint8_t MPU_Set_Rate(uint16_t rate)
{
  uint8_t data;
  if (rate > 1000)
    rate = 1000;
  if (rate < 4)
    rate = 4;
  data = 1000 / rate - 1;
  data = MPU_Write_Byte(MPU_ADDR, MPU6050_RA_SMPLRT_DIV, data); //�������ֵ�ͨ�˲���
  return MPU_Set_LPF(rate / 2);                                 //�Զ�����LPFΪ�����ʵ�һ��
}

// IICдһ���ֽ�
// reg:�Ĵ�����ַ
// data:����
//����ֵ:0,����
//       ����,�������
uint8_t MPU_Write_Byte(uint8_t addr, uint8_t reg, uint8_t data)
{
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char W_Data = 0;

  W_Data = data;
  HAL_I2C_Mem_Write(&MPU_I2C, (addr << 1), reg, I2C_MEMADD_SIZE_8BIT, &W_Data, 1, 0xfff);
  return 0;
}

// IIC��һ���ֽ�
// reg:�Ĵ�����ַ
//����ֵ:����������
uint8_t MPU_Read_Byte(uint8_t addr, uint8_t reg, uint8_t *data)
{
  extern I2C_HandleTypeDef MPU_I2C;

  HAL_I2C_Mem_Read(&MPU_I2C, (addr << 1), reg, I2C_MEMADD_SIZE_8BIT, data, 1, 0xfff);
  return 0;
}

// IIC����д
// addr:������ַ
// reg:Ҫд�ļĴ�����ַ
// len:Ҫд�ĳ���
// buf:д���ݵ����ݴ洢��
//����ֵ:0,����
//       ����,�������
uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Write(&MPU_I2C, (addr << 1), reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  return 0;
}

// IIC������
// addr:������ַ
// reg:Ҫ��ȡ�ļĴ�����ַ
// len:Ҫ��ȡ�ĳ���
// buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//       ����,�������
uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Read(&MPU_I2C, (addr << 1), reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  return 0;
}

// IICдһ���ֽ�
// reg:�Ĵ�����ַ
// data:����
//����ֵ:0,����
//       ����,�������
uint8_t HMC_Write_Byte(uint8_t reg, uint8_t data)
{
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char W_Data = 0;

  W_Data = data;
  HAL_I2C_Mem_Write(&MPU_I2C, HMC_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &W_Data, 1, 0xfff);
  return 0;
}

// IIC��һ���ֽ�
// reg:�Ĵ�����ַ
//����ֵ:����������
uint8_t HMC_Read_Byte(uint8_t reg)
{
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char R_Data = 1;

  HAL_I2C_Mem_Read(&MPU_I2C, HMC_READ, reg, I2C_MEMADD_SIZE_8BIT, &R_Data, 1, 0xfff);
  return R_Data;
}

// IIC����д
// addr:������ַ
// reg:Ҫд�ļĴ�����ַ
// len:Ҫд�ĳ���
// buf:д���ݵ����ݴ洢��
//����ֵ:0,����
//       ����,�������
uint8_t HMC_Write_Len(uint8_t reg, uint8_t len, uint8_t *buf)
{
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Write(&MPU_I2C, HMC_WRITE, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  return 0;
}

// IIC������
// addr:������ַ
// reg:Ҫ��ȡ�ļĴ�����ַ
// len:Ҫ��ȡ�ĳ���
// buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//       ����,�������
uint8_t HMC_Read_Len(uint8_t reg, uint8_t len, uint8_t *buf)
{
  extern I2C_HandleTypeDef MPU_I2C;
  HAL_I2C_Mem_Read(&MPU_I2C, HMC_READ, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  return 0;
}

// window filter
float window_filter(float data, int *phead, float *window_buffer)
{
  int idx;
  float sum = 0;
  //װ�������ݣ����Ǿ�����
  window_buffer[*phead] = data;
  *phead = (*phead + 1) % WINDOW_SIZE;
  //�͵�����
  qsort(window_buffer, 0, WINDOW_SIZE - 1);
  //ȥ��ֵ���
  for (idx = 1; idx < WINDOW_SIZE - 1; idx++)
  {
    sum += window_buffer[idx];
  }

  return sum / (WINDOW_SIZE - 2);
}

void qsort(float *a, int left, int right)
{
  int i, j, temp;
  i = left;
  j = right;
  temp = a[left];
  if (left > right)
    return;
  while (i != j) /*i������jʱ��ѭ������*/
  {
    while (a[j] >= temp && j > i)
      j--;
    if (j > i)
      a[i++] = a[j];
    while (a[i] <= temp && j > i)
      i++;
    if (j > i)
      a[j--] = a[i];
  }
  a[i] = temp;
  qsort(a, left, i - 1);  /*��С�ڻ�׼Ԫ�صĲ��ֽ��п�������*/
  qsort(a, i + 1, right); /*�Դ��ڻ�׼Ԫ�صĲ��ֽ��п�������*/
}

// PT2 Low Pass filter
float PT2Dt;
float pt2FilterGain(float f_cut, float dT)
{
  const float order = 2.0f;
  const float orderCutoffCorrection = 1 / sqrtf(powf(2, 1.0f / order) - 1);
  float RC = 1 / (2 * orderCutoffCorrection * M_PIf * f_cut);
  // float RC = 1 / (2 * 1.553773974f * M_PIf * f_cut);
  // where 1.553773974 = 1 / sqrt( (2^(1 / order) - 1) ) and order is 2
  return dT / (RC + dT);
}

void pt2FilterInit(pt2Filter_t *filter, float k)
{
  filter->state = 0.0f;
  filter->state1 = 0.0f;
  filter->k = k;
}

void pt2FilterUpdateCutoff(pt2Filter_t *filter, float k)
{
  filter->k = k;
}

float pt2FilterApply(pt2Filter_t *filter, float input)
{
  filter->state1 = filter->state1 + filter->k * (input - filter->state1);
  filter->state = filter->state + filter->k * (filter->state1 - filter->state);
  return filter->state;
}

void pt2init(void)
{
  pt2FilterInit(&Accel_x_PT2, 0);
  pt2FilterInit(&Accel_y_PT2, 0);
  pt2FilterInit(&Accel_z_PT2, 0);
  pt2FilterInit(&Gyro_x_PT2, 0);
  pt2FilterInit(&Gyro_y_PT2, 0);
  pt2FilterInit(&Gyro_z_PT2, 0);
}

void pt2update(void)
{
  pt2FilterUpdateCutoff(&Accel_x_PT2, pt2FilterGain(ACCEL_LPF_CUTOFF_FREQ, PT2Dt));
  pt2FilterUpdateCutoff(&Accel_y_PT2, pt2FilterGain(ACCEL_LPF_CUTOFF_FREQ, PT2Dt));
  pt2FilterUpdateCutoff(&Accel_z_PT2, pt2FilterGain(ACCEL_LPF_CUTOFF_FREQ, PT2Dt));
  pt2FilterUpdateCutoff(&Gyro_x_PT2, pt2FilterGain(GYRO_LPF_CUTOFF_FREQ, PT2Dt));
  pt2FilterUpdateCutoff(&Gyro_y_PT2, pt2FilterGain(GYRO_LPF_CUTOFF_FREQ, PT2Dt));
  pt2FilterUpdateCutoff(&Gyro_z_PT2, pt2FilterGain(GYRO_LPF_CUTOFF_FREQ, PT2Dt));
}