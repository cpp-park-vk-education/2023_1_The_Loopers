function(prefix_clangformat_setup prefix)
    if(NOT CLANGFORMAT_EXECUTABLE)
        set(CLANGFORMAT_EXECUTABLE clang-format)
    endif()

    if(NOT EXISTS ${CLANGFORMAT_EXECUTABLE})
        find_program(clangformat_executable_tmp ${CLANGFORMAT_EXECUTABLE})

        if(clangformat_executable_tmp)
            set(CLANGFORMAT_EXECUTABLE ${clangformat_executable_tmp})
            unset(clangformat_executable_tmp)
        else()
            message(FATAL_ERROR "ClangFormat: ${CLANGFORMAT_EXECUTABLE} not found! Aborting")
        endif()
    endif()

    foreach(clangformat_source ${ARGN})
        get_filename_component(clangformat_source ${clangformat_source} ABSOLUTE)
        list(APPEND clangformat_sources ${clangformat_source})
    endforeach()

    add_custom_target(${prefix}_clangformat
        COMMAND
        ${CLANGFORMAT_EXECUTABLE}

        -i
        ${clangformat_sources}
        WORKING_DIRECTORY
        ${CMAKE_SOURCE_DIR}
        COMMENT
        "Formatting ${prefix} with ${CLANGFORMAT_EXECUTABLE} sources: ${clangformat_sources} ..."
    )

    if(TARGET clangformat)
        add_dependencies(clangformat ${prefix}_clangformat)
    else()
        add_custom_target(clangformat DEPENDS ${prefix}_clangformat)
    endif()
endfunction()

function(clangformat_setup SOURCE_FILES)
    prefix_clangformat_setup(${PROJECT_NAME} ${SOURCE_FILES})
endfunction()

function(target_clangformat_setup target)
    get_target_property(target_sources ${target} SOURCES)
    prefix_clangformat_setup(${target} ${target_sources})
endfunction()
