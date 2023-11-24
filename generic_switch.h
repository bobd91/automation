#ifndef _GENERIC_SWITCH_H
#define _GENERIC_SWITCH_H

void generic_switch_register(async_event_listener turn_off, async_event_listener turn_on);
void generic_switch_init(void);

#endif