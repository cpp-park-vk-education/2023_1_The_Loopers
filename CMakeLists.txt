cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0076 NEW)
if (${CMAKE_VERSION} VERSION_GREATER "3.24")
    cmake_policy(SET CMP0135 OLD)
endif()

project(inklink VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/get_boost_lib.cmake)

option(ENABLE_BACK "Set to \"ON\" to build back" ON)
option(ENABLE_FRONT "Set to \"ON\" to build front" OFF)

option(CLEAR_CACHE "Set to \"ON\" to clear cache" ON)

option(ENABLE_TEST "Set to \"ON\" for test build" ON)

option(ENABLE_CLANG_FORMAT "Set to \"ON\" to add clang-format build target" OFF)

option(ENABLE_CLANG_TIDY "Set to \"ON\" to enable clang-tidy" OFF)
option(ENABLE_CPPLINT "Set to \"ON\" to enable cpplint" OFF)
option(ENABLE_CPPCHECK "Set to \"ON\" to enable cppcheck" OFF)

option(ENABLE_SANITIZER "Set to \"ON\" to enable sanitizer in debug build" OFF)

set(UTILS_DIR "${PROJECT_SOURCE_DIR}/utils")

include(cmake/apply_options.cmake)

add_subdirectory(${UTILS_DIR})
if (ENABLE_BACK)
    add_subdirectory(back)
endif()
if (ENABLE_FRONT)
    add_subdirectory(front)
endif()
if (ENABLE_TEST)
    add_subdirectory(tests)
endif()

if (CLEAR_CACHE)
    unset(ENABLE_TEST CACHE)

    unset(ENABLE_CLANG_FORMAT CACHE)
    unset(ENABLE_CPPLINT CACHE)
    unset(ENABLE_CLANG_TIDY CACHE)
    unset(ENABLE_CPPCHECK CACHE)
    
    unset(ENABLE_SANITIZER CACHE)
endif()
