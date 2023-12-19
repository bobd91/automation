#include "core.h"
#include "pico/async_context.h"
#include "hardware/gpio.h"
#include "lwip/tcp.h"
#include "mock/when.h"
#include <assert.h>

static void pressed_off() {
    assert(mock_gpio_get_out_level(OFF_LED_PIN));
    assert(!mock_gpio_get_out_level(AUTO_LED_PIN));
    assert(!mock_gpio_get_out_level(ON_LED_PIN));

    exit(0);
}

static void running() {
    mock_when_server(TOFF, NULL, pressed_off);
    mock_gpio_set_in_level(OFF_BUTTON_PIN, true);
}

int press_off_when_auto(int argc, char *argv[]) {

    mock_when_init_gpio_sensor();

    mock_when_server(IDFY, RUNN, running);

    core_run();
}
