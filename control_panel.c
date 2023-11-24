
static repeating_timer_t blink_timer;

typedef void (* led_button_action)(led_button_info *led_button);

// Assumption is that control panels
// Will have one off button, one on button
// And may have one auto button
static led_button_info *off_button;
static led_button_info *auto_button;
static led_button_info *on_button;

static bool auto_mode;
static bool is_running;

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
    if(off_button)  *action(off_button);
    if(auto_button) *action(auto_button);
    if(on_button)   *action(on_button);
}

static void turn_led_off(led_button_info *led_button) {
    led_button_set(led_button, false);    
}

static void toggle_led(led_button_info *led_button) {
    led_button_toggle(led_button);
}

static void turn_all_leds_off(void) {
    with_all_buttons(turn_led_off);
}

static void stop_blinking(void) {
    cancel_repeating_timer(&blink_timer);
    turn_all_leds_off();
}

static void start_blinking(repeating_timer_callback_t matcher, led_button_info *led_button, uint32_t ms) {
    turn_all_leds_off();
    error_if(!add_repeating_timer_ms(ms, matcher, led_button, &blink_timer),,"Failed to add reeating timer");
}

static bool toggle_only_led(repeating_timer_t *blink_timer) {
    led_button_info *led_button = blink_timer->user_data;
    led_button_toggle(led_button);
    return true;
}

static bool toggle_all_leds(repeatimg_timer_t *blink_timer) {
    with_all_buttons(toggle_led);
    return true;
}

static bool toggle_next_button(repeating_timer_t *blink_timer) {
    led_button_info *led_button = blink_timer->user_data;
    if(!led_button_toggle(button)) {
        blink_timer->user_data = next_led_button(led_button);
    }
    return true;
}

static void led_button_set_all(bool on) {
    stop_blinking();
    // All leds are now off, so to turn them on just toggle
    if(on) {
        with_all_buttons(toggle_led);     
    }
}

static void led_button_enable_all() {
    stop_blinking();
    with_all_buttons(enable_button);
    if(auto_button) {
        led_button_set(auto_button, true);
    } else {
        led_button_set(off_button, true);
    }
}

static void led_button_blink_only(led_button_info *led_button, uint32_t ms) {
    start_blinking(toggle_only_button, led_button, ms);
}

static void led_button_blink_all(uint32_t ms) {
    start_blinking(toggle_all_buttons, NULL, ms);
}

static void led_button_blink_loop(uint32_t ms) {
    start_blinking(toggle_next_button, off_button, ms);
}

static void off_button_pressed(led_button_info *led_button) {
    auto_mode = false;
    if(is_running) {
        async_event_send(ASYNC_EVENT_TURN_OFF, NULL);
    }
}

static void auto_button_pressed(led_button_info *led_button) {
    if(is_running) {
        auto_mode = true;
    }
}

static void on_button_pressed(led_button_info *led_button) {
    auto_mode = false;
    if(is_running) {
        async_event_send(ASYNC_EVENT_TURN_ON, NULL);
    }
}

void control_panel_add_off_button(gpiopin button_pin, gpiopin led_pin) {
    off_button = led_button_new(button_pin, led_pin, off_button_pressed);
}

void control_panel_add_auto_button(gpiopin button_pin, gpiopin led_pin) {
    auto_button = led_button_new(button_pin, led_pin, auto_button_pressed);
}

void control_panel_add_on_button(gpiopin button_pin, gpiopin led_pin) {
    on_button = led_button_new(button_pin, led_pin, on_button_pressed);
}

bool control_panel_is_auto_mode(void) {
    return auto_mode;
}