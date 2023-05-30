# add_library(${DRAW} STATIC "")

set(SOURCES
        "src/temp.cpp"
        "src/NotDrawView.cpp"
        "src/NotDrawModel.cpp"
        "src/DrawSceneModel.cpp"
        "src/GraphicsDrawView.cpp"
        "src/IObject.cpp"
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        "Itool/include"
        "tool_box/include"
        )

file(GLOB HEADER_FILES include/*.h)

qt_wrap_cpp(SOURCES 
        ${HEADER_FILES}
        tool_box/include/ToolBox.h
)

qt_add_library(${DRAW}
        STATIC
        "${SOURCES}"
)

target_include_directories(${DRAW} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
        "${REQUIRED_LIBS_QUALIFIED}"
        "${SERIALIZER}"
        "${CLIENT_NETWORK}"
        "${NETWORK_ADAPTER}"
        )

target_link_libraries(${DRAW} PRIVATE ${PRIVATE_LINK_LIBRARIES})
