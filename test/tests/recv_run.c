#include "core.h"
#include "mock/when.h"
#include "hardware/gpio.h"
#include <assert.h>

static void running() {
    assert(mock_gpio_irq_enabled(OFF_BUTTON_PIN));
    assert(mock_gpio_irq_enabled(AUTO_BUTTON_PIN));
    assert(mock_gpio_irq_enabled(ON_BUTTON_PIN));
    assert(!mock_gpio_get(OFF_LED_PIN));
    assert(mock_gpio_get(AUTO_LED_PIN));
    assert(!mock_gpio_get(ON_LED_PIN));

    exit(0);
}

int recv_run(int argc, char *argv[]) {

    mock_when_init_gpio_sensor();

    mock_when_server(IDFY, RUNN, running);

    core_run();
}
