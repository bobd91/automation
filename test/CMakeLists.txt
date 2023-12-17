# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(automation_tests C)
set(CMAKE_C_STANDARD 11)

include(CTest)

set(WIFI_SSID "TEST_SSID" CACHE INTERNAL "WiFi SSID")
set(WIFI_PASSWORD "TEST_PASSWORD" CACHE INTERNAL "WiFi Password")
set(SERVER_IP "1.2.3.4" CACHE INTERNAL "Server IP Address")
set(SERVER_PORT "5678" CACHE INTERNAL "Server Port")

include(timings.cmake)

add_subdirectory(mock)
add_subdirectory(core)
add_subdirectory(tests)