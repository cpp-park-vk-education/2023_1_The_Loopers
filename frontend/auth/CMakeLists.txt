add_executable(${AUTH} "")

target_sources(${AUTH} PRIVATE
        "src/temp.cpp"
        )

set(PRIVATE_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${AUTH} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})
