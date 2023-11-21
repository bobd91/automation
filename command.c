#include "command.h"

static command_t *commands = NULL;

static command_t identify = {
    .name = "IDFY",
    .handler = handle_identify
}

static command_t run = {
    .name = "RUNN",
    .handler = handle_run
}

static void handle_unknown(char *buf) {
    error_event("Unknown server command: %s", buf);
}

static void handle_identify(char *buf) {
    async_event(ASYNC_EVENT_IDENTIFY, async_event_copy_arg(buf));
}

static void handle_run(char *buf) {
    async_event(ASYNC_EVENT_RUN, async_event_copy_arg(buf))
}

static char *skip_spaces(char *buf) {
    while(' ' == *buf) buf++;
    return buf;
}

static char *null_if_empty(char *buf) {
    return (buf && buf[0]) ? buf : NULL;
}

void command_init() {
    command_add(identify);
    command_add(run);
}

void command_add(command_t *command) {
    command->next = commands;
    commands = command;
}

void command_dispatch(char *buf) {
    int cmd_size = min(COMMAND_SIZE, strlen(buf));
    command_t command = commands;
    while(command && 0 != memcmp(buf, command->name, cmd_size)) {
        command = command->next;        
    }
    if(command) {
        *command->handler(null_if_empty(skip_spaces(buf + cmd_size)));
    } else {
        handle_unknown(buf);
    }
}