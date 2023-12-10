# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(automation_tests C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(WIFI_SSID "TEST_SSID" CACHE INTERNAL "WiFi SSID")
set(WIFI_PASSWORD "TEST_PASSWORD" CACHE INTERNAL "WiFi Password")
set(SERVER_IP "1.2.3.4" CACHE INTERNAL "Server IP Address")
set(SERVER_PORT "5678" CACHE INTERNAL "Server Port")


add_executable(automation_tests
        automation_tests.c
        )

target_include_directories(automation_tests PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mock
        ${CMAKE_CURRENT_LIST_DIR}/../src/core
        ${CMAKE_CURRENT_LIST_DIR}/../src/gpio_monitor
        ${CMAKE_CURRENT_LIST_DIR}/../src/gpio_controller
        ${CMAKE_CURRENT_LIST_DIR}/../src/button_monitor
        )


target_sources(automation_tests PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mock/hardware/gpio.c
        ${CMAKE_CURRENT_LIST_DIR}/mock/hardware/watchdog.c
        ${CMAKE_CURRENT_LIST_DIR}/mock/lwip/pbuf.c
        ${CMAKE_CURRENT_LIST_DIR}/mock/lwip/tcp.c
        ${CMAKE_CURRENT_LIST_DIR}/mock/pico/async_context_poll.c
        ${CMAKE_CURRENT_LIST_DIR}/mock/pico/async_context.c
        ${CMAKE_CURRENT_LIST_DIR}/mock/pico/cyw43_arch.c
        ${CMAKE_CURRENT_LIST_DIR}/mock/pico/stdio.c
        ${CMAKE_CURRENT_LIST_DIR}/mock/pico/time.c
        ${CMAKE_CURRENT_LIST_DIR}/mock/pico/unique_id.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/core.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/gpiopin.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/led_button.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/async.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/async_event.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/error_event.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/control_panel.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/wifi.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/server.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/server_command.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/gpio_sensor.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/gpio_switch.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/switch_action.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/core/unique_id.c
)

target_compile_definitions(automation_tests PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        SERVER_IP=\"${SERVER_IP}\"
        SERVER_PORT=\"${SERVER_PORT}\"
)