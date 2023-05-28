add_library(${SERVER_NETWORK} INTERFACE)

# target_sources(${SERVER_NETWORK} PRIVATE
#     # "service_session/src/iservice_session.cpp"
#     # "sessions_factory/src/isessions_factory.cpp"
#     # "listener/src/beast_websocket_listener.cpp"
# )

set(INTERFACE_LINK_LIBRARIES
    "${NETWORK_ADAPTER}"
    ${Boost_LIBRARIES}
)
if (WIN32)
    set(INTERFACE_LINK_LIBRARIES "${INTERFACE_LINK_LIBRARIES}"
        "wsock32"
        "ws2_32"
    )
endif()

target_link_libraries(${SERVER_NETWORK} INTERFACE ${INTERFACE_LINK_LIBRARIES})

set(INTERFACE_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "include"
    "listener/include"
    "service_session/include"
    "sessions_factory/include"

    # TODO (a.novak): same as utils/network_adapter/CMakeLists.txt
    "${Boost_INCLUDE_DIRS}"
)

target_include_directories(${SERVER_NETWORK} INTERFACE ${INTERFACE_INCLUDE_DIRECTORIES})
