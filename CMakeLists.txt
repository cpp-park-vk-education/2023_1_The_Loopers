cmake_minimum_required(VERSION 3.12)
project(CPP_HW1 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_TEST "Set to \"ON\" for test build" ON)
option(ENABLE_CLANG_TIDY "Set to \"ON\" to enable clang-tidy" OFF)
option(ENABLE_CLANG_FORMAT "Set to \"ON\" to add clang-format build target" OFF)
option(ENABLE_CPPLINT "Set to \"ON\" to enable cpplint" OFF)
option(ENABLE_CPPCHECK "Set to \"ON\" to enable cppcheck" OFF)
option(ENABLE_SANITIZER "Set to \"ON\" to enable sanitizer in debug build" OFF)

include(cmake/apply_options.cmake)

# add_subdirectory(Back)
# add_subdirectory(Utilities)

# add_executable(${PROJECT_NAME} "")
# target_sources(${PROJECT_NAME}
# PRIVATE
# "src/main.cpp"
# "src/solve_hw1.cpp"
# )

# set(PRIVATE_INCLUDE_DIRECTORIES
# "include"
# )

# target_include_directories(${PROJECT_NAME} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

# set(PRIVATE_LINK_LIBRARIES
# "imdb_top_movies"
# )

# target_link_libraries(${PROJECT_NAME} PRIVATE ${PRIVATE_LINK_LIBRARIES})
