# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD "pico_w" CACHE INTERNAL "Will not work without pico_w")

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(automation C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise the SDK
pico_sdk_init()

set(WIFI_SSID "${WIFI_SSID}" CACHE INTERNAL "WiFi SSID")
set(WIFI_PASSWORD "${WIFI_PASSWORD}" CACHE INTERNAL "WiFi Password")
set(SERVER_IP "${SERVER_IP}" CACHE INTERNAL "Server IP Address")
set(SERVER_PORT, "${SERVER_PORT}" CACHE INTERNAL "Server Port")
        
add_subdirectory(core)
add_subdirectory(button_monitor)
add_subdirectory(gpio_monitor)
add_subdirectory(gpio_controller)
