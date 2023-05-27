add_library(${DRAW} STATIC "")

target_sources(${DRAW} PRIVATE
        "src/temp.cpp"
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        "itool/include"
        "tool_box/include"
        )

target_include_directories(${DRAW} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

target_link_libraries(${DRAW} PRIVATE ${REQUIRED_LIBS_QUALIFIED})
