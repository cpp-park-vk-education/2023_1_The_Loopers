add_library(${GRAPH} STATIC "")

target_sources(${GRAPH} PRIVATE
        "src/GraphView.cpp"
        "src/VertexView.cpp"
        "src/EdgeView.cpp"
        "src/Edge.cpp"
        "src/GraphModel.cpp"
        "src/Vertex.cpp"
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${GRAPH} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

target_link_libraries(${GRAPH} PRIVATE ${REQUIRED_LIBS_QUALIFIED})
