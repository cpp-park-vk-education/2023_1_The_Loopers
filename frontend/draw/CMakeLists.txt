add_library(${DRAW} STATIC "")

set_target_properties(${DRAW} PROPERTIES AUTOMOC TRUE)

target_sources(${DRAW} PRIVATE
        "src/temp.cpp"
        "src/DrawView.cpp"
        "src/DrawModel.cpp"
        "src/IObject.cpp"
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        "Itool/include"
        "tool_box/include"
        )

target_include_directories(${DRAW} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
        "${REQUIRED_LIBS_QUALIFIED}"
        "${SERIALIZER}"
        "${CLIENT_NETWORK}"
        "${NETWORK_ADAPTER}"
        )

target_link_libraries(${DRAW} PRIVATE ${PRIVATE_LINK_LIBRARIES})
