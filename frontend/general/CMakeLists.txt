add_executable(${DRAW} "")

target_sources(${DRAW} PRIVATE
        "src/temp.cpp"
        )

set(PRIVATE_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${DRAW} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})
