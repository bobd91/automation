#include "gpio_sensor.h"
#include "control_panel.h"
#include "core.h"
#include "pico/async_context.h"
#include "hardware/gpio.h"
#include "lwip/tcp.h"
#include "mock/trace_match.h"
#include <assert.h>

static void no_work_to_do() {
    assert(mock_gpio_get_irq_enabled(OFF_BUTTON_PIN));
    assert(mock_gpio_get_irq_enabled(AUTO_BUTTON_PIN));
    assert(mock_gpio_get_irq_enabled(ON_BUTTON_PIN));
    assert(!mock_gpio_get_out_level(OFF_LED_PIN));
    assert(mock_gpio_get_out_level(AUTO_LED_PIN));
    assert(!mock_gpio_get_out_level(ON_LED_PIN));

    exit(0);
}

static err_t handle_server_input(const char *data) {
    if(!strcmp(data, "IDFY MOCKBOARD1")) {
        mock_async_context_no_work_function_set(no_work_to_do);
        mock_tcp_server_sent("RUNN");
    }
    return ERR_OK;
}

int recv_run(int argc, char *argv[]) {
    gpio_sensor_setpin(SENSOR_PIN);

    control_panel_add_off_button(OFF_BUTTON_PIN, OFF_LED_PIN);
    control_panel_add_auto_button(AUTO_BUTTON_PIN, AUTO_LED_PIN);
    control_panel_add_on_button(ON_BUTTON_PIN, ON_LED_PIN);

    mock_tcp_set_server_recv_handler(handle_server_input);

    core_run();
}
