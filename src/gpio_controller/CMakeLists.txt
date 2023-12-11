include(gpio_pins.cmake)

add_executable(automation_gpio_controller
        gpio_controller.c
)

target_include_directories(automation_gpio_controller PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(automation_gpio_controller PRIVATE
    SWITCH_PIN=${SWITCH_PIN}
    OFF_BUTTON_PIN=${OFF_BUTTON_PIN}
    OFF_LED_PIN=${OFF_LED_PIN}
    AUTO_BUTTON_PIN=${AUTO_BUTTON_PIN}
    AUTO_LED_PIN=${AUTO_LED_PIN}
    ON_BUTTON_PIN=${ON_BUTTON_PIN}
    ON_LED_PIN=${ON_LED_PIN}
)

target_link_libraries(automation_gpio_controller
        automation_core
)

pico_add_extra_outputs(automation_gpio_controller)