#include "gpio_sensor.h"
#include "async_event.h"
#include "gpiopin.h"
#include "control_panel.h"


static void sensor_on(void) {
    async_event_send(ASYNC_EVENT_AUTO_TURN_ON);
}

static void sensor_off(void) {
    async_event_send(ASYNC_EVENT_AUTO_TURN_OFF);
} 

void gpio_sensor_setpin(gpiopin pin) {
    gpiopin_set_in(pin, sensor_on, sensor_off);
}