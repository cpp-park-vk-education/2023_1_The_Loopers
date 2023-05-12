set(ENV{NETWORK_ADAPTER} ${PROJECT_NAME}_network_adapter)

add_library($ENV{NETWORK_ADAPTER} SHARED "")

target_sources($ENV{NETWORK_ADAPTER} PRIVATE
    "src/trash.cpp"
)

set(PUBLIC_LINK_LIBRARIES
    ${Boost_LIBRARIES}

    # "Boost::system"
    # "Boost::beast"
    # "Boost::asio"
)

target_link_libraries($ENV{NETWORK_ADAPTER} PUBLIC ${PUBLIC_LINK_LIBRARIES})

set(PUBLIC_INCLUDE_DIRECTORIES
    "include"

    # TODO (a.novak): because boost::system::error_code.hpp and I don't know how to add only
    # boost::system::error_code.hpp include folder such that it will be available through full path
    # (boost/system/error_code.hpp)
    # P.S.
    # I think it's unnecessary to use some kind of custom error codes for system errors:
    # if there are library that uses different error codes, it's possible to convert them to boosts
    # error_code
    "${Boost_INCLUDE_DIRS}"
)

target_include_directories($ENV{NETWORK_ADAPTER} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
