if(NOT TARGET hardware_gpio)

    add_library(hardware_gpio INTERFACE)

    target_sources(hardware_gpio INTERFACE
        gpio.c
    )

    target_include_directories(hardware_gpio INTERFACE
        ${MOCK_INCLUDE_DIR}
    )

    target_link_libraries(hardware_gpio INTERFACE
        mock_trace
    )

endif()