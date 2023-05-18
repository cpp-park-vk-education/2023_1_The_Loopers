set(MAIN src/main.cpp)

add_executable(${GENERAL} ${MAIN})

set(PRIVATE_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${GENERAL} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})
