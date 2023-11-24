#include "gpio_switch.h"
#include "switch_action.h"
#include "gpiopin.h"
#include "async_event.h"

static gpiopin switch_pin;

static void turn_off(void) {
    gpiopin_put(switch_pin, false);
    async_event_send(ASYNC_EVENT_TURNED_OFF);
}

static void turn_on(void) {
    gpiopin_put(switch_pin, true);
    async_event_send(ASYNC_EVENT_TURNED_ON);
}

void gpio_switch_setpin(gpiopin pin) {
    switch_pin = pin;
    gpiopin_set_out(pin);
    switch_action_register(turn_off, turn_on);
}