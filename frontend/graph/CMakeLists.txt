add_library(${GRAPH} STATIC "")

target_sources(${GRAPH} PRIVATE
        src/Edge.cpp
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${GRAPH} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
