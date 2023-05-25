add_library(${MENU} STATIC "")

target_sources(${MENU} PRIVATE
        "src/temp.cpp"
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${MENU} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

target_link_libraries(${MENU} PRIVATE ${REQUIRED_LIBS_QUALIFIED})
