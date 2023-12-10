#include "pico/time.h"

static repeating_timer_t *timers;
static int timer_counter;

static bool remove_timer(repeating_timer_t *timer) 
{
    repeating_timer_t *next = timers;
    repeating_timer_t *prev = NULL;

    while(next && next != timer) {
        prev = next;
        next = next->next;
    }

    if(!next) return false;

    if(prev) {
        prev->next = next->next;
    } else {
        timers = next->next;
    }

    return true;
}

static void set_run_time(repeating_timer_t *timer) { 
    assert(!clock_gettime(CLOCK_MONOTONIC, &timer->run_time));
    long int nsecs = abs(timer->delay_ms) * (long int)1e6 + timer->run_time.tv_nsec;
    if(nsecs > 1e9) {
        timer->run_time.tv_sec++;
        nsecs -= 1e9;
    }
    timer->run_time.tv_nsec = nsecs;
}

static uint32_t until_run_ms(repeating_timer_t *timer) {
    struct timespec now;
    assert(!clock_gettime(CLOCK_MONOTONIC, &now));

    double diff_s = difftime(timer->run_time.tv_sec, now.tv_sec);

    if(diff_s < 0) return 0;

    uint run_ms = (uint)(timer->run_time.tv_nsec / 1e6);
    uint now_ms = (uint)(now.tv_nsec / 1e6);
    int diff_ms = run_ms - now_ms;

    if(diff_s == 0 && diff_ms <= 0) return 0;

    // In all of our cases we expect diff to be small (< 300)
    return (uint32_t)(1000 * diff_s) + diff_ms;
}

static uint32_t run_timer(repeating_timer_t *timer) {
    bool set_run_time_at_start = timer->delay_ms < 0;

    if(set_run_time_at_start) {
        set_run_time(timer);
    }

    (*timer->callback)(timer);

    if(set_run_time_at_start) {
        return until_run_ms(timer);
    } else {
        set_run_time(timer);
        return abs(timer->delay_ms);
    }
}

bool add_repeating_timer_ms(int32_t delay_ms, repeating_timer_callback_t callback, void *user_data, repeating_timer_t *out) {
    assert(callback);
    assert(out);
    out->number = ++timer_counter;
    MOCK_TRACE("%d, callback, data, { .number = %d }", delay_ms, out->number);

    out->delay_ms = delay_ms;
    out->callback = callback;
    out->user_data = user_data;
    set_run_time(out);

    out->next = timers;
    timers = out;

    return true;
}

bool cancel_repeating_timer(repeating_timer_t *timer) {
    assert(timer);
    MOCK_TRACE("{ .number = %d }", timer->number);

    return remove_timer(timer);
}

void sleep_ms(uint32_t ms) {
    MOCK_TRACE("%u", ms);
}

uint32_t mock_time_run_pending_timers(void) {
    repeating_timer_t *timer = timers;
    uint32_t remaining_ms;
    uint32_t min_wait_ms;

    while(timer) {
        remaining_ms = until_run_ms(timer);
        if(!remaining_ms) {
            remaining_ms = run_timer(timer);
        }
        min_wait_ms = (min_wait_ms && min_wait_ms < remaining_ms) ? min_wait_ms : remaining_ms;
        timer = timer->next;
    }

    return min_wait_ms;
}
