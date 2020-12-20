#ifndef _DATA_TRANSFER_H
#define _DATA_TRANSFER_H

#include "stm32f4xx.h"
#define u8 	uint8_t
#define u16 uint16_t
#define s16 int16_t
#define s32 int32_t

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef struct {
    u8 send_version;
    u8 send_status;
    u8 send_senser;
    u8 send_pid1;
    u8 send_pid2;
    u8 send_pid3;
    u8 send_pid4;
    u8 send_pid5;
    u8 send_pid6;
    u8 send_rcdata;
    u8 send_offset;
    u8 send_motopwm;
    u8 send_power;
    u8 send_F1;
    u8 send_F2;
    u8 send_F3;
    u8 send_F4;
} dt_flag_t;

extern dt_flag_t f;

void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed);
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z);
void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6);
void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8);
void ANO_DT_Send_Fx_9float(u8 buf[], u8 Fx,
                    float _f1, float _f2, float _f3,
                    float _f4, float _f5, float _f6,
                    float _f7, float _f8, float _f9);

#endif /* _DATA_TRANSFER_H */

