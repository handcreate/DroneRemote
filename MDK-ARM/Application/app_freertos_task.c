#include "app_freertos_task.h"
#include "com_debug.h"

void test_task( void *pvParameters );
#define TEST_TASK_STACK_SIZE 128
#define TEST_TASK_PRIORITY 1
#define TEST_TASK_NAME "test_task"
#define TEST_TASK_PERIOD 1000

void app_freertos_start(void)
{
    xTaskCreate(test_task, "test_task", TEST_TASK_STACK_SIZE, NULL, TEST_TASK_PRIORITY, NULL);
    vTaskStartScheduler();
}

void test_task( void *pvParameters )
{
    while (1)
    {
        DEBUG_PRINTF("test_task running\n");
        vTaskDelay(TEST_TASK_PERIOD);
    }
}
