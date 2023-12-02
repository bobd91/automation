#include "server_command.h"
#include "async_event.h"
#include <string.h>

#define COMMAND_SIZE 4

struct command {
    char *name;
    async_event_id event_id;
};

typedef struct command command;

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
    .event_id = ASYNC_EVENT_AUTO_TURN_OFF
},
{
    .name = SERVER_COMMAND_TURN_ON,
    .event_id = ASYNC_EVENT_AUTO_TURN_ON
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

static char *skip_spaces(char *buf) {
    while(' ' == *buf) buf++;
    return buf;
}

static char *null_if_empty(char *buf) {
    return (buf && buf[0]) ? buf : NULL;
}

void command_dispatch(char *buf) {
    const command *cmd = NULL;

    if(strlen(buf) >= COMMAND_SIZE) {
        cmd = commands;
        while(cmd->name && 0 != memcmp(buf, cmd->name, COMMAND_SIZE)) {
            cmd++;        
        }
    }

    if(cmd && cmd->name) {
        char *arg = null_if_empty(skip_spaces(buf + COMMAND_SIZE));
        async_event_send_arg(cmd->event_id, async_event_format_arg(arg));
    } else {
        async_event_send_arg(ASYNC_EVENT_SERVER_ERROR, async_event_format_arg("Unrecognized command: %s", buf));
    }
}