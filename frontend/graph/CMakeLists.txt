add_library(${GRAPH} STATIC "")

target_sources(${GRAPH} PRIVATE
        "src/temp.cpp"
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${GRAPH} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

target_link_libraries(${GRAPH} PRIVATE ${REQUIRED_LIBS_QUALIFIED})
