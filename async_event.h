#ifndef _ASYNC_EVENT_H
#define _ASYNC_EVENT_H


typedef void (* async_event_handler)(char *arg);

async_event_handler ASYNC_EVENT_START = async_event_start;
async_event_handler ASYNC_EVENT_CONNECTED = async_event_connected;
async_event_handler ASYNC_EVENT_IDENTIFY = async_event_identify;
async_event_handler ASYNC_EVENT_RUN = async_event_run;
async_event_handler ASYNC_EVENT_TURNED_OFF = async_event_turned_off;
async_event_handler ASYNC_EVENT_TURNED_ON = async_event_turned_on;
async_event_handler ASYNC_EVENT_ERROR = async_event_error;

void async_event(async_event_handler handler, char *arg);

#endif