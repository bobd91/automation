if(NOT TARGET automation_core)

    add_library(automation_core INTERFACE)

    target_sources(automation_core INTERFACE
            core.c
            gpiopin.c
            led_button.c
            async.c
            async_event.c
            error_event.c
            control_panel.c
            wifi.c
            server.c
            server_command.c
            gpio_sensor.c
            gpio_switch.c
            switch_action.c
            unique_id.c
    )

    target_include_directories(automation_core INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}
    )

    target_compile_definitions(automation_core INTERFACE
            WIFI_SSID=\"${WIFI_SSID}\"
            WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
            SERVER_IP=\"${SERVER_IP}\"
            SERVER_PORT=${SERVER_PORT}
            ASYNC_POLL_LOOP_MS=${ASYNC_POLL_LOOP_MS}
            WIFI_LINK_CHECK_MS=${WIFI_LINK_CHECK_MS}
            CONTROL_PANEL_BLINK_INTERVAL_MS=${CONTROL_PANEL_BLINK_INTERVAL_MS}
            CONTROL_PANEL_FLASH_SHORT_ON_MS=${CONTROL_PANEL_FLASH_SHORT_ON_MS}
            CONTROL_PANEL_FLASH_SHORT_OFF_MS=${CONTROL_PANEL_FLASH_SHORT_OFF_MS}
            CONTROL_PANEL_FLASH_LONG_ON_MS=${CONTROL_PANEL_FLASH_LONG_ON_MS}
            CONTROL_PANEL_FLASH_LONG_OFF_MS=${CONTROL_PANEL_FLASH_LONG_OFF_MS}
            ERROR_EVENT_SLEEP_MS=${ERROR_EVENT_SLEEP_MS}
            ERROR_EVENT_REBOOT_MS=${ERROR_EVENT_REBOOT_MS}

    )

    target_link_libraries(automation_core INTERFACE
            pico_stdlib
            pico_stdio
            pico_time
            pico_unique_id
            pico_cyw43_arch_lwip_poll
            hardware_gpio
            hardware_watchdog
    )

endif()