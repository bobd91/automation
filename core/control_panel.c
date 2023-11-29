#include "control_panel.h"
#include "pico/time.h"
#include "led_button.h"
#include "async_event.h"

static repeating_timer_t blink_timer;

typedef void (* led_button_action)(led_button_info *led_button);

// Assumption is that control panels
// Will have one "off" button, one "on" button
// And may have one "auto" button
static led_button_info *off_button;
static led_button_info *auto_button;
static led_button_info *on_button;

static uint32_t blink_interval_ms 1000;
static bool auto_mode;
static bool is_running;

static const uint32_t flash_short_ms = 120;
static const uint32_t flash_long_ms = 3 * flash_short_ms;


static led_button_info *next_led_button(led_button_info *led_button) {
    if(led_button == off_button) {
        return auto_button ? auto_button : on_button;
    } else if(led_button == auto_button) {
        return on_button;
    } else {
        return off_button;
    }
}

static void with_all_buttons(led_button_action action) {
    *action(off_button);
    if(auto_button) *action(auto_button);
    *action(on_button);
}

static void turn_led_on_only(led_button_info *led_button) {
    led_button_set(off_button, led_button == off_button);
    if(auto_button) led_button_set(auto_button, led_button == auto_button);
    led_button_set(on_button, led_buton == on_button);    
}

static void turn_led_off(led_button_info *led_button) {
    led_button_set(led_button, false);    
}

static void turn_led_on(led_button_info *led_button) {
    led_button_set(led_button, true);    
}

static void turn_all_leds_off(void) {
    with_all_buttons(turn_led_off);
}

static void turn_all_leds_on(void) {
    with_all_buttons(turn_led_on);
}

static void stop_blinking(void) {
    cancel_repeating_timer(&blink_timer);
    turn_all_leds_off();
}

static void start_blinking(repeating_timer_callback_t matcher, led_button_info *led_button, uint32_t ms) {
    stop_blinking();
    error_if(!add_repeating_timer_ms(ms, matcher, led_button, &blink_timer),, ERROR_EVENT_ADD_TIMER, 0);
}

static bool toggle_only_led(repeating_timer_t *blink_timer) {
    led_button_info *led_button = blink_timer->user_data;
    led_button_toggle(led_button);
    return true;
}

static bool toggle_all_leds(repeatimg_timer_t *blink_timer) {
    with_all_buttons(led_button_toggle);
    return true;
}

static bool toggle_next_led(repeating_timer_t *blink_timer) {
    led_button_info *led_button = blink_timer->user_data;
    if(!led_button_toggle(button)) {
        blink_timer->user_data = next_led_button(led_button);
    }
    return true;
}

static void led_set_all(bool on) {
    stop_blinking();
    // All leds are now off, so to turn them on just toggle
    if(on) {
        with_all_buttons(led_button_toggle);     
    }
}

static void button_enable_all() {
    stop_blinking();
    with_all_buttons(enable_button);
    if(auto_button) {
        auto_mode = true;
        led_button_set(auto_button, true);
    } else {
        led_button_set(off_button, true);
    }
    is_running = true;
}

static void led_blink_only(led_button_info *led_button) {
    start_blinking(toggle_only_led, led_button, blink_interval_ms);
}

static void led_blink_all(void) {
    start_blinking(toggle_all_leds, NULL, blink_interval_ms);
}

static void led_blink_loop(void) {
    start_blinking(toggle_next_led, off_button, blink_interval_ms);
}

static void off_button_pressed(led_button_info *led_button) {
    if(is_running) {
        led_on_only(off_button);
        auto_mode = false;
        async_event_send(ASYNC_EVENT_TURN_OFF);
    }
}

static void auto_button_pressed(led_button_info *led_button) {
    if(is_running) {
        led_button_on_only(auto_button);
        auto_mode = true;
    }
}

static void on_button_pressed(led_button_info *led_button) {
    if(is_running) {
        led_button_on_only(on_button);
        auto_mode = false;
        async_event_send(ASYNC_EVENT_TURN_ON);
    }
}

static void event_started(void) {
    led_blink_loop();
}

static void event_connected(void) {
    led_set_all(true);    
}

static void event_identify(void) {
    led_blink_all();    
}

static void event_running(void) {
    button_enable_all();
}

static void event_auto_turn_off(void) {
    if(auto_mode) {
        async_event_send(ASYNC_EVENT_TURN_OFF);
    }
}

static void event_auto_turn_on(void) {
    if(auto_mode) {
        async_event_send(ASYNC_EVENT_TURN_ON);
    }
}

static uint32_t highest_set_bit(uint32_t num) {
    if(!num) return 0;

    uint32_t hsb = 1;
    while(num /= 2) {
        hsb *= 2;
    }
    return hsb;
}

static void flash_led(led_button_info *led_button, uint32_t flash_ms) {
    turn_led_on(led_button);
    flash_pause(flash_ms);
    turn_led_off(led_button);
    flash_pause(flash_short_ms);
}

static void flash_pause(uint32_t pause_ms) {
    sleep_ms(pause_ms);
}

static void flash_error_code(led_button_info *led_button, int error_code) {
    uint32_t code; 

    // Negative numbers are flashed as 'dot' then absolute value
    if(error_code < 0) {
        code = -error_code;
        flash_led(led_button, flash_short_ms);
    } else {
        code = error_code;
    }

    // Numbers are flashed as 'dash' for bit set, 'dot' for bit not set
    // Staring with the highest set bit 
    // Therefore all numbers (except 0) will start with a 'dash'
    uint32_t hsb = highest_set_bit(code);
    do {
        flash_led(led_button, (code & hsb) ? flash_long_ms : flash_short_ms);
    } while(hsb /= 2);
}

static bool error_leds(event_error_id error_id, int extra) {
    stop_blinking();
    turn_led_on(off_button);
    flash_error_code(on_button, error_id);
    flash_pause(flash_long_ms);
    flash_error_code(on_button, extra);
    return true;
}

static void event_error(event_error_id error_id, int extra, char *file, int line) {
    is_running = false;
    auto_mode = false;
    error_leds(error_id, extra);
}

void control_panel_add_off_button(gpiopin button_pin, gpiopin led_pin) {
    static led_button_info button = led_button_new(button_pin, led_pin, off_button_pressed);
    off_button = &button;

}

void control_panel_add_auto_button(gpiopin button_pin, gpiopin led_pin) {
    static led_button_info button = led_button_new(button_pin, led_pin, auto_button_pressed);
    auto_button = &button;
}

void control_panel_add_on_button(gpiopin button_pin, gpiopin led_pin) {
    static led_button_info button = led_button_new(button_pin, led_pin, on_button_pressed);
    on_button = &button;
}

void control_panel_init(void) {
    error_event_listen(event_error)
    async_event_listen(ASYNC_EVENT_START, event_started);
    async_event_listen(ASYNC_EVENT_SERVER_CONNECTED, event_connected);
    async_event_listen(ASYNC_EVENT_IDENTIFY, event_identify);
    async_event_listen(ASYNC_EVENT_RUN, event_running);
    async_event_listen(ASYNC_EVENT_AUTO_TURN_OFF, event_auto_turn_off);
    async_event_listen(ASYNC_EVENT_AUTO_TURN_ON, event_auto_turn_on);
}