#include "control_panel.h"
#include "core.h"

// Edit gpio_pins.cmake to change value of pins

void main(void) {
    control_panel_add_off_button(OFF_BUTTON_PIN, OFF_LED_PIN);
    control_panel_add_on_button(ON_BUTTON_PIN, ON_LED_PIN);

    core_run();
}