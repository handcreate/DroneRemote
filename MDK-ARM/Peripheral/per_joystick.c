#include "per_joystick.h"

uint16_t adc_buff[4] = {0};

void per_joystick_init(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buff, 4);
}

void per_joystick_get(JoystickStruct *js)
{
    js->thr = adc_buff[1];
    js->yaw = adc_buff[0];
    js->pit = adc_buff[3];
    js->rol = adc_buff[2];
}
