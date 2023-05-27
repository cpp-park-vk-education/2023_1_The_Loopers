add_executable(${GENERAL} "")

target_sources(${GENERAL} PRIVATE
        "src/main.cpp"
        )

set(PRIVATE_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${GENERAL} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

target_link_libraries(${GENERAL} PRIVATE ${REQUIRED_LIBS_QUALIFIED} ${AUTH} ${DRAW} ${GRAPH} ${MENU})
