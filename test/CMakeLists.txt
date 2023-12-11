# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13)

if("$ENV{PICO_SDK_PATH}" STREQUAL "" OR NOT EXISTS $ENV{PICO_SDK_PATH})
    message(FATAL_ERROR "PICO_SDK_PATH must be set to the PICO-SDK directory")
endif()

# Set name of project (as PROJECT_NAME) and C/C   standards
project(automation_tests C)
set(CMAKE_C_STANDARD 11)

set(WIFI_SSID "TEST_SSID" CACHE INTERNAL "WiFi SSID")
set(WIFI_PASSWORD "TEST_PASSWORD" CACHE INTERNAL "WiFi Password")
set(SERVER_IP "1.2.3.4" CACHE INTERNAL "Server IP Address")
set(SERVER_PORT "5678" CACHE INTERNAL "Server Port")

include(timings.cmake)

add_subdirectory(mock)
add_subdirectory(core)

add_executable(automation_tests
        automation_tests.c
)

target_include_directories(automation_tests PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

include(gpio_pins.cmake)

target_link_libraries(automation_tests
        automation_core
)