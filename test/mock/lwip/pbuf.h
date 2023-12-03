#ifndef MOCK_LWIP_PBUF_H__
#define MOCK_LWIP_PBUF_H__


struct pbuf *p

p->tot_len
state->buffer_len = pbuf_copy_partial(p, state->buffer, p->tot_len, 0);

#endif