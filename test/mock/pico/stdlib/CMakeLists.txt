if(NOT TARGET pico_stdlib)

    add_library(pico_stdlib INTERFACE)

    target_sources(pico_stdlib INTERFACE
            stdlib.c
    )

    target_include_directories(pico_stdlib INTERFACE
            ${MOCK_INCLUDE_DIR}
    )

endif()
