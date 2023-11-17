#include "gpio_detector.h"
#include "async_event_monitor.h"
#include "gpiopin.h"

void gpio_detector_init(gpiopin_t pin) {
    gpiopin_set_in(pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, gpio_switched);
}

static void gpio_switched(uint gpio, uint32_t event_mask) {
    if(event_mask & GPIO_IRQ_EDGE_RISE) {
        async_event_monitor(ASYNC_EVENT_GPIO_ON);
    } else if (event_mask & GPIO_IRQ_EDGE_FALL) {
        async_event_monitor(ASYNC_EVENT_GPIO_OFF);
    }
} 