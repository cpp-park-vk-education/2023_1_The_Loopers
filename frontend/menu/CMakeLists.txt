add_executable(${MENU} "")

target_sources(${MENU} PRIVATE
        "src/temp.cpp"
        )

set(PRIVATE_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${MENU} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})
