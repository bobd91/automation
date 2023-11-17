#ifndef _COMMAND_H
#define _COMMAND_H

#define COMMAND_SIZE 4;

stuct command_s {
    command_t *next,
    char[1 + COMMAND_SIZE] name,
    command_handler_t handler
};

typedef command_t struct command_s;
typedef void (* command_handler_t)(char *buf);

void command_init(void);
void command_dispatch(char *buf);
void command_add(command_t *command);

#endif