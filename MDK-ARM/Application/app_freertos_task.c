#include "app_freertos_task.h"
#include "com_debug.h"
#include "per_tp4336.h"

// 测试任务
void test_task( void *args );
#define TEST_TASK_STACK_SIZE 128
#define TEST_TASK_PRIORITY 1
#define TEST_TASK_NAME "test_task"
#define TEST_TASK_PERIOD 1000
TaskHandle_t test_task_handle;

// 电源启动任务，避免电源自动关闭
void power_task( void *args );
#define POWER_TASK_STACK_SIZE 128
#define POWER_TASK_PRIORITY 4
#define POWER_TASK_NAME "power_task"
#define POWER_TASK_PERIOD 10000
TaskHandle_t power_task_handle;

void app_freertos_start(void)
{
    xTaskCreate(test_task, 
        TEST_TASK_NAME, 
        TEST_TASK_STACK_SIZE, 
        NULL, 
        TEST_TASK_PRIORITY, 
        &test_task_handle);

    xTaskCreate(power_task, 
        POWER_TASK_NAME, 
        POWER_TASK_STACK_SIZE, 
        NULL, 
        POWER_TASK_PRIORITY, 
        &power_task_handle);

    vTaskStartScheduler();
}

void test_task( void *args )
{
    while (1)
    {
        DEBUG_PRINTF("test_task running\n");
        vTaskDelay(TEST_TASK_PERIOD);
    }
}

void power_task( void *args )
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, POWER_TASK_PERIOD);
        per_tp4336_start();
    }
}
