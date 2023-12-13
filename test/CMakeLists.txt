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

create_test_sourcelist(Tests
        automation_tests.c
        test_send_idfy.c
        test_recv_run.c
)

add_executable(automation_tests ${Tests})

target_include_directories(automation_tests PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

include(gpio_pins.cmake)

target_link_libraries(automation_tests
        automation_core
)

set (TestsToRun ${Tests})
remove (TestsToRun automation_tests.c)

foreach (test ${TestsToRun})
  get_filename_component (TName ${test} NAME_WE)
  add_test (NAME ${TName} COMMAND automation_tests ${TName})
endforeach ()