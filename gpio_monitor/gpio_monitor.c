#include "gpio_monitor.h"

#include "led_button.h"
#include "async_event.h"

void main(void) {
    gpio_sensor_setpin(SENSOR_PIN);

    led_button_on_setpins(ON_BUTTON_PIN, ON_LED_PIN);
    led_button_auto_setpins(AUTO_BUTTON_PIN, AUTO_LED_PIN);
    led_button_off_setpins(OFF_BUTTON_PIN, OFF_LED_PIN);

    led_button_auto_callback(gpio_sensor_enable);

    async_event(ASYNC_EVENT_START, NULL);
}