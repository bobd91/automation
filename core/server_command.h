#ifndef SERVER_COMMAND_H__
#define SERVER_COMMAND_H__

void server_command_dispatch(char *buf);

#define SERVER_COMMAND_SIZE 4;
typedef char[1 + SERVER_COMMAND_SIZE] server_command;

// Some server commands have the same values
// In this case some will be server->client and others client->server
// <->  command is sent by client and server
// ->   command is sent by client to server
// <-   command is sent by server to client

const server_command SERVER_COMMAND_IDENTIFY = "IDFY";          // <->
const server_command SERVER_COMMAND_RUN = "RUNN";               // <-
const server_command SERVER_COMMAND_TURN_OFF = "TOFF";          // <-
const server_command SERVER_COMMAND_TURNED_OFF = "TOFF";        // ->
const server_command SERVER_COMMAND_TURN_ON = "TONN";           // <-
const server_command SERVER_COMMAND_TURNED_ON = "TONN";         // ->
const server_command SERVER_COMMAND_SENSOR_THRESHOLD = "STHR";  // <-
const server_command SERVER_COMMAND_SENSOR_VALUE = "SVAL";      // <->
const server_command SERVER_COMMAND_ERROR = "ERRR";             // ->

#endif