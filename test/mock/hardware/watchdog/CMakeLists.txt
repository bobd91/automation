if(NOT TARGET hardware_watchdog)

    add_library(hardware_watchdog INTERFACE)

    target_sources(hardware_watchdog INTERFACE
            watchdog.c
    )

    target_include_directories(hardware_watchdog INTERFACE
            ${MOCK_INCLUDE_DIR}
    )

endif()