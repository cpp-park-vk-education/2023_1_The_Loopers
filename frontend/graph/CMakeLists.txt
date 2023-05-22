add_library(${GRAPH} STATIC "")

target_sources(${GRAPH} PRIVATE
#        "src/temp.cpp"
        "src/Edge.cpp"
        "src/GraphModel.cpp"
        "src/Vertex.cpp"
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${GRAPH} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
