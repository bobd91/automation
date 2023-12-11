#ifndef CONTROL_PANEL_H__
#define CONTROL_PANEL_H__

#include "gpiopin.h"

void control_panel_init(void);
void control_panel_add_off_button(gpiopin button_pin, gpiopin led_pin);
void control_panel_add_auto_button(gpiopin button_pin, gpiopin led_pin);
void control_panel_add_on_button(gpiopin button_pin, gpiopin led_pin);

#endif