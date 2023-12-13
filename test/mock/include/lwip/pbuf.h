#ifndef MOCK_LWIP_PBUF_H__
#define MOCK_LWIP_PBUF_H__

#define MOCK_PBUF_MAX_DATA 50

#include "mock/trace.h"
#include "lwip/arch.h"

struct pbuf {
    int tot_len;
    char data[1 + MOCK_PBUF_MAX_DATA];
};

u16_t pbuf_copy_partial(const struct pbuf *buf, void *dataptr, u16_t len, u16_t offset);
u8_t pbuf_free(struct pbuf *p);

u16_t mock_pbuf_copy_partial(const struct pbuf *buf, void *dataptr, u16_t len, u16_t offset);
struct pbuf *mock_pbuf_free(void);
void mock_pbuf_concat(const char *data);

#endif