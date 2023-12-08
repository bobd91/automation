#include "lwip/pbuf.h"
#include "mock.h"
#include <assert.h>

#define PBUFP_FMT "&{ .tot_len=%d, .data=%.10s}"

// Our pbufs are not long lived
// and we can cope with just the one
static pbuf mock_pbuf;

u16_t pbuf_copy_partial(const struct pbuf *buf, void *dataptr, u16_t len, u16_t offset) {
    assert(buf == &mock_pbuf);
    assert(0 < buf->tot_len);
    assert(0 < len);
    assert(dataptr);
    MOCK_TRACE(PBUFP_FMT ", dataptr, %u, %u", buf->tot_len, buf->data, len, offset);

    u16_t num_bytes = len < buf->tot_len ? len : buf->tot_len;
    memcpy(dataptr, buf->data, num_bytes);

    return num_bytes;
}

u8_t pbuf_free(struct pbuf *p) {
    assert(p == &mock_pbuf);
    assert(0 < p->tot_len);
    MOCK_TRACE(PBUFP_FMT, p->tot_len, p->data);

    p->tot_len = 0;

    return 1;
}

struct pbuf *mock_pbuf(const char *data) {
    mock_pbuf.tot_len = strlen(data) + 1;
    strcpy(mock_pbuf.data, data);

    return &mock_pbuf;
}