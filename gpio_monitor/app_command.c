#include "app_command.h"
#include "command.h"


static command_t exec = {
    .name = "EXEC",
    .handler = handle_exec
}


static void handle_exec(char *buf) {
    // Received an exec command, no arguments expected
}

void app_command_init() {
    command_add(exec);
}