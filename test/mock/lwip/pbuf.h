#ifndef MOCK_LWIP_PBUF_H__
#define MOCK_LWIP_PBUF_H__

#define MOCK_PBUF_MAX_DATA 50

#include "mock.h"

struct pbuf {
    int tot_len;
    char data[1 + MOCK_PBUF_MAX_DATA];
};

u16_t pbuf_copy_partial(const struct pbuf *buf, void *dataptr, u16_t len, u16_t offset);
u8_t pbuf_free(struct pbuf *p);

struct pbuf *mock_pbuf_init(const char *data);

#endif