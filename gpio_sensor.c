#include "gpio_sensor.h"
#include "async_event.h"
#include "gpiopin.h"

static void gpio_sensor_on(void) {
    async_event(ASYNC_EVENT_TURNED_ON, NULL);
}

static void gpio_sensor_off(void) {
    async_event(ASYNC_EVENT_TURNED_OFF, NULL);
} 