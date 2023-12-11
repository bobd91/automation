#ifndef MOCK_PICO_TIME_H__
#define MOCK_PICO_TIME_H__

#include "mock.h"
#include <time.h>

typedef struct mock_repeating_timer repeating_timer_t;

typedef bool (*repeating_timer_callback_t)(repeating_timer_t *rt);

struct mock_repeating_timer {
    repeating_timer_t *next;
    repeating_timer_callback_t callback;
    void *user_data;
    struct timespec run_time;
    int32_t delay_ms;
    int number;
};

bool add_repeating_timer_ms(int32_t delay_ms, repeating_timer_callback_t callback, void *user_data, repeating_timer_t *out);
bool cancel_repeating_timer(repeating_timer_t *timer);
void sleep_ms(uint32_t ms);

uint32_t mock_time_run_pending_timers(void);

#endif