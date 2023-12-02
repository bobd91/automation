# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD "pico_w" CACHE INTERNAL "Will not work without pico_w")

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(automation C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(WIFI_SSID "${WIFI_SSID}" CACHE INTERNAL "WiFi SSID")
set(WIFI_PASSWORD "${WIFI_PASSWORD}" CACHE INTERNAL "WiFi Password")
set(SERVER_IP "${SERVER_IP}" CACHE INTERNAL "Server IP Address")
set(SERVER_PORT, "${SERVER_PORT}" CACHE INTERNAL "Server Port")

add_library(automation_core OBJECT
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

target_include_directories(automation_core PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_compile_definitions(automation_core PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        SERVER_IP=\"${SERVER_IP}\"
        SERVER_PORT=\"${SERVER_PORT}\"
        )

target_link_libraries(automation_core
        pico_stdlib
        pico_time
        pico_unique_id
        pico_cyw43_arch_lwip_poll
        hardware_gpio
        hardware_watchdog
        )


add_executable(automation_gpio_monitor
        gpio_monitor.c
        )

target_include_directories(automation_gpio_monitor PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(automation_gpio_monitor
        automation_core
        )

pico_add_extra_outputs(automation_gpio_monitor)



add_executable(automation_gpio_controller
        gpio_controller.c
        )

target_include_directories(automation_gpio_controller PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(automation_gpio_controller
        automation_core
        )

pico_add_extra_outputs(automation_gpio_controller)