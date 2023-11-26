#ifndef _ASYNC_EVENT_H
#define _ASYNC_EVENT_H

enum async_event_id {
    ASYNC_EVENT_START,
    ASYNC_EVENT_WIFI_CONNECTED,
    ASYNC_EVENT_SERVER_CONNECTED,
    ASYNC_EVENT_IDENTIFY,
    ASYNC_EVENT_RUN,
    ASYNC_EVENT_TURN_OFF,
    ASYNC_EVENT_AUTO_TURN_OFF,
    ASYNC_EVENT_TURNED_OFF,
    ASYNC_EVENT_TURN_ON,
    ASYNC_EVENT_AUTO_TURN_ON,
    ASYNC_EVENT_TURNED_ON,
    ASYNC_EVENT_SENSOR_THRESHOLD,
    ASYNC_EVENT_SENSOR_VALUE,
    ASYNC_EVENT_MAX_ID // Placeholder, ensure that this is always last
}

typedef enum async_event_id async_event_id;
typedef void (* async_event_listener)(void);
typedef void (* async_event_listener_arg)(char *arg);

void async_event_send(async_event_id event_id);
void async_event_send_arg(async_event_id event_id, char *arg);
void async_event_listen(async_event_id event_id, async_event_listener listener);
void async_event_listen_arg(async_event_id event_id, async_event_listen_arg listener))

#endif