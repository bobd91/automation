#include "mock/when.h"
#include "lwip/tcp.h"
#include "gpio_sensor.h"
#include "control_panel.h"
#include "pico/async_context.h"
#include "hardware/gpio.h"

char *await_command;
char *await_reply;
mock_async_context_no_work_function await_complete;

static err_t await_server(const char *data) {
    if(!strcmp(data, await_command)) {
        mock_async_context_no_work_function_set(await_complete);
        if(await_reply) mock_tcp_server_sent(await_reply);
    }
    return ERR_OK;
}

static void await_failed(void) {
    printf("AWAIT_FAILED\n");
    exit(1);
}

void mock_when_init_gpio_sensor(void) {
    gpio_sensor_setpin(SENSOR_PIN);

    control_panel_add_off_button(OFF_BUTTON_PIN, OFF_LED_PIN);
    control_panel_add_auto_button(AUTO_BUTTON_PIN, AUTO_LED_PIN);
    control_panel_add_on_button(ON_BUTTON_PIN, ON_LED_PIN);
}

void mock_when_server(char *command, char *reply, mock_async_context_no_work_function complete) {
    await_command = command;
    await_reply = reply;
    await_complete = complete;
    mock_tcp_set_server_recv_handler(await_server);

    // Horrible hack, this only works once up and running
    if(strcmp(command, IDFY)) {
        mock_async_context_no_work_function_set(await_failed);
    }
}
