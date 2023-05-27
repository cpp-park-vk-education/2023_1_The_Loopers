string(ASCII 27 Esc)
set(ResetColor "${Esc}[m")

if(ENABLE_CLANG_FORMAT)
    include(cmake/get_all_sources.cmake)
endif()

if(ENABLE_CLANG_TIDY)
    set(BoldBlue "${Esc}[1;34m")
    message("\n${BoldBlue}Clang tidy is enabled${ResetColor}\n")

    set(CLANG_TIDY_FLAGS
        "--export-fixes=fixes.yaml"
        "--use-color"
        "-p=${CMAKE_BUILD_RPATH}"
    )

    set(CMAKE_CXX_CLANG_TIDY clang-tidy ${CLANG_TIDY_FLAGS})
endif()

if(ENABLE_CPPLINT)
    set(BoldBlue "${Esc}[1;34m")
    message("\n${BoldBlue}Cpplint is enabled${ResetColor}\n")

    set(CPPLINT_FLAGS
        "--linelength=100"
        "--filter=-build/include_subdir,-legal/copyright,-whitespace/indent,-build/namespaces,-whitespace/braces,-whitespace/newline"
    )

    set(CMAKE_CXX_CPPLINT "cpplint" ${CPPLINT_FLAGS})
endif()

if(ENABLE_CPPCHECK)
    set(BoldBlue "${Esc}[1;34m")
    message("\n${BoldBlue}Cppcheck is enabled${ResetColor}\n")

    set(CPPCHECK_FLAGS
        "--report-progress"
    )

    set(CMAKE_CXX_CPPCHECK "cppcheck" ${CPPCHECK_FLAGS})
endif()

if(ENABLE_CLANG_FORMAT)
    set(BoldYellow "${Esc}[1;33m")
    message("\n${BoldYellow}Clang format build target is added${ResetColor}\n")

    include(cmake/clang_format.cmake)

    clangformat_setup(${ALL_SOURCE_FILES})
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(BoldGreen "${Esc}[1;32m")
    message("\n${BoldGreen}Release build${ResetColor}\n")
else()
    set(BoldMagenta "${Esc}[1;35m")
    set(DEBUG_MESSAGE "\n${BoldMagenta}Debug build")

    if(ENABLE_SANITIZER)
        set(DEBUG_MESSAGE "${DEBUG_MESSAGE} with sanitizer enabled")

        set(SANITIZERS_FLAGS
            "-fsanitize=address"
            "-fsanitize=undefined"
            "-fsanitize=leak"
        )

        add_compile_options(${SANITIZERS_FLAGS})
        add_link_options(${SANITIZERS_FLAGS})
    endif()

    set(DEBUG_MESSAGE "${DEBUG_MESSAGE}${ResetColor}\n")
    message(${DEBUG_MESSAGE})

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") 
        set(WARNINGS_FLAGS
            # "-W2"
        )
    else()
        set(WARNINGS_FLAGS
            "-Werror"
            "-Wall"
            "-Wextra"
            "-Wpedantic"
            "-Wno-error=return-type"
            "-Wno-error=unused-parameter"
            # "-Wno-error=nested-anon-types"
        )
        if (WIN32)
            list(APPEND ${WARNINGS_FLAGS} "-Wno-error=nested-anon-types") # boost/winapi/file_management.hpp
        endif()
        if(NOT ENABLE_CLANG_TIDY)
            list(APPEND ${WARNINGS_FLAGS} "-Wabi=${CMAKE_CXX_STANDARD}")
        endif()
    endif()

    add_compile_options(${WARNINGS_FLAGS})
endif()

if(ENABLE_TEST)
    set(BoldCyan "${Esc}[1;36m")
    message("\n${BoldCyan}Build with tests is enabled${ResetColor}\n")

    find_package(GTest)

    if(${GTest_FOUND})
    else()
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.11.0
        )
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

    enable_testing()

    set(SERVICES_DIR "${PROJECT_SOURCE_DIR}/backend/services")
    set(BACKEND_UTILS_DIR "${PROJECT_SOURCE_DIR}/backend/utils")
endif()
