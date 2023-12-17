create_test_sourcelist(Tests
        automation_tests.c
        send_idfy.c
        recv_run.c
        press_off_when_auto.c
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