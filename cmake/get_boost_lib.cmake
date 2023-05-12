# find third party libraries (boost:: beast, asio, system)
find_package(Boost 1.74)

if(${Boost_FOUND})
else()
    include(FetchContent)
    FetchContent_Declare(boost URL https://github.com/boostorg/boost/releases/download/boost-1.82.0/boost-1.82.0.tar.xz)
    FetchContent_MakeAvailable(boost)

    if(${Boost_INCLUDE_DIRS} STREQUAL "Boost_INCLUDE_DIR-NOTFOUND")
        set(Boost_INCLUDE_DIRS "${PROJECT_BINARY_DIR}/_deps/boost-build/libs")
    endif()

    set(Boost_LIBRARIES
        "Boost::system"
        "Boost::asio"
        "Boost::beast"
    )
endif()
