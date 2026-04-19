#include "app_data_transmit.h"
#include "FreeRTOS.h"
#include "task.h"

extern RemoteData remote_data;

// 定义发送缓冲区
uint8_t comm_buffer[TX_PLOAD_WIDTH] = {0};

void app_transmit_data(void)
{
    per_SI24R1_TX_Mode();
    
    // 定义发送数据，报文格式为：3个字节帧头+10个字节数据+4个字节校验码
    comm_buffer[0] = FRAME_HEAD_CODE_1;
    comm_buffer[1] = FRAME_HEAD_CODE_2;
    comm_buffer[2] = FRAME_HEAD_CODE_3;
    // 打包遥控数据，高8位在前，低8位在后
    comm_buffer[3] = (remote_data.thr >> 8) & 0xFF;
    comm_buffer[4] = remote_data.thr & 0xFF;
    comm_buffer[5] = (remote_data.yaw >> 8) & 0xFF;
    comm_buffer[6] = remote_data.yaw & 0xFF;
    comm_buffer[7] = (remote_data.pit >> 8) & 0xFF;
    comm_buffer[8] = remote_data.pit & 0xFF;
    comm_buffer[9] = (remote_data.rol >> 8) & 0xFF;
    comm_buffer[10] = remote_data.rol & 0xFF;

    // 以下两个数据发送完后需要重置，为了避免在此期间按键任务修改了这两个值，需要使用临界区来保护
    taskENTER_CRITICAL();
    comm_buffer[11] = remote_data.shutdown;
    remote_data.shutdown = 0;
    comm_buffer[12] = remote_data.fix_height;
    remote_data.fix_height = 0;
    taskEXIT_CRITICAL();

    uint32_t sum = 0;
    for (uint8_t i = 0; i < 13; i++)
    {
        sum += comm_buffer[i];
    }
    comm_buffer[13] = (sum >> 24) & 0xFF;
    comm_buffer[14] = (sum >> 16) & 0xFF;
    comm_buffer[15] = (sum >> 8) & 0xFF;
    comm_buffer[16] = sum & 0xFF;

    // 发送数据
    per_SI24R1_TxPacket(comm_buffer);

    per_SI24R1_RX_Mode();
}
