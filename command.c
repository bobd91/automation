#include "command.h"

static command *commands = NULL;

static command identify = {
    .name = "IDFY",
    .event_handler = ASYNC_EVENT_IDENTIFY
};

static command run = {
    .name = "RUNN",
    .event_handler = ASYNC_EVENT_RUN
}

static void handle_unknown(char *buf) {
    error_event("Unknown server command: %s", buf);
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

void command_add(command *cmd) {
    cmd->next = commands;
    commands = cmd;
}

void command_dispatch(char *buf) {
    int cmd_size = min(COMMAND_SIZE, strlen(buf));
    command *cmd = commands;
    while(cmd && 0 != memcmp(buf, cmd->name, cmd_size)) {
        cmd = cmd->next;        
    }
    if(cmd) {
        char *args = null_if_empty(skip_spaces(buf + cmd_size));
        async_event(cmd->event_handler, async_event_copy_arg(arg));
    } else {
        handle_unknown(buf);
    }
}