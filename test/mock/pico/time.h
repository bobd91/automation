#ifndef MOCK_PICO_TIME_H__
#define MOCK_PICO_TIME_H__

typedef struct repeating_timer repeating_timer_t;

typedef bool (*repeating_timer_callback_t)(repeating_timer_t *rt);

struct repeating_timer {
    void *user_data;
};

bool add_repeating_timer_ms(int32_t delay_ms, repeating_timer_callback_t callback, void *user_data, repeating_timer_t *out);
void sleep_ms(uint32_t ms);

#endif