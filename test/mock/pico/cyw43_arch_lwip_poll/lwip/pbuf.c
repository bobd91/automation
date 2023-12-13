#include "lwip/pbuf.h"

#define PBUFP_FMT "&{ .tot_len=%d, .data=\"%.30s\" }"

// Our pbufs are not long lived
// and we can cope with just the one
static struct pbuf mock_pbuf;

u16_t mock_pbuf_copy_partial(const struct pbuf *buf, void *dataptr, u16_t len, u16_t offset) {
    u16_t num_bytes = len < buf->tot_len ? len : buf->tot_len;
    memcpy(dataptr, buf->data, num_bytes);
    return num_bytes;
}

u16_t pbuf_copy_partial(const struct pbuf *buf, void *dataptr, u16_t len, u16_t offset) {
    assert(buf == &mock_pbuf);
    assert(0 < buf->tot_len);
    assert(0 < len);
    assert(dataptr);
    MOCK_TRACE(PBUFP_FMT ", dataptr, %u, %u", buf->tot_len, buf->data, len, offset);

    return mock_pbuf_copy_partial(buf, dataptr, len, offset);
}

u8_t pbuf_free(struct pbuf *p) {
    assert(p == &mock_pbuf);
    MOCK_TRACE(PBUFP_FMT, p->tot_len, p->data);

    mock_pbuf_free();

    return 1;
}

struct pbuf *mock_pbuf_free(void) {
    mock_pbuf.tot_len = 0;
    mock_pbuf.data[0] = '\0';
    return &mock_pbuf;
}

void mock_pbuf_concat(const char *data) {
    // We must copy over trailing NULL of any previous data
    int prev_len = mock_pbuf.tot_len;
    prev_len = prev_len ? prev_len - 1 : 0;

    strcpy(mock_pbuf.data + prev_len, data);

    mock_pbuf.tot_len = prev_len + strlen(data) + 1;
}
