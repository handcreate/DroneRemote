#ifndef __PER_KEY_H
#define __PER_KEY_H

#include "main.h"

typedef enum
{
    KEY_NONE = 0,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_LEFT_X,
    KEY_RIGHT_X,
    KEY_RIGHT_X_LONG
} KeyType;

KeyType per_key_get(void);

#endif /* __PER_KEY_H */
