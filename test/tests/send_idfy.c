#include "gpio_sensor.h"
#include "control_panel.h"
#include "core.h"
#include "lwip/tcp.h"
#include "mock/when.h"

static err_t server_received(const char *data) {
    if(!strcmp(data, "IDFY MOCKBOARD1")) {
        exit(0);
    } else {
        exit(1);
    }
}

int send_idfy(int argc, char *argv[]) {

    mock_when_init_gpio_sensor();

    mock_tcp_server_received(server_received);

    core_run();
}