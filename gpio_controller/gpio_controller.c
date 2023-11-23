#include "gpio_controller.h"

#include "led_button.h"
#include "async_event.h"

static command toff {
    .name = "TOFF",
    .event_handler = async_turn_off
};

static command tonn {
    .name = "TONN",
    .event_handler = async_turn_on
};

void main(void) {
    gpio_switch_setpin(SWITCH_PIN);

    led_button_on_setpins(ON_BUTTON_PIN, ON_LED_PIN);
    led_button_auto_setpins(AUTO_BUTTON_PIN, AUTO_LED_PIN);
    led_button_off_setpins(OFF_BUTTON_PIN, OFF_LED_PIN);

    command_add(toff);
    command_add(tonn);

    async_event(ASYNC_EVENT_START, NULL);
}

static void async_turn_off(char *arg) {
    if(led_button_auto_mode()) {
        gpio_switch_turn_off();
    }
}

static void async_turn_on(char *arg) {
    if(led_button_auto_mode()) {
        gpio_switch_turn_on();
    }
}