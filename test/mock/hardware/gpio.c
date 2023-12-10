#include "hardware/gpio.h"

#define MIN_GPIO 0
#define MAX_GPIO 28

// We ignore the fact that some GPIO numbers in this range are invalid
#define IS_VALID_GPIO(gpio)  ((gpio) >= MIN_GPIO && (gpio) <= MAX_GPIO)

static bool is_init[MAX_GPIO + 1];
static bool is_set_dir[MAX_GPIO + 1];
static bool is_out[MAX_GPIO + 1];
static bool level_value[MAX_GPIO + 1];
static uint32_t mask[MAX_GPIO + 1];

static gpio_irq_callback_t irq_callback;

void gpio_init(uint gpio) {
    assert(IS_VALID_GPIO(gpio));
    assert(!is_init[gpio]);
    MOCK_TRACE("%d", gpio);

    is_init[gpio] = true;
}

void gpio_set_dir(uint gpio, bool out) {
    assert(IS_VALID_GPIO(gpio));
    assert(is_init[gpio]);
    MOCK_TRACE("%d, %d", gpio, out);

    is_set_dir[gpio] = true;
    is_out[gpio] = out;
}

void gpio_put(uint gpio, bool value) {
    assert(IS_VALID_GPIO(gpio));
    assert(is_init[gpio]);
    assert(is_set_dir[gpio]);
    assert(is_out[gpio]);
    MOCK_TRACE("%d, %d", gpio, value);

    level_value[gpio] = value;
}

bool gpio_get(uint gpio) {
    assert(IS_VALID_GPIO(gpio));
    assert(is_init[gpio]);
    assert(is_set_dir[gpio]);
    assert(!is_out[gpio]);
    MOCK_TRACE("%d", gpio);

    return level_value[gpio];
}

bool gpio_get_out_level(uint gpio) {
    assert(IS_VALID_GPIO(gpio));
    assert(is_init[gpio]);
    assert(is_set_dir[gpio]);
    assert(is_out[gpio]);
    MOCK_TRACE("%d", gpio);

    return level_value[gpio];
}

void gpio_set_irq_enabled(uint gpio, uint32_t event_mask, bool enabled) {
    assert(IS_VALID_GPIO(gpio));
    assert(is_init[gpio]);
    assert(is_set_dir[gpio]);
    assert(!is_out[gpio]);
    MOCK_TRACE("%d, %u, %d", gpio, event_mask, enabled);

    mask[gpio] = enabled ? event_mask : 0;
}

void gpio_set_irq_callback(gpio_irq_callback_t callback) {
    MOCK_TRACE("%s", callback ? "callback" : "NULL");
    irq_callback = callback;
}

void mock_gpio_set_in_level(uint gpio, bool value) {
    assert(IS_VALID_GPIO(gpio));
    assert(is_init[gpio]);
    assert(is_set_dir[gpio]);
    assert(!is_out[gpio]);

    bool changed = level_value[gpio] != value;
    uint32_t event_mask = value ? GPIO_IRQ_LEVEL_HIGH : GPIO_IRQ_LEVEL_LOW;

    if(changed) {
        level_value[gpio] = value;
        event_mask |= value ? GPIO_IRQ_EDGE_RISE : GPIO_IRQ_EDGE_FALL;
    }

    if(irq_callback && mask[gpio] & event_mask) {
        (*irq_callback)(gpio, event_mask);
    }

}