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