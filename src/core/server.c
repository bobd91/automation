#include "server.h"
#include "async_event.h"
#include "error_event.h"
#include "server_command.h"
#include "unique_id.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include <string.h>

#define BUF_SIZE 2048
#define POLL_TIME_S 5

struct lwip_state {
    struct tcp_pcb *tcp_pcb;
    ip_addr_t remote_addr;
    uint8_t buffer[BUF_SIZE];
    int buffer_len;
};

typedef struct lwip_state lwip_state;

static lwip_state tcp_state;

static char* server_ip;
static char *server_port;

static void process_buffer(lwip_state *state) {
    // We expect a string in the buffer so add a trailing null (we left space for it)
    state->buffer[state->buffer_len++] = '\0';
    server_command_dispatch(state->buffer);
}

static err_t sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    // We ignore the sent callback
    return ERR_OK;
}

static err_t connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
    // errors go to error callback so safe to assume OK
    // https://www.nongnu.org/lwip/2_1_x/tcp_8h.html#a939867106bd492caf2d85852fb7f6ae8

    async_event_send(ASYNC_EVENT_SERVER_CONNECTED);

    return ERR_OK;
}

static err_t poll(void *arg, struct tcp_pcb *tpcb) {
    // We ignore the poll timer
    return ERR_OK;
}

static void error(void *arg, err_t err) {
    error_if(true,, ERROR_EVENT_TCP_ERROR, err);
}

static err_t received(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    lwip_state *state = arg; 
    
    cyw43_arch_lwip_check();
    if(p) {
        if(p->tot_len) {
            error_if(p->tot_len >= BUF_SIZE, ERR_BUF, ERROR_EVENT_BUFFER_OVERFLOW, 0);
            state->buffer_len = pbuf_copy_partial(p, state->buffer, p->tot_len, 0);
            error_if(state->buffer_len != p->tot_len, ERR_BUF, ERROR_EVENT_BUFFER_COPY, 0);
            tcp_recved(tpcb, p->tot_len);
        }
        pbuf_free(p);
        if(state->buffer_len) {
            process_buffer(state);
            return ERR_OK;
        }
    }

    return ERR_OK;
}

static void server_connect() {
    int ok = ip4addr_aton(server_ip, &tcp_state.remote_addr);
    error_if(!ok,, ERROR_EVENT_IP_ADDRESS, 0);
    
    int port = atoi(server_port);
    error_if(!port,, ERROR_EVENT_IP_PORT, 0);

    tcp_state.tcp_pcb = tcp_new_ip_type(IP_GET_TYPE(tcp_state.remote_addr));

    tcp_arg(tcp_state.tcp_pcb, &tcp_state);
    tcp_poll(tcp_state.tcp_pcb, poll, POLL_TIME_S * 2);
    tcp_sent(tcp_state.tcp_pcb, sent);
    tcp_recv(tcp_state.tcp_pcb, received);
    tcp_err(tcp_state.tcp_pcb, error);
    cyw43_arch_lwip_begin();
    err_t err = tcp_connect(tcp_state.tcp_pcb, &tcp_state.remote_addr, port, connected);
    cyw43_arch_lwip_end();

    error_if(err != ERR_OK,, ERROR_EVENT_TCP_CONNECT, err);

    return;
}

static bool server_send(const char *data, bool is_last) {
    err_t err = tcp_write(tcp_state.tcp_pcb, data, strlen(data), is_last ? 0 : TCP_WRITE_FLAG_MORE);
    error_if(err != ERR_OK, false, ERROR_EVENT_TCP_WRITE, err);

    if(!is_last) {
        err = tcp_write(tcp_state.tcp_pcb, " ", 1, TCP_WRITE_FLAG_MORE);
        error_if(err != ERR_OK, false, ERROR_EVENT_TCP_WRITE, err);
    } else {
        err = tcp_output(tcp_state.tcp_pcb);
        error_if(err != ERR_OK, false, ERROR_EVENT_TCP_OUTPUT, err);
    }

    return true;
}

static void server_send_arg(const char *cmd, const char *arg) {
    if(server_send(cmd, !arg) && arg) {
        server_send(arg, true);
    }
}

static void server_send_identify(void) {
    server_send_arg(SERVER_COMMAND_IDENTIFY, unique_id());
}

static void server_send_turned_off(void) {
    server_send(SERVER_COMMAND_TURNED_OFF, true);
}

static void server_send_turned_on(void) {
    server_send(SERVER_COMMAND_TURNED_ON, true);
}

static void server_send_sensor_value(const char *value) {
    server_send_arg(SERVER_COMMAND_SENSOR_VALUE, value);
}

static void server_send_error_message(const char *error_message) {
    server_send_arg(SERVER_COMMAND_ERROR, error_message);
}

void server_init(char *ip, char *port) {
    server_ip = ip;
    server_port = port;

    async_event_listen(ASYNC_EVENT_WIFI_CONNECTED, server_connect);
    async_event_listen(ASYNC_EVENT_SERVER_CONNECTED, server_send_identify);
    async_event_listen(ASYNC_EVENT_TURNED_OFF, server_send_turned_off);
    async_event_listen(ASYNC_EVENT_TURNED_ON, server_send_turned_on);
    async_event_listen_arg(ASYNC_EVENT_SENSOR_VALUE, server_send_sensor_value);
    async_event_listen_arg(ASYNC_EVENT_SERVER_ERROR, server_send_error_message);
}