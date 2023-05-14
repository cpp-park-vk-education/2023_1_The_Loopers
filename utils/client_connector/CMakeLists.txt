set(ENV{CLIENT_NETWORK} ${PROJECT_NAME}_client_network)

add_library($ENV{CLIENT_NETWORK} SHARED "")

target_sources($ENV{CLIENT_NETWORK} PRIVATE
    "src/websocket_client_session.cpp"
)

set(PUBLIC_LINK_LIBRARIES
    ${Boost_LIBRARIES}

    # "Boost::system"
)

set(PRIVATE_LINK_LIBRARIES
    "$ENV{NETWORK_ADAPTER}"
)

target_link_libraries($ENV{CLIENT_NETWORK} PUBLIC ${PUBLIC_LINK_LIBRARIES})
target_link_libraries($ENV{CLIENT_NETWORK} PRIVATE ${PRIVATE_LINK_LIBRARIES})

set(PUBLIC_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "${UTILS_DIR}/network_adapter/include"
    "include"

    # TODO (a.novak): same as utils/network_adapter/CMakeLists.txt
    "${Boost_INCLUDE_DIRS}"
)

target_include_directories($ENV{CLIENT_NETWORK} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
