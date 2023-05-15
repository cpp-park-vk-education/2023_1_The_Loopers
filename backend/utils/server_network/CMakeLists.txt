add_library(${SERVER_NETWORK} STATIC "")

target_sources(${SERVER_NETWORK} PRIVATE
    "service_session/src/iservice_session.cpp"
    "sessions_factory/src/isessions_factory.cpp"
)

set(PUBLIC_LINK_LIBRARIES
    ${Boost_LIBRARIES}

    # "Boost::system"
)

set(PRIVATE_LINK_LIBRARIES
    "${NETWORK_ADAPTER}"
)

target_link_libraries(${SERVER_NETWORK} PUBLIC ${PUBLIC_LINK_LIBRARIES})
target_link_libraries(${SERVER_NETWORK} PRIVATE ${PRIVATE_LINK_LIBRARIES})

set(PUBLIC_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "listener/include"
    "service_session/include"
    "sessions_factory/include"

    # TODO (a.novak): same as utils/network_adapter/CMakeLists.txt
    "${Boost_INCLUDE_DIRS}"
)

target_include_directories(${SERVER_NETWORK} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
