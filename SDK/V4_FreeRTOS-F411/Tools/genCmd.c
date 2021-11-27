#include <stdio.h>
#include <string.h>

typedef unsigned char           uint8_t;  
typedef unsigned short int      uint16_t;  

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: ./genCmd <funcCode> <Cmd>\n");
        return 1;
    }

    uint8_t funcCode = atoi(argv[1]);
    uint16_t cmdCode = atoi(argv[2]);

    uint8_t send_buffer[20] = {0};
    send_buffer[0] = 0xAA;
    send_buffer[1] = 0xFF;
    send_buffer[2] = funcCode;
    send_buffer[3] = 0x02;
    send_buffer[4] = BYTE0(cmdCode);
    send_buffer[5] = BYTE1(cmdCode);
    //data check
    uint8_t check_sum1 = 0, check_sum2 = 0;
    uint8_t i = 0, _cnt = 6;
    for (i = 0; i < _cnt; i++)
    {
        check_sum1 += send_buffer[i];
        check_sum2 += check_sum1;
    }
    send_buffer[_cnt++] = check_sum1;
    send_buffer[_cnt++] = check_sum2;

    printf("Cmd:\n");
    for(i = 0; i < 8; i++){
        printf("%.2x", send_buffer[i]);
    }
    return 0;
}