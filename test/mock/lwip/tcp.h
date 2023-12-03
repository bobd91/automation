#ifndef MOCK_LWIP_TCP_H__
#define MOCK_LWIP_TCP_H__

#incude "lwip/pbuf.h"

#define IPADDR_TYPE_V4 0
#define IP_GET_TYPE(ipaddr) IPADDR_TYPE_V4
#define TCP_WRITE_FLAG_MORE (0x02)

enum err_enum_t {
    ERR_OK,
    ERR_BUF,
};

typedef s8_t err_t;

struct ip_addr {
    u8_t type;
};

typedef struct ip_addr ip_addr_t;

struct tcp_pcb {
    ip_addr_t local_ip;
};
typedef err_t(* tcp_connected_fn) (void *arg, struct tcp_pcb *tpcb, err_t err);
typedef void(* tcp_err_fn) (void *arg, err_t err);
typedef err_t(* tcp_poll_fn) (void *arg, struct tcp_pcb *tpcb);
typedef err_t(* tcp_recv_fn) (void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
typedef err_t(* tcp_sent_fn) (void *arg, struct tcp_pcb *tpcb, u16_t len);

int ipaddr_aton(const char * cp, ip_addr_t *addr);
void tcp_recved(struct tcp_pcb *pcb, u16_t len);
err_t tcp_connect(struct tcp_pcb *pcb, const ip_addr_t *ipaddr, u16_t port, tcp_connected_fn connected);
struct tcp_pcb *tcp_new_ip_type(u8_t type);
void tcp_arg(struct tcp_pcb *pcb, void *arg);
void tcp_recv(struct tcp_pcb *pcb, tcp_recv_fn recv);
void tcp_sent(struct tcp_pcb *pcb, tcp_sent_fn sent);
void tcp_err(struct tcp_pcb *pcb, tcp_err_fn err);
void tcp_poll(struct tcp_pcb *pcb, tcp_poll_fn poll, u8_t interval);
err_t tcp_write(struct tcp_pcb *pcb, const void *arg, u16_t len, u8_t apiflags);
err_t tcp_output(struct tcp_pcb *pcb);

#endif