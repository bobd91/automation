#include "control_buttons.h"
#include "app_monitor.h"

#include "led_button.h"
#include "gpiopin.h"

static led_button_t off_button = { 
    .button_pin = GPIOPIN_10,
    .led_pin = GPIOPIN_11,
    .button_pressed = event_off
};

static led_button_t auto_button = {
    .button_pin = GPIOPIN_12,
    .led_pin = GPIOPIN_13,
    .button_pressed = event_auto
};

static led_button_t on_button = {
    .button_pin = GPIOPIN_14,
    .led_pin = GPIOPIN_15,
    .button_pressed = event_on
};

void control_buttons_init(void) {
    led_button_add(&off_button);
    led_button_add(&auto_button);
    led_button_add(&on_button);
}