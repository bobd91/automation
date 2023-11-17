
#ifndef _GPIOPIN_H
#define _GPIOPIN_H

#inlcude "hardware/gpio.h"

enum gpiopin_e {
    GPIOPIN_0, GPIOPIN_1, GPIOPIN_2, GPIOPIN_3, GPIOPIN_4,
    GPIOPIN_5, GPIOPIN_6, GPIOPIN_7, GPIOPIN_8, GPIOPIN_9,
    GPIOPIN_10, GPIOPIN_11, GPIOPIN_12, GPIOPIN_13, GPIOPIN_14, 
    GPIOPIN_15, GPIOPIN_16, GPIOPIN_17, GPIOPIN_18, GPIOPIN_19,
    GPIOPIN_20, GPIOPIN_21, GPIOPIN_22, 
    GPIOPIN_26 = 26, GPIOPIN_27 = 27, GPIOPIN_28 = 28
};

typedef enum gpiopin_e gpiopin_t;

void gpiopin_set_in(gpiopin_t gpiopin, uint32_t event_mask, gpio_irq_callback_t callback);
void gpiopin_set_out(gpiopint gpiopin);
void gpiopin_put(gpiopin_t gpiopin, bool on);
bool gpiopin_get(gpiopin_t gpiopin);
bool gpiopin_get_out_level(gpiopin_t gpiopin);

#endif