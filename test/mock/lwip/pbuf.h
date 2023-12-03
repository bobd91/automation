#ifndef MOCK_LWIP_PBUF_H__
#define MOCK_LWIP_PBUF_H__


struct pbuf {
    int tot_len;
};

u16_t pbuf_copy_partial(const struct pbuf *buf, void *dataptr, u16_t len, u16_t offset);

#endif