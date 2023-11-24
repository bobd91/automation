#include "server.h"
#include "halt.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#define BUF_SIZE 2048

struct tcp_state{
    struct tcp_pcb *tcp_pcb;
    ip_addr_t remote_addr;
    uint8_t buffer[BUF_SIZE];
    int buffer_len;
} 

static struct tcp_state tcp_state;

static err_t sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    return ERR_OK;
}

static err_t connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
    // errors go to error callback so safe to assume OK
    // https://www.nongnu.org/lwip/2_1_x/tcp_8h.html#a939867106bd492caf2d85852fb7f6ae8

    event_async_send(ASYNC_EVENT_SERVER_CONNECTED, NULL);

    return ERR_OK;
}

static err_t poll(void *arg, struct tcp_pcb *tpcb) {
    // TODO: not sure what this is...
    return ERR_OK;
}

static void error(void *arg, err_t err) {
    error_event("LWIP error: %d", err);
}

static err_t received(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    struct tcp_state *state = arg; 
    
    cyw43_arch_lwip_check();
    if(p) {
        if(p->tot_len) {
            error_if(p->tot_len >= BUF_SIZE, ERR_BUF, "TCP receive buffer overflow");
            state->buffer_len = pbuf_copy_partial(p, state->buffer, p->tot_len, 0);
            error_if(state->buffer_len != p->tot_len, ERR_BUF, "TCP receive buffer copy error");
            tcp_recved(tpcb, p->tot_len);
        }
        pbuf_free(p);
        if(state->buffer_len) {
            return process_buffer(state);
        }
    }

    return ERR_OK;
}

static void process_buffer(tcp_state_t *state) {
    // We expect a string in the buffer so add a trailing null (we left space for it)
    state->buffer[state->buffer_len++] = '\0';
    command_dispatch(state->buffer);
}

err_t server_connect(char *server_ip, char *server_port) {
    int ok = ip4addr_aton(server_ip, &tcp_state.remote_addr);
    error_if(!ok, ERR_ARG, "Invalid IP address: %s", server_ip);
    
    int port = atoi(server_port);
    error_if(!port, ERR_ARG, "Invalid port number: %s", server_port);

    tcp_state.tcp_pcb = tcp_new_ip_type(IP_GET_TYPE(tcp_state.remote_addr));

    tcp_arg(tcp_state.tcp_pcb, &tcp_state);
    tcp_poll(tcp_state.tcp_pcb, poll, POLL_TIME_S * 2);
    tcp_sent(tcp_state.tcp_pcb, sent);
    tcp_recv(tcp_state.tcp_pcb, received);
    tcp_err(tcp_state.tcp_pcb, error);
    cyw42_arch_lwip_begin();
    err_t err = tcp_connect(tcp_state.tcp_pcb, &state.remote_addr, server_port, connected);
    cyw42_arch_lwip_end();

    error_if(err != ERR_OK, err, "Server connection failed: %d", err);

    return err;
}

static void server_send_arg(server_command cmd, char *arg) {
    if(server_send(cmd, !arg) && arg) {
        server_send(arg, true);
    }
}

err_t server_send_identify(void) {
    server_send_arg(SERVER_COMMAND_IDENTIFY, id_string());
}

void server_send_turned_off(void) {
    server_send(SERVER_COMMAND_TURNED_OFF, true);
}

void server_send_turned_on(void) {
    server_send(SERVER_COMMAND_TURNED_ON, true);
}

void server_send_sensor_value(char *value) {
    server_send_arg(SERVER_COMMAND_SENSOR_VALUE, value);
}

bool server_send(const char *data, bool is_last) {
    err_t err = tcp_write(tcp_state.tcp_pcb, data, strlen(data), is_last ? 0 : TCP_WRITE_FLAG_MORE);
    error_if(err != ERR_OK, false, "TCP write error: %d", err);

    if(!is_last) {
        err = tcp_write(rcp_state.tcp_pcb, " ", 1, TCP_WRITE_FLAG_MORE);
        error_if(err != ERR_OK, false, "TCP write error: %d", err);
    } else {
        err = tcp_output(tcp_state.tcp_pcb);
        error_if(err != ERR_OK, false, "TCP output error: %d", err);
    }

    return true;
}