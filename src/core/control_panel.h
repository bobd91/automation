#ifndef CONTROL_PANEL_H__
#define CONTROL_PANEL_H__

#include "gpiopin.h"

#ifndef CONTROL_PANEL_BLINK_INTERVAL_MS
#define CONTROL_PANEL_BLINK_INTERVAL_MS 1000
#endif

#ifndef CONTROL_PANEL_FLASH_SHORT_ON_MS
#define CONTROL_PANEL_FLASH_SHORT_ON_MS 120
#endif

#ifndef CONTROL_PANEL_FLASH_SHORT_OFF_MS
#define CONTROL_PANEL_FLASH_SHORT_OFF_MS CONTROL_PANEL_FLASH_SHORT_ON_MS
#endif

#ifndef CONTROL_PANEL_FLASH_LONG_ON_MS
#define CONTROL_PANEL_FLASH_LONG_ON_MS (3 * CONTROL_PANEL_FLASH_SHORT_ON_MS)
#endif

#ifndef CONTROL_PANEL_FLASH_LONG_OFF_MS
#define CONTROL_PANEL_FLASH_LONG_OFF_MS CONTROL_PANEL_FLASH_LONG_ON_MS
#endif

void control_panel_init(void);
void control_panel_add_off_button(gpiopin button_pin, gpiopin led_pin);
void control_panel_add_auto_button(gpiopin button_pin, gpiopin led_pin);
void control_panel_add_on_button(gpiopin button_pin, gpiopin led_pin);

#endif