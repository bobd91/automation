if(NOT TARGET pico_unique_id)

    add_library(pico_unique_id INTERFACE)

    target_sources(pico_unique_id INTERFACE
            unique_id.c
    )

    target_include_directories(pico_unique_id INTERFACE
            ${MOCK_INCLUDE_DIR}
    )

    target_link_libraries(pico_unique_id INTERFACE
        mock_trace
    )

endif()
