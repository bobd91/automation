#include "gpiopin.h"
#include "hardware/gpio.h"

static const uint GPIOPIN_MAX = GPIOPIN_28;
static const gpio_irq_callback_t[1 + GPIOPIN_MAX] gpiopin_callbacks;

static void set_gpio_irq_callback(void) {
    static bool done;
    if(done) return;
    
    gpio_set_irq_callback(call_callback);
    done = true;
}

static void set_callback(gpiopin_t gpiopin, gpio_irq_callback_t callback) {
    set_gpio_irq_callback();
    gpiopin_callbacks[gpiopin] = callback;
}

static void call_callback(uint gpio, uint32_t event_mask) {
    gpio_irq_callback_t callback = gpiopin_callbacks[gpio];
    if(callback) *callback(gpio, event_mask);
}

void gpiopin_set_in(gpiopin_t gpiopin, uint32_t event_mask, gpio_irq_callback_t callback) {
    set_callback(gpiopin, callback);
    gpio_init(gpiopin);
    gpio_set_dir(gpiopin, GPIO_IN);
    gpio_set_irq_enabled(gpiopin, event_mask, true);
}

void gpiopin_set_out(gpiopin_t gpiopin) {
    gpio_init(gpiopin);
    gpio_set_dir(gpiopin, GPIO_OUT);
}

void gpiopin_put(gpiopin_t gpiopin, bool on) {
    gpio_put(gpiopin, on);
}

bool gpiopin_get(gpiopin_t gpiopin) {
    return gpio_get(gpiopin);
}

bool gpiopin_get_out_level(gpiopin_t gpiopin) {
    return gpio_get_out_level(gpiopin);
}