#ifndef MOCK_HARDWARE_GPIO_H__
#define MOCK_HARDWARE_GPIO_H__

#define GPIO_OUT 1
#define GPIO_IN 0

#include "mock/trace.h"

typedef void(*gpio_irq_callback_t)(uint gpio, uint32_t event_mask);

enum  gpio_irq_level { 
    GPIO_IRQ_LEVEL_LOW = 0x1u, 
    GPIO_IRQ_LEVEL_HIGH = 0x2u, 
    GPIO_IRQ_EDGE_FALL = 0x4u, 
    GPIO_IRQ_EDGE_RISE = 0x8u, 
};

void gpio_init (uint gpio);
void gpio_set_dir (uint gpio, bool out);
void gpio_put (uint gpio, bool value);
bool gpio_get (uint gpio);
bool gpio_get_out_level (uint gpio);
void gpio_set_irq_enabled (uint gpio, uint32_t event_mask, bool enabled);
void gpio_set_irq_callback (gpio_irq_callback_t callback);

void mock_gpio_set(uint gpio, bool value);
bool mock_gpio_get(uint gpio);
bool mock_gpio_irq_enabled(uint gpio);

#endif