#include "app_freertos_task.h"
#include "com_debug.h"
#include "per_tp4336.h"
#include "per_SI24R1.h"
#include "app_data_process.h"

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

// 通讯任务
void comm_task( void *args );
#define COMM_TASK_STACK_SIZE 128
#define COMM_TASK_PRIORITY 3
#define COMM_TASK_NAME "comm_task"
#define COMM_TASK_PERIOD 6
TaskHandle_t comm_task_handle;

// 按键任务
void key_task( void *args );
#define KEY_TASK_STACK_SIZE 128
#define KEY_TASK_PRIORITY 2
#define KEY_TASK_NAME "key_task"
#define KEY_TASK_PERIOD 20
TaskHandle_t key_task_handle;

// 摇杆任务
void joy_task( void *args );
#define JOY_TASK_STACK_SIZE 128
#define JOY_TASK_PRIORITY 5  // 摇杆优先级高于通讯，保证实时性
#define JOY_TASK_NAME "joy_task"
#define JOY_TASK_PERIOD 20
TaskHandle_t joy_task_handle;

void app_freertos_start(void)
{
    // xTaskCreate(test_task, 
    //     TEST_TASK_NAME, 
    //     TEST_TASK_STACK_SIZE, 
    //     NULL, 
    //     TEST_TASK_PRIORITY, 
    //     &test_task_handle);

    xTaskCreate(power_task, 
        POWER_TASK_NAME, 
        POWER_TASK_STACK_SIZE, 
        NULL, 
        POWER_TASK_PRIORITY, 
        &power_task_handle);

    xTaskCreate(comm_task, 
        COMM_TASK_NAME, 
        COMM_TASK_STACK_SIZE, 
        NULL, 
        COMM_TASK_PRIORITY, 
        &comm_task_handle);

    xTaskCreate(key_task, 
        KEY_TASK_NAME, 
        KEY_TASK_STACK_SIZE, 
        NULL, 
        KEY_TASK_PRIORITY, 
        &key_task_handle);

    xTaskCreate(joy_task, 
        JOY_TASK_NAME, 
        JOY_TASK_STACK_SIZE, 
        NULL, 
        JOY_TASK_PRIORITY, 
        &joy_task_handle);

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

uint8_t comm_buffer[TX_PLOAD_WIDTH] = {0};

void comm_task( void *args )
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        comm_buffer[0] = 'H';
        comm_buffer[1] = 'E';
        comm_buffer[2] = 'L';
        comm_buffer[3] = 'L';
        comm_buffer[4] = 'O';
        
        per_SI24R1_TX_Mode();
        uint8_t res = per_SI24R1_TxPacket(comm_buffer);
        // DEBUG_PRINTF("SEND: %s\r\n", res == 0 ? "SUCCESS" : "FAILED");
        per_SI24R1_RX_Mode();

        // res = per_SI24R1_RxPacket(comm_buffer);
        // DEBUG_PRINTF("RECEIVE: %s\r\n", res == 0 ? "SUCCESS" : "FAILED");

        vTaskDelayUntil(&xLastWakeTime, COMM_TASK_PERIOD);
    }

    // TickType_t xLastWakeTime = xTaskGetTickCount();
    // while (1)
    // {
    //     uint8_t res = per_SI24R1_RxPacket(comm_buffer);
    //     if(res == 0)
    //     {
    //         DEBUG_PRINTF("RECEIVE: %s\r\n", comm_buffer);
    //     }

    //     vTaskDelayUntil(&xLastWakeTime, COMM_TASK_PERIOD);
    // }
}

void key_task( void *args )
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        app_process_key_data();
        vTaskDelayUntil(&xLastWakeTime, KEY_TASK_PERIOD);
    }
}

void joy_task( void *args )
{
    // ADC DMA只需初始化一次，不能在循环中重复调用
    per_joystick_init();

    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        app_process_joystick_data();
        vTaskDelayUntil(&xLastWakeTime, JOY_TASK_PERIOD);
    }
}
