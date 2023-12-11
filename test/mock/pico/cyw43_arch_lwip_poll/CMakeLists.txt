if(NOT TARGET pico_cyw43_arch_lwip_poll)

    add_library(pico_cyw43_arch_lwip_poll INTERFACE)

    target_sources(pico_cyw43_arch_lwip_poll INTERFACE
            async_context.c
            async_context_poll.c
            cyw43_arch.c
            lwip/pbuf.c
            lwip/tcp.c
    )

    target_include_directories(pico_cyw43_arch_lwip_poll INTERFACE
            ${MOCK_INCLUDE_DIR}
    )

endif()
