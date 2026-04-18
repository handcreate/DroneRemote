#include "per_key.h"
#include "FreeRTOS.h"
#include "task.h"

KeyType per_key_get(void)
{
    if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
    {
        // 电弧消抖
        vTaskDelay(5);
        while (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
        {
            vTaskDelay(1);
        }
        
        return KEY_UP;
    }
    else if(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET)
    {
        // 电弧消抖
        vTaskDelay(5);
        while (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET)
        {
            vTaskDelay(1);
        }

        return KEY_DOWN;
    }
    else if(HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET)
    {
        // 电弧消抖
        vTaskDelay(5);
        while (HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET)
        {
            vTaskDelay(1);
        }

        return KEY_LEFT;
    }
    else if(HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET)
    {
        // 电弧消抖
        vTaskDelay(5);
        while (HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET)
        {
            vTaskDelay(1);
        }

        return KEY_RIGHT;
    }
    else if(HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
    {
        // 电弧消抖
        vTaskDelay(5);
        while (HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
        {
            vTaskDelay(1);
        }
        return KEY_LEFT_X;
    }
    else if(HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
    {
        TickType_t tick_start = xTaskGetTickCount();
        // 电弧消抖
        vTaskDelay(5);
        while (HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
        {
            vTaskDelay(1);
        }
        if(xTaskGetTickCount() - tick_start > 1000)
        {
            return KEY_RIGHT_X_LONG;
        }
        return KEY_RIGHT_X;
    }
    return KEY_NONE;
}
