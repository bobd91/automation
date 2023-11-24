#include "gpio_sensor.h"
#include "async_event.h"
#include "gpiopin.h"
#include "control_panel.h"


static void sensor_on(void) {
    if(control_panel_is_auto_mode()) {
        async_event_send(ASYNC_EVENT_TURN_ON, NULL);
    }
}

static void sensor_off(void) {
    if(control_panel_is_auto_mode()) {
        async_event_send(ASYNC_EVENT_TURN_OFF, NULL);
    }
} 

void gpio_sensor_setpin(gpiopin pin) {
    gpiopin_set_in(pin, sensor_on, sensor_off);
}