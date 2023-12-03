#ifndef MOCK_PICO_CYW$£_ARCH_H__
#define MOCK_PICO_CYW$£_ARCH_H__


    int status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);

    if(status == CYW43_LINK_UP) {

    cyw43_arch_set_async_context(async_get_context());

    int err = cyw43_arch_init();

    cyw43_arch_enable_sta_mode();

    err = cyw43_arch_wifi_connect_async(wifi_ssid, wifi_password, CYW43_AUTH_WPA2_AES_PSK);
    
    cyw43_arch_lwip_check();
    cyw43_arch_lwip_begin();
    cyw43_arch_lwip_end();

#endif