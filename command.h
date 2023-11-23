#ifndef _COMMAND_H
#define _COMMAND_H

#include "async_event.h"

#define COMMAND_SIZE 4;

stuct command {
    command *next,
    char[1 + COMMAND_SIZE] name,
    async_event_handler event_handler
};

typedef struct command command;

void command_init(void);
void command_add(command *cmd);
void command_dispatch(char *buf);

#endif