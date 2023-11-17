#ifndef _ASYNC_EVENT_MONITOR_H
#define _ASYNC_EVENT_MONITOR_H

typedef void (* app_event_monitor_t)(int event);

void async_event_monitor_init(app_event_monitor_t monitor);
void async_event_monitor(void *event);

#endif