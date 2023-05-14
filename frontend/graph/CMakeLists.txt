add_executable(${GRAPH} "")

target_sources(${GRAPH} PRIVATE
        "src/temp.cpp"
        )

set(PRIVATE_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${GRAPH} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})
