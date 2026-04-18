#include "com_utils.h"

int16_t clamp(int16_t val, int16_t min, int16_t max)
{
    if (val < min) {
        return min;
    } else if (val > max) {
        return max;
    } else {
        return val;
    }
}
