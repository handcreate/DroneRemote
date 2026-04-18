#ifndef __PER_JOYSTICK_H
#define __PER_JOYSTICK_H

#include "adc.h"

typedef struct
{
    int16_t thr;
    int16_t yaw;
    int16_t pit;
    int16_t rol;
} JoystickStruct;

void per_joystick_init(void);

void per_joystick_get(JoystickStruct *js);

#endif /* __PER_JOYSTICK_H */
