#ifndef _SWITCH_ACTION_H
#define _SWITCH_ACTION_H

#include "async_event.h"

void switch_action_register(async_event_listener turn_off, async_event_listener turn_on);
void switch_action_init(void);

#endif