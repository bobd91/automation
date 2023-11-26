#include "gpio_monitor.h"

#include "control_panel.h"
#include "app.h"

void main(void) {
    gpio_sensor_setpin(SENSOR_PIN);

    control_panel_add_off_button(OFF_BUTTON_PIN, OFF_LED_PIN);
    control_panel_add_auto_button(AUTO_BUTTON_PIN, AUTO_LED_PIN);
    control_panel_add_on_button(ON_BUTTON_PIN, ON_LED_PIN);

    app_run();
}