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

