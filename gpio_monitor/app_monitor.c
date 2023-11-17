#include "app_monitor.h"
#include "control_buttons.h"
#include "mains_detector.h"
#include "app_command.h"

#include "async.h"
#include "async_event_monitor.h"
#include "gpiopin.h"
#include "command.h"

enum app_state {
    APP_STATE_STARTING,
    APP_STATE_CONNECTED,
    APP_STATE_WAITING,
    APP_STATE_RUNNING,
    APP_STATE_HALTED
}

static app_state current_state = APP_STATE_STARTING;
static app_monitor_event last_button_event = APP_MONITOR_AUTO_BUTTON;

static void monitor_event(int event) {
    if(current_state == APP_STATE_RUNNING) {
        // If server thinks we are off:
        //   If event is button on or (event is current on and last button event was auto)
        //     Tell server we are on
        // If server thinks we are on:
        //   If event is button off or (event is current off and last button event was auto)
        //     Tell server we are of

    }   
}

static void app_monitor_init(void) {
    async_event_monitor_init(monitor_event);
    gpiopin_init();
    command_init();

    buttons_init();
    mains_detector_init();
    app_command_init();
}