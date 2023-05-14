add_library(${GRAPH} SHARED "")

target_sources(${GRAPH} PRIVATE
        "src/temp.cpp"
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${GRAPH} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
