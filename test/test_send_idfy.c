#include "gpio_sensor.h"
#include "control_panel.h"
#include "core.h"
#include "mock.h"
#include "lwip/tcp.h"

static err_t handle_server_input(const char *data) {
    if(!strcmp(data, "IDFY MOCKBOARD1")) {
        printf("Test Passed, exit(0)\n");
        fflush(stdout);
        exit(0);
    } else {
        printf("Received: \"%s\"\n", data);
        printf("Test Failed, exit(1)\n");
        fflush(stdout);
        exit(1);
    }
}

int test_send_idfy(int argc, char *argv[]) {
    gpio_sensor_setpin(SENSOR_PIN);

    control_panel_add_off_button(OFF_BUTTON_PIN, OFF_LED_PIN);
    control_panel_add_auto_button(AUTO_BUTTON_PIN, AUTO_LED_PIN);
    control_panel_add_on_button(ON_BUTTON_PIN, ON_LED_PIN);

    mock_tcp_set_server_recv_handler(handle_server_input);

    core_run();
}