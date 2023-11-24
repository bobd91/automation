#ifndef _LED_BUTTON_H
#define _LED_BUTTON_H

#include "gpiopin.h"

typedef void (* led_button_event)(led_button_info *led_button);

struct led_button_info {
    gpiopin button_pin,
    gpiopin led_pin,
    led_button_event button_pressed
};

typedef struct led_button_info led_button_info;

#endif