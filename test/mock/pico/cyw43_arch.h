#ifndef MOCK_PICO_CYW43_ARCH_H__
#define MOCK_PICO_CYW43_ARCH_H__

#include "mock.h"
#include "pico/async_context.h"

struct mock_cyw43 {
    int link_status;
    async_context_t *context;
    bool is_init;
    int itf;

};

typedef struct mock_cyw43 cyw43_t;
extern cyw43_t cyw43_state;

enum {
CYW43_ITF_STA,  
CYW43_ITF_AP,   
};

#define CYW43_LINK_DOWN    (0)
#define CYW43_LINK_JOIN    (1)
#define CYW43_LINK_NOIP    (2)
#define CYW43_LINK_UP      (3)
#define CYW43_LINK_FAIL    (-1)
#define CYW43_LINK_NONET   (-2)
#define CYW43_LINK_BADAUTH (-3)

#define CYW43_AUTH_WPA2_AES_PSK   (0x00400004)

int cyw43_tcpip_link_status(cyw43_t *self, int itf);
void cyw43_arch_set_async_context(async_context_t *context);
int cyw43_arch_init(void);
void cyw43_arch_enable_sta_mode(void);
int cyw43_arch_wifi_connect_async(const char *ssid, const char *pw, uint32_t auth);
void cyw43_arch_lwip_begin(void);
void cyw43_arch_lwip_end(void); 
void cyw43_arch_lwip_check(void);

#endif