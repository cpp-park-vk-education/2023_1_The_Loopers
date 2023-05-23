add_library(${CLIENT_NETWORK} INTERFACE)

# target_sources(${CLIENT_NETWORK} PRIVATE
#     "src/websocket_client_session.cpp"
# )

set(INTERFACE_LINK_LIBRARIES
    ${Boost_LIBRARIES}  
    "${NETWORK_ADAPTER}"
)
if (WIN32)
    set(INTERFACE_LINK_LIBRARIES "${PRIVATE_LINK_LIBRARIES}"
        "wsock32"
        "ws2_32"
    )
endif()

target_link_libraries(${CLIENT_NETWORK} INTERFACE ${INTERFACE_LINK_LIBRARIES})

set(INTERFACE_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "${UTILS_DIR}/network_adapter/include"
    "include"

    # TODO (a.novak): same as utils/network_adapter/CMakeLists.txt
    "${Boost_INCLUDE_DIRS}"
)

target_include_directories(${CLIENT_NETWORK} INTERFACE ${INTERFACE_INCLUDE_DIRECTORIES})
