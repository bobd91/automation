#ifndef MOCK_HARDWARE_WATCHDOG_H__
#define MOCK_HARDWARE_WATCHDOG_H__

void watchdog_enable(uint32_t delay_ms, bool pause_on_debug);

#endif