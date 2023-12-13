if(NOT TARGET pico_stdio)

    add_library(pico_stdio INTERFACE)

    target_sources(pico_stdio INTERFACE
            stdio.c
    )

    target_include_directories(pico_stdio INTERFACE
            ${MOCK_INCLUDE_DIR}
    )

    target_link_libraries(pico_stdio INTERFACE
        mock_trace
    )

endif()
