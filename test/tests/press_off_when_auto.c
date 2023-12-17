#include "gpio_sensor.h"
#include "control_panel.h"
#include "core.h"
#include "pico/async_context.h"
#include "hardware/gpio.h"
#include "lwip/tcp.h"
#include "mock/trace_match.h"
#include <assert.h>

static bool turned_off;

static void pressed_off() {
    assert(mock_gpio_get_out_level(OFF_LED_PIN));
    assert(!mock_gpio_get_out_level(AUTO_LED_PIN));
    assert(!mock_gpio_get_out_level(ON_LED_PIN));

    assert(turned_off);

    exit(0);
}

static err_t await_turned_off(const char *data) {
    if(!strcmp(data, "TOFF")) {
        turned_off = true;
    }
    return ERR_OK;
}

static void running() {
    mock_async_context_no_work_function_set(pressed_off);
    mock_tcp_set_server_recv_handler(await_turned_off);
    mock_gpio_set_in_level(OFF_BUTTON_PIN, true);
}

static err_t await_idfy(const char *data) {
    if(!strcmp(data, "IDFY MOCKBOARD1")) {
        mock_async_context_no_work_function_set(running);
        mock_tcp_server_sent("RUNN");
    }
    return ERR_OK;
}

int press_off_when_auto(int argc, char *argv[]) {
    gpio_sensor_setpin(SENSOR_PIN);

    control_panel_add_off_button(OFF_BUTTON_PIN, OFF_LED_PIN);
    control_panel_add_auto_button(AUTO_BUTTON_PIN, AUTO_LED_PIN);
    control_panel_add_on_button(ON_BUTTON_PIN, ON_LED_PIN);

    mock_tcp_set_server_recv_handler(await_idfy);

    core_run();
}
