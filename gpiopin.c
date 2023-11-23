#include "gpiopin.h"
#include "hardware/gpio.h"

static struct callback_info {
    gpiopin_callback rise,
    gpiopin_callback fall,
    bool enabled
};

static const uint GPIOPIN_MAX = GPIOPIN_28;
static const struct callback_info[1 + GPIOPIN_MAX] callback_infos;

static void set_gpio_irq_callback(void) {
    static bool done;
    if(done) return;
    
    gpio_set_irq_callback(irq_callback);
    done = true;
}

static void irq_callback(uint gpio, uint32_t event_mask) {
    callback_info callback = callback_infos[gpio];
    if(callback.enabled) {
        if(callback.rise && event_mask & GPIO_IRQ_EDGE_RISE) {
            *callback.rise();
        } else if (callback.fall && event_mask & GPIO_IRQ_EDGE_FALL) {
            *callback.fall();
        }
    }
}

void gpiopin_set_in_enabled(gpiopin gpiopin, bool enabled) {
    callback_infos[gpiopin].enabled = enabled;
}

void gpiopin_set_in(gpiopin gpiopin, gpiopin_callback rise, gpiopin_callback_fall) {
    callback_infos[gpiopin].rise = rise;
    callback_infos[gpiopin].fall = fall;
    gpio_init(gpiopin);
    gpio_set_dir(gpiopin, GPIO_IN);
    uint32_t event_mask = (rise ? GPIO_IRQ_EDGE_RISE : 0) & (fall ? GPIO_IRQ_EDGE_FALL : 0);
    gpio_set_irq_enabled(gpiopin, event_mask, true);
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