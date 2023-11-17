#ifndef _LED_BUTTON_H
#define _LED_BUTTON_H

#include "gpiopin.h"

typedef void (* led_button_event_t)(led_button_t *led_button);

struct led_button_s {
    led_button_t *next;
    gpiopin_t button_pin,
    gpiopin_t led_pin,
    led_button_event_t button_pressed,
    led_button_event_t button_released,
    bool initial_state
};

typedef led_button_t struct led_button_s;

void led_button_add(led_button_t led_button);
void led_button_set(led_button_t *led_button, bool on);
bool led_button_is_on(led_button_t *led_button);
bool led_button_toggle(led_button_t *led_button);
void led_button_on_only(led_button_t *led_button);
void led_button_set_all(bool on);
void led_button_reset_all();
void led_button_blink_only(led_button_t *led_button, uint32_t ms);
void led_button_blink_all(uint32_t ms);
void led_button_blink_loop(uint32_t ms);

#endif