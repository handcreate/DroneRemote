#include "app_data_process.h"
#include "FreeRTOS.h"
#include "task.h"
#include "com_debug.h"
#include "com_utils.h"

// 摇杆数据结构体 
JoystickStruct joystick = {0};
RemoteData remote_data = {0};

// 按键微调值
int16_t key_pit_offset = 0;
int16_t key_rol_offset = 0;

// 摇杆编译量
int16_t thr_offset = 0;
int16_t yaw_offset = 0;
int16_t pit_offset = 0;
int16_t rol_offset = 0;

// 校准摇杆函数
void app_calibrate_joystick(void)
{
    // 重置按键微调值
    key_pit_offset = 0;
    key_rol_offset = 0;
    // 多次读取求平均值
    int16_t thr_sum = 0;
    int16_t yaw_sum = 0;
    int16_t pit_sum = 0;
    int16_t rol_sum = 0;
    for(int i = 0; i < 10; i++)
    {
        app_process_joystick_data();
        thr_sum += joystick.thr - 0;
        yaw_sum += joystick.yaw - 500;
        pit_sum += joystick.pit - 500;
        rol_sum += joystick.rol - 500;
        vTaskDelay(10);
    }

    // 计算平均值，这里用 += 是为了防止二次校准又恢复到初始值
    thr_offset += thr_sum / 10;
    yaw_offset += yaw_sum / 10;
    pit_offset += pit_sum / 10;
    rol_offset += rol_sum / 10;
}

void app_process_key_data(void)
{
    KeyType key = per_key_get();
    if(key == KEY_UP)
    {
        key_pit_offset += 10;
    }
    else if(key == KEY_DOWN)
    {
        key_pit_offset -= 10;
    }
    else if(key == KEY_LEFT)
    {
        key_rol_offset -= 10;
    }
    else if(key == KEY_RIGHT)
    {
        key_rol_offset += 10;
    }
    else if(key == KEY_LEFT_X)
    {
        remote_data.shutdown = 1;
    }
    else if(key == KEY_RIGHT_X)
    {
        remote_data.fix_height = 1;
    }
    else if(key == KEY_RIGHT_X_LONG)
    {
        app_calibrate_joystick();
    }
}

void app_process_joystick_data(void)
{
    per_joystick_get((JoystickStruct *)&joystick);

    // 为了防止同优先级任务交替执行导致数据还未处理完就被其他任务使用，这里使用临界区来保护数据
    taskENTER_CRITICAL();

    // 处理范围和极性
    joystick.thr = joystick.thr * 1000 / 4095;
    joystick.yaw = 1000 - joystick.yaw * 1000 / 4095;
    joystick.pit = joystick.pit * 1000 / 4095;
    joystick.rol = 1000 - joystick.rol * 1000 / 4095;

    // 处理零偏校准
    joystick.thr -= thr_offset;
    joystick.yaw -= yaw_offset;
    joystick.pit -= pit_offset;
    joystick.rol -= rol_offset;

    // 叠加按键偏移
    joystick.pit += key_pit_offset;
    joystick.rol += key_rol_offset;

    // 限幅
    joystick.thr = clamp(joystick.thr, 0, 1000);
    joystick.yaw = clamp(joystick.yaw, 0, 1000);
    joystick.pit = clamp(joystick.pit, 0, 1000);
    joystick.rol = clamp(joystick.rol, 0, 1000);

    // 数据处理完成，退出临界区
    taskEXIT_CRITICAL();

    // 赋值给遥控数据结构体
    remote_data.thr = joystick.thr;
    remote_data.yaw = joystick.yaw;
    remote_data.pit = joystick.pit;
    remote_data.rol = joystick.rol;

    DEBUG_PRINTF("thr: %d, yaw: %d, pit: %d, rol: %d\n",
        joystick.thr, joystick.yaw, joystick.pit, joystick.rol);
}
