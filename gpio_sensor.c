#include "gpio_sensor.h"
#include "async_event.h"
#include "gpiopin.h"

void gpio_sensor_init(gpiopin_t pin) {
    gpiopin_set_in(pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, gpio_switched);
}

static void gpio_switched(uint gpio, uint32_t event_mask) {
    if(event_mask & GPIO_IRQ_EDGE_RISE) {
        async_event(ASYNC_EVENT_TURNED_ON, NULL);
    } else if (event_mask & GPIO_IRQ_EDGE_FALL) {
        async_event(ASYNC_EVENT_TURNED_OFF, NULL);
    }
} 