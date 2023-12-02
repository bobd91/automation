#ifndef GPIOPIN_H__
#define GPIOPIN_H__

#include <stdbool.h>
#include "hardware/gpio.h"

enum gpiopin {
    GPIOPIN_0, GPIOPIN_1, GPIOPIN_2, GPIOPIN_3, GPIOPIN_4,
    GPIOPIN_5, GPIOPIN_6, GPIOPIN_7, GPIOPIN_8, GPIOPIN_9,
    GPIOPIN_10, GPIOPIN_11, GPIOPIN_12, GPIOPIN_13, GPIOPIN_14, 
    GPIOPIN_15, GPIOPIN_16, GPIOPIN_17, GPIOPIN_18, GPIOPIN_19,
    GPIOPIN_20, GPIOPIN_21, GPIOPIN_22, 
    GPIOPIN_26 = 26, GPIOPIN_27 = 27, GPIOPIN_28 = 28
};

typedef enum gpiopin gpiopin;
typedef void (*gpiopin_callback)(void);

void gpiopin_set_in_enabled(gpiopin gpiopin, bool enabled);
void gpiopin_set_in(gpiopin gpiopin, gpiopin_callback rise, gpiopin_callback fall);
void gpiopin_set_out(gpiopin gpiopin);
void gpiopin_put(gpiopin gpiopin, bool on);
bool gpiopin_get(gpiopin gpiopin);
bool gpiopin_get_out_level(gpiopin gpiopin);

#endif