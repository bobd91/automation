#include "gpiopin.h"
#include "hardware/gpio.h"

struct callback_info {
    gpiopin_callback rise;
    gpiopin_callback fall;
    uint32_t event_mask;
};

#define GPIOPIN_MAX GPIOPIN_28

static struct callback_info callback_infos[1 + GPIOPIN_MAX];

static void irq_callback(uint gpio, uint32_t event_mask) {
    struct callback_info *info = &callback_infos[gpio];
    if(info->rise && event_mask & GPIO_IRQ_EDGE_RISE) {
        (*info->rise)();
    } else if (info->fall && event_mask & GPIO_IRQ_EDGE_FALL) {
        (*info->fall)();
    }
}

static void set_gpio_irq_callback(void) {
    static bool done;
    if(done) return;
    
    gpio_set_irq_callback(irq_callback);
    done = true;
}

void gpiopin_set_in_enabled(gpiopin gpiopin, bool enabled) {
    gpio_set_irq_enabled(gpiopin, callback_infos[gpiopin].event_mask, enabled);
}

void gpiopin_set_in(gpiopin gpiopin, gpiopin_callback rise, gpiopin_callback fall) {
    struct callback_info *info=&callback_infos[gpiopin];
    info->rise = rise;
    info->fall = fall;
    info->event_mask = (rise ? GPIO_IRQ_EDGE_RISE : 0) | (fall ? GPIO_IRQ_EDGE_FALL : 0);
    gpio_init(gpiopin);
    gpio_set_dir(gpiopin, GPIO_IN);
}

void gpiopin_set_out(gpiopin gpiopin) {
    gpio_init(gpiopin);
    gpio_set_dir(gpiopin, GPIO_OUT);
}

void gpiopin_put(gpiopin gpiopin, bool on) {
    gpio_put(gpiopin, on);
}

bool gpiopin_get(gpiopin gpiopin) {
    return gpio_get(gpiopin);
}

bool gpiopin_get_out_level(gpiopin gpiopin) {
    return gpio_get_out_level(gpiopin);
}