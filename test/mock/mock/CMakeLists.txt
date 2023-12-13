if(NOT TARGET mock_trace)

    add_library(mock_trace INTERFACE)

    target_sources(mock_trace INTERFACE
            trace.c
    )

    target_include_directories(mock_trace INTERFACE
            ${MOCK_INCLUDE_DIR}
    )

endif()