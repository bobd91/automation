#ifndef ERROR_EVENT_H__
#define ERROR_EVENT_H__

enum error_event_id {
  ERROR_EVENT_ASYNC_POLL_INIT = 1,
  ERROR_EVENT_ASYNC_ADD_WORKER,
  ERROR_EVENT_ASYNC_QUEUE_FULL,
  ERROR_EVENT_NO_MEMORY,
  ERROR_EVENT_ADD_TIMER,
  ERROR_EVENT_BUFFER_OVERFLOW,
  ERROR_EVENT_BUFFER_COPY,
  ERROR_EVENT_IP_ADDRESS,
  ERROR_EVENT_IP_PORT,
  ERROR_EVENT_TCP_CONNECT,
  ERROR_EVENT_TCP_WRITE,
  ERROR_EVENT_TCP_OUTPUT,
  ERROR_EVENT_WIFI_INIT,
  ERROR_EVENT_WIFI_CONNECT,
  ERROR_EVENT_TCP_ERROR,
  ERROR_EVENT_TCP_LINK,
  ERROR_EVENT_MAX_LISTENERS
}

struct error_event_info {
  error_event_id event_id,
  int err,
  char *file,
  int line
};

typedef enum error_event_id error_event_id;
typedef struct error_event_info error_event_info;
typedef bool (* error_event_listener)(error_event_info *event_info);

#define error_if(test, ret, id, err) \
do { \
  if(test) { \
    error_event_log((id), (extra), __FILE__, __LINE__); \
    return ret; \
  } \
} while(0)

void error_event_async_mode();
void error_event_handle();
void error_event_call(error_event_id event_id, int err, char *file, int line);
bool error_event_listen(error_event_listener listener);

#endif