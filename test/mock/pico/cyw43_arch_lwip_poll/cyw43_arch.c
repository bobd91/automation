#include "pico/cyw43_arch.h"

cyw43_t cyw43_state;

static bool lwip_begun;
static int connect_link_status = CYW43_LINK_UP;

int cyw43_tcpip_link_status(cyw43_t *self, int itf) {
    assert(self == &cyw43_state);
    assert(self->context);
    assert(self->is_init);
    assert(self->itf == itf);
    MOCK_TRACE("cyw43, %d", itf);

    return self->link_status;
}

void cyw43_arch_set_async_context(async_context_t *context) {
    assert(context);
    MOCK_TRACE("context");

    cyw43_state.context = context;
}

int cyw43_arch_init(void) {
    MOCK_TRACE("");

    cyw43_state.is_init = true;
    return 0;
}

void cyw43_arch_enable_sta_mode(void) {
    assert(cyw43_state.is_init);
    MOCK_TRACE("");

    cyw43_state.itf = CYW43_ITF_STA;
}

int cyw43_arch_wifi_connect_async(const char *ssid, const char *pw, uint32_t auth) {
    assert(cyw43_state.is_init);
    MOCK_TRACE("\"%s\", \"%s\", %u", ssid, pw, auth);

    cyw43_state.link_status = connect_link_status;
    return 0;
}

void cyw43_arch_lwip_begin(void) {
    assert(cyw43_state.is_init);
    assert(!lwip_begun);
    MOCK_TRACE("");

    lwip_begun = true;
}

void cyw43_arch_lwip_end(void) {
    assert(cyw43_state.is_init);
    assert(lwip_begun);
    MOCK_TRACE("");

    lwip_begun = false;
}

void cyw43_arch_lwip_check(void) {
    assert(cyw43_state.is_init);
    MOCK_TRACE("");
}

void mock_set_connect_link_status(int status) {
    connect_link_status = status;
}