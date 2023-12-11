#include "hardware/watchdog.h"

// Currently we only use this for reboot 
void watchdog_enable(uint32_t delay_ms, bool pause_on_debug) {
    assert(0 == delay_ms);
    MOCK_TRACE("%u, %d", delay_ms, pause_on_debug);
    exit(1);
}