#include "led_button.h"
#include "gpiopin.h"
#include "pico/time.h"

/*
TODO: gpio mask for setting all led states in one go?
*/
static led_button_t *buttons;
static repeating_timer_t blink_timer;

static void button_callback(uint gpio, uint32_t event_mask) {
    led_button_t *button = find_button(gpio);
    if(event_mask & GPIO_IRQ_EDGE_RISE) {
        if(button->button_pressed) *button->button_pressed(button);
    } else if(event_mask & GPIO_IRQ_EDGE_FALL) {
        if(button->released) *button->button_relased(button);
    }
}

static led_button_t *find_button(uint gpio) {
    led_button_t *button = buttons;
    while(button) {
        if(button->button_pin == gpio) return button;
        button = button->next;
    }
    halt();
}

static void stop_blinking(void) {
    led_button_set_all(false);
    cancel_repeating_timer(&blink_timer);
}

static void start_blinking(repeating_timer_callback_t matcher, led_button_t *led_button, uint32_t ms) {
    led_button_set_all(false);
    halt_if(!add_repeating_timer_ms(ms, matcher, led_button, &blink_timer));
}

static bool toggle_only_button(repeating_timer_t *blink_timer) {
    led_button_t *button = blink_timer->user_data;
    led_button_toggle(button);
    return true;
}

static bool toggle_all_buttons(repeatimg_timer_t *blink_timer) {
    led_button_t *button = buttons;
    while(button) {
        led_button_toggle(button);
        button = button->next;
    }
    return true;
}

static bool toggle_next_button(repeating_timer_t *blink_timer) {
    led_button *button = blink_timer->user_data;
    if(!led_button_toggle(button)) {
        blink_timer->user_data = button->next ? button->next : buttons;
    }
    return true;
} 

static void enable_button(led_button_t button) {
    gpiopin_set_in(led_button->button_pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, button_callback);
}

void led_button_add(led_button_t *led_button) {
    gpiopin_set_out(led_button->led_pin);
    led_button->next = buttons;
    buttons = led_button->next;
}

void led_button_set(led_button_t *led_button, bool on) {
    gpiopin_put(led_button->led_pin, on);
}

void led_button_enable_all() {
    stop_blinking();
    led_button_t *button = buttons;
    while(button) {
        enable_button(button);
        led_button_set(button, button->initial_state);
        button = button->next;
    }
}

bool led_button_is_on(led_button_t *led_button) {
    return gpiopin_get_out_level(led_button->led_pin);
}

bool led_button_toggle(led_button_t *led_button) {
    led_button_set(!led_button_is_on());
}

void led_button_on_only(led_button_t *led_button) {
    stop_blinking();
    led_button_t *button = buttons;
    while(button) {
        led_button_set(button, button == led_button);
        button = button->next;
    }
}

void led_button_set_all(bool on) {
    stop_blinking();
    led_button_t *button = buttons;
    while(button) {
        led_button_set(button, on);
        button = button->next;
    }
}

void led_button_blink_only(led_button_t *button, uint32_t ms) {
    led_button_set_all(false);
    start_blinking(toggle_only_button, button, ms);
}

void led_button_blink_all(uint32_t ms) {
    led_button_set_all(false);
    start_blinking(toggle_all_buttons, buttons, ms);
}

void led_button_blink_loop(uint32_t ms) {
    led_button_set_all(false);
    start_blinking(toggle_next_button, buttons, ms);
}
