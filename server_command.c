#include "command.h"


static stuct command {
    const server_command name,
    const async_event_id event_id
};

static typedef struct command command;

static const command commands[] = {
{
    .name = SERVER_COMMAND_IDENTIFY,
    .event_id = ASYNC_EVENT_IDENTIFY
},
{
    .name = SERVER_COMMAND_RUN,
    .event_id = ASYNC_EVENT_RUN
},
{
    .name = SERVER_COMMAND_TURN_OFF,
    .event_id = ASYNC_EVENT_TURN_OFF
},
{
    .name = SERVER_COMMAND_TURN_ON,
    .event_id = ASYNC_EVENT_TURN_ON
},
{
    .name = SERVER_COMMAND_SENSOR_THRESHOLD,
    .event_id = ASYNC_EVENT_SENSOR_THRESHOLD
},
{
    .name = SERVER_COMMAND_SENSOR_VALUE,
    .event_id = ASYNC_EVENT_SENSOR_VALUE
},
{
  // command_dispatch depends upon empty placeholder at end   
}
};


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

void command_dispatch(char *buf) {
    int cmd_size = min(COMMAND_SIZE, strlen(buf));
    command *cmd = commands;
    while(cmd->name && 0 != memcmp(buf, cmd->name, cmd_size)) {
        cmd++;        
    }
    if(cmd->name) {
        char *arg = null_if_empty(skip_spaces(buf + cmd_size));
        async_event_send(cmd->event_id, async_event_copy_arg(arg));
    } else {
        handle_unknown(buf);
    }
}