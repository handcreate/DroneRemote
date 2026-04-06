#include "per_tp4336.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

void per_tp4336_start(void)
{
    // 单击按键时间大于50ms 时，TP4336 开启 OUT 升压输出
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET);
    vTaskDelay(100);
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET);
}
