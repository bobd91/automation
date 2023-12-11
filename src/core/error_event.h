#ifndef ERROR_EVENT_H__
#define ERROR_EVENT_H__

#include <stdbool.h>

#ifndef ERROR_EVENT_SLEEP_MS 
#define ERROR_EVENT_SLEEP_MS 5000
#endif

#ifndef ERROR_EVENT_REBOOT_MS
#define ERROR_EVENT_REBOOT_MS (60 * ERROR_EVENT_SLEEP_MS)
#endif

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
};

typedef enum error_event_id error_event_id;

struct error_event_info {
  enum error_event_id event_id;
  int err;
  char *file;
  int line;
};

typedef struct error_event_info error_event_info;
typedef bool (*error_event_listener)(error_event_info *event_info);

#define error_if(test, ret, id, err) \
do { \
  if(test) { \
    error_event_log((id), (err), __FILE__, __LINE__); \
    return ret; \
  } \
} while(0)

void error_event_async_init();
void error_event_async_check();
void error_event_log(error_event_id event_id, int err, char *file, int line);
void error_event_listen(error_event_listener listener);

#endif