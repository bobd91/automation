#ifndef MOCK_LWIP_TCP_H__
#define MOCK_LWIP_TCP_H__


struct tcp_pcb *tcp_pcb;
ip_addr_t remote_addr;
err_t
tcp_recved(tpcb, p->tot_len);
    int ok = ip4addr_aton(server_ip, &tcp_state.remote_addr);
    tcp_state.tcp_pcb = tcp_new_ip_type(IP_GET_TYPE(tcp_state.remote_addr));
    tcp_arg(tcp_state.tcp_pcb, &tcp_state);
    tcp_poll(tcp_state.tcp_pcb, poll, POLL_TIME_S * 2);
    tcp_sent(tcp_state.tcp_pcb, sent);
    tcp_recv(tcp_state.tcp_pcb, received);
    tcp_err(tcp_state.tcp_pcb, error);
    err_t err = tcp_connect(tcp_state.tcp_pcb, &tcp_state.remote_addr, port, connected);
    err_t err = tcp_write(tcp_state.tcp_pcb, data, strlen(data), is_last ? 0 : TCP_WRITE_FLAG_MORE);
        err = tcp_output(tcp_state.tcp_pcb);

#endif