#include "server.h"
#include "halt.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#define BUF_SIZE 2048

struct tcp_state_s {
    struct tcp_pcb *tcp_pcb;
    ip_addr_t remote_addr;
    uint8_t buffer[BUF_SIZE];
    int buffer_len;
} 

typedef tcp_state_t struct tcp_state_s;

static tcp_state_t tcp_state = { .buffer_len = 0 };

err_t server_connect(char *server_ip, int server_port) {
    int ok = ip4addr_aton(server_ip, &tcp_state.remote_addr);
    if(!ok) return error_event_async("Invalid IP address", server_ip, ERR_ARG);

    tcp_state.tcp_pcb = tcp_new_ip_type(IP_GET_TYPE(tcp_state.remote_addr));

    tcp_arg(tcp_state.tcp_pcb, &tcp_state);
    tcp_poll(tcp_state.tcp_pcb, poll, POLL_TIME_S * 2);
    tcp_sent(tcp_state.tcp_pcb, sent);
    tcp_recv(tcp_state.tcp_pcb, received);
    tcp_err(tcp_state.tcp_pcb, error);
    cyw42_arch_lwip_begin();
    err_t err = tcp_connect(tcp_state.tcp_pcb, &state.remote_addr, server_port, connected);
    cyw42_arch_lwip_end();

    if(err != ERR_OK) return error_event_async("Server connection failed", err);

    return err;
}


static err_t sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    // TODO: data has been sent, do we have to do anything?

    return ERR_OK;
}

static err_t connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
    // errors go to error callback so save to assume OK
    // https://www.nongnu.org/lwip/2_1_x/tcp_8h.html#a939867106bd492caf2d85852fb7f6ae8

    event_async(EVENT_CONNECTED, arg);

    return ERR_OK;
}

static err_t poll(void *arg, struct tcp_pcb *tpcb) {
    // TODO: not sure what this is...
    return ERR_OK;
}

static void error(void *arg, err_t err) {
    error_event_async("LWIP error", err);
}

err_t received(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    tcp_state_t *state = arg; 
    
    cyw43_arch_lwip_check();
    if (p->tot_len > 0) {
        if(p->tot_len + state->buffer_len > BUF_SIZE) return error_event_async("Buffer too small", ERR_BUF);
        state->buffer_len += pbuf_copy_partial(p, state->buffer + state->buffer_len,
                                               p->tot_len, 0);
        tcp_recved(tpcb, p->tot_len);
    }
    pbuf_free(p);

    process_buffer(state);

    return ERR_OK;
}

void process_buffer(tcp_state_t *state) {

}
