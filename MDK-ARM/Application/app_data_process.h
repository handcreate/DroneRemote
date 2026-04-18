#ifndef __APP_DATA_PROCESS_H__
#define __APP_DATA_PROCESS_H__

#include "per_key.h"
#include "per_joystick.h"

typedef struct
{
    int16_t thr;
    int16_t yaw;
    int16_t pit;
    int16_t rol;
    uint8_t shutdown; // 关机
    uint8_t fix_height; // 定高
} RemoteData;

void app_process_key_data(void);

void app_process_joystick_data(void);

#endif //__APP_DATA_PROCESS_H__
