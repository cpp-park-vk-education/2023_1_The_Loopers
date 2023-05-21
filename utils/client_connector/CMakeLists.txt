add_library(${CLIENT_NETWORK} STATIC "")

target_sources(${CLIENT_NETWORK} PRIVATE
    "src/websocket_client_session.cpp"
)

set(PUBLIC_LINK_LIBRARIES
    ${Boost_LIBRARIES}

    # "Boost::system"
)

set(PRIVATE_LINK_LIBRARIES
    "${NETWORK_ADAPTER}"
)
if (WIN32)
    set(PRIVATE_LINK_LIBRARIES "${PRIVATE_LINK_LIBRARIES}"
        "wsock32"
        "ws2_32"
    )
endif()

target_link_libraries(${CLIENT_NETWORK} PUBLIC ${PUBLIC_LINK_LIBRARIES})
target_link_libraries(${CLIENT_NETWORK} PRIVATE ${PRIVATE_LINK_LIBRARIES})

set(PUBLIC_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "${UTILS_DIR}/network_adapter/include"
    "include"

    # TODO (a.novak): same as utils/network_adapter/CMakeLists.txt
    "${Boost_INCLUDE_DIRS}"
)

target_include_directories(${CLIENT_NETWORK} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
