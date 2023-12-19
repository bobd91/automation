#ifndef MOCK_WHEN_H__
#define MOCK_WHEN_H__

#include "pico/async_context.h"

#define IDFY "IDFY MOCKBOARD1"
#define RUNN "RUNN"
#define TOFF "TOFF"
#define TONN "TONN"

void mock_when_init_gpio_sensor(void);
void mock_when_server(char *command, char *reply, mock_async_context_no_work_function complete);

#endif
