#include "command.h"

static command_t *commands = NULL;

static command_t wait = {
    .name = "WAIT",
    .handler = handle_wait
}

static command_t identify = {
    .name = "IDFY",
    .handler = handle_identify
}

static command_t exec = {
    .name = "EXEC",
    .handler = handle_exec
}

static void handle_unknown(char *buf) {
    // Received a command we didn't expect
}

static void handle_wait(char *buf) {
    // Received a wait command, no arguments expected
}

static void handle_identify(char *buf) {
    // Received an identify command, no arguments expected
}

static void handle_exec(char *buf) {
    // Received an exec command, no arguments expected
}

static char *skip_spaces(char *buf) {
    while(' ' == *buf) buf++;
    return buf;
}

void command_init() {
    command_add(wait);
    command_add(identify);
    command_add(exec);
}

void command_add(command_t *command) {
    command->next = commands;
    commands = command;
}

void command_dispatch(char *buf) {
    command_t command = commands;
    while(command && 0 != memcmp(buf, command->name, COMMAND_SIZE)) {
        command = command->next;        
    }
    if(command) {
        *command->handler(skip_spaces(buf + COMMAND_SIZE));
    } else {
        handle_unknown(buf);
    }
}