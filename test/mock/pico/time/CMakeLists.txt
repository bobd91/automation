if(NOT TARGET pico_time)

    add_library(pico_time INTERFACE)

    target_sources(pico_time INTERFACE
            time.c
    )

    target_include_directories(pico_time INTERFACE
            ${MOCK_INCLUDE_DIR}
    )

    target_link_libraries(pico_time INTERFACE
        mock_trace
    )

endif()
