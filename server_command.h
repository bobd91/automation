#ifndef SERVER_COMMAND_H__
#define SERVER_COMMAND_H__

void server_command_dispatch(char *buf);

// Some server commands have the same values
// In this case some will be server->client and others client->server
// <->  command is sent by client and server
// ->   command is sent by client to server
// <-   command is sent by server to client

#define SERVER_COMMAND_IDENTIFY "IDFY"          // <->
#define SERVER_COMMAND_RUN "RUNN"               // <-
#define SERVER_COMMAND_TURN_OFF "TOFF"          // <-
#define SERVER_COMMAND_TURNED_OFF "TOFF"        // ->
#define SERVER_COMMAND_TURN_ON "TONN"           // <-
#define SERVER_COMMAND_TURNED_ON "TONN"         // ->
#define SERVER_COMMAND_SENSOR_THRESHOLD "STHR"  // <-
#define SERVER_COMMAND_SENSOR_VALUE "SVAL"      // <->
#define SERVER_COMMAND_ERROR "ERRR"             // ->

#endif
