#include "led_button.h"
#include "gpiopin.h"

static bool led_button_is_on(led_button_info *led_button) {
    return gpiopin_get_out_level(led_button->led_pin);
}

void enable_button(led_button_info *led_button) {
    gpiopin_set_in_enabled(led_button->button_pin, true);
}

void led_button_set(led_button_info *led_button, bool on) {
    gpiopin_put(led_button->led_pin, on);
}

bool led_button_toggle(led_button_info *led_button) {
    bool new_state = !led_button_is_on(led_button);
    led_button_set(led_button, new_state);
    return new_state;
}

led_button_info led_button_new(gpiopin button_pin, gpiopin led_pin, gpiopin_callback button_pressed) {
    led_button_info info = {
        .button_pin = button_pin,
        .led_pin = led_pin,
        .button_pressed = button_pressed
    };

    gpiopin_set_in(button_pin, button_pressed, NULL);
    gpiopin_set_out(led_pin);

    return info;
}
