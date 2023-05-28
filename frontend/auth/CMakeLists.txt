add_library(${AUTH} STATIC "")

target_sources(${AUTH} PRIVATE
        "src/temp.cpp"
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${AUTH} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

target_link_libraries(${AUTH} PRIVATE ${REQUIRED_LIBS_QUALIFIED})
