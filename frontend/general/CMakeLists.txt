add_executable(${GENERAL} "")

target_sources(${GENERAL} PRIVATE
        "src/temp.cpp"
        )

set(PRIVATE_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${GENERAL} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})
