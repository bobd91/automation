# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(automation_tests C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(WIFI_SSID "${WIFI_SSID}" CACHE INTERNAL "WiFi SSID")
set(WIFI_PASSWORD "${WIFI_PASSWORD}" CACHE INTERNAL "WiFi Password")
set(SERVER_IP "${SERVER_IP}" CACHE INTERNAL "Server IP Address")
set(SERVER_PORT, "${SERVER_PORT}" CACHE INTERNAL "Server Port")

