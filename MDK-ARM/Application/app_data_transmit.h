#ifndef __APP_DATA_TRANSMIT_H__
#define __APP_DATA_TRANSMIT_H__

#include "per_SI24R1.h"
#include "app_data_process.h"

// 定义三位帧头校验码
#define FRAME_HEAD_CODE_1 'X'
#define FRAME_HEAD_CODE_2 'I'
#define FRAME_HEAD_CODE_3 'S'


void app_transmit_data(void);

#endif /* __APP_DATA_TRANSMIT_H__ */
