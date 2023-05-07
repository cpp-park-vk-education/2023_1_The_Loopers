set(ENV{SERVER_NETWORK} ${PROJECT_NAME}_server_network PARENT_SCOPE)

add_library($ENV{SERVER_NETWORK} SHARED "")

target_sources($ENV{SERVER_NETWORK}
PRIVATE
# "src/main.cpp"
# "src/solve_hw1.cpp"
)

set(PUBLIC_LINK_LIBRARIES
    "Boost::system"
)

set(PRIVATE_LINK_LIBRARIES
    "$ENV{NETWORK_ADAPTER}"
)

target_link_libraries($ENV{SERVER_NETWORK} PUBLIC ${PUBLIC_LINK_LIBRARIES})
target_link_libraries($ENV{SERVER_NETWORK} PRIVATE ${PRIVATE_LINK_LIBRARIES})


set(PUBLIC_INCLUDE_DIRECTORIES
    "${UTILS_FOLDER}/include"
    "listener/include"
    "service_session/include"
    "sessions_factory/include"

    # TODO (a.novak): same as utils/network_adapter/CMakeLists.txt
    "${Boost_INCLUDE_DIRS}"
)

target_include_directories($ENV{SERVER_NETWORK} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
