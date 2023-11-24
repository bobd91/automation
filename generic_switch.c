#include "generic_switch.h"
#include "async_event.h"

static async_event_listener when_turn_off = default_turn_off;
static async_event_listener when_turn_on = default_turn_on;

static void default_turn_off(void) {
    async_event_send(ASYNC_EVENT_TURNED_OFF, NULL);
}

static void default_turn_on(void) {
    async_event_send(ASYNC_EVENT_TURNED_ON, NULL);
}

void generic_switch_register(async_event_listener turn_off, async_event_listener turn_on) {
    when_turn_off = turn_off;
    when_turn_on = turn_on;
}

void generic_switch_init(void) {
    async_event_add_listener(ASYNC_EVENT_TURN_OFF, when_turn_off);
    async_event_add_listener(ASYNC_EVENT_TURN_ON, when_turn_on);
}