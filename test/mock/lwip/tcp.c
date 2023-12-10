#include "lwip/tcp.h"

static bool run_connected_fn, run_recv_fn, run_sent_fn;

static struct tcp_pcb mock_tcp_pcb = { .error = ERR_OK };

int ip4addr_aton(const char *cp, ip_addr_t *addr) {
    assert(cp);
    assert(addr);
    int len= strlen(cp);
    assert(len <= IP_ADDRESS_LEN);
    MOCK_TRACE("%s, ip_addr_t *", cp);

    strcpy(addr->ip_address, cp);
    return 1;    
}

void tcp_recved(struct tcp_pcb *pcb, u16_t len) {
    assert(pcb == &mock_tcp_pcb);
    assert(pcb->recv_pbuf);
    assert(pcb->recv_pbuf->tot_len >= len);
    MOCK_TRACE("pcb, %u", len);

    pcb->recv_pbuf->tot_len -= len;
}

err_t tcp_connect(struct tcp_pcb *pcb, const ip_addr_t *ipaddr, u16_t port, tcp_connected_fn connected) {
    assert(pcb == &mock_tcp_pcb);
    assert(ipaddr);
    assert(0 != strlen(ipaddr->ip_address));
    assert(0 < port);
    assert(65536 > port);
    MOCK_TRACE("pcb, %s, %u, connected_fn", ipaddr->ip_address, port);

    pcb->ip = *ipaddr;
    pcb->port = port;
    pcb->connected_fn = connected;
    pcb->connected = false;

    return ERR_OK;
}

struct tcp_pcb *tcp_new_ip_type(u8_t type) {
    assert(type == IPADDR_TYPE_V4);
    MOCK_TRACE("%u", type);

    return &mock_tcp_pcb;
}

void tcp_arg(struct tcp_pcb *pcb, void *arg) {
    assert(pcb == &mock_tcp_pcb);
    MOCK_TRACE("pcb, arg");

    pcb->arg = arg;
}

void tcp_recv(struct tcp_pcb *pcb, tcp_recv_fn recv) {
    assert(pcb == &mock_tcp_pcb);
    MOCK_TRACE("pcb, recv_fn");

    pcb->recv_fn = recv;
}

void tcp_sent(struct tcp_pcb *pcb, tcp_sent_fn sent) {
    assert(pcb == &mock_tcp_pcb);
    MOCK_TRACE("pcb, sent_fn");
}

void tcp_err(struct tcp_pcb *pcb, tcp_err_fn err) {
    assert(pcb == &mock_tcp_pcb);
    MOCK_TRACE("pcb, err_fn");

    pcb->err_fn = err;
}

void tcp_poll(struct tcp_pcb *pcb, tcp_poll_fn poll, u8_t interval) {
    assert(pcb == &mock_tcp_pcb);
    MOCK_TRACE("pcb, poll_fn, %u", interval);
}

err_t tcp_write(struct tcp_pcb *pcb, const void *arg, u16_t len, u8_t apiflags) {
    assert(pcb == &mock_tcp_pcb);
    assert(arg);
    assert(len > 0);
    MOCK_TRACE("pcb, %.10s, %u, %u", arg, len, apiflags);

    return ERR_OK;
}

err_t tcp_output(struct tcp_pcb *pcb) {
    assert(pcb == &mock_tcp_pcb);
    MOCK_TRACE("pcb");

    return ERR_OK;
}

void mock_tcp_receive_data(const char *data) {
    mock_tcp_pcb.recv_pbuf = mock_pbuf_init(data);
}

void mock_tcp_error(err_t error) {
    mock_tcp_pcb.error = error;
}

void mock_tcp_call_pending_callbacks(void) {
    struct tcp_pcb *pcb = &mock_tcp_pcb;
    if(pcb->connected_fn && !pcb->connected) {
        pcb->connected = true;
        (*pcb->connected_fn)(pcb->arg, pcb, ERR_OK);
    }
    if(pcb->recv_fn && pcb->recv_pbuf && pcb->recv_pbuf->tot_len) {
        (*pcb->recv_fn)(pcb->arg, pcb, pcb->recv_pbuf, ERR_OK);
    }
    if(pcb->err_fn && pcb->error != ERR_OK)  {
        (*pcb->err_fn)(pcb->arg, pcb->error);
        pcb->error = ERR_OK;

    }
}

