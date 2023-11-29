#ifndef LEDBUTTON_H__
#define LEDBUTTON_H__

#include "gpiopin.h"

typedef void (* led_button_event)(led_button_info *led_button);

struct led_button_info;

typedef struct led_button_info led_button_info;

void enable_button(led_button_info *led_button);
void led_button_set(led_button_info *led_button, bool on);
bool led_button_toggle(led_button_info *led_button);
led_button_info *led_button_new(gpiopin button_pin, gpiopin led_pin, led_button_event button_pressed);

#endif