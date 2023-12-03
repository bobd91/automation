add_executable(automation_button_monitor
        button_monitor.c
        )

target_include_directories(automation_button_monitor PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(automation_button_monitor
        automation_core
        )

pico_add_extra_outputs(automation_button_monitor)
