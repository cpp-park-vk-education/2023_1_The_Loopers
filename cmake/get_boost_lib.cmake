# find third party libraries (boost:: beast, asio, system)
find_package(Boost)

if(${Boost_FOUND})
else()
    include(FetchContent)
    FetchContent_Declare(boost URL https://github.com/boostorg/boost/releases/download/boost-1.82.0/boost-1.82.0.tar.xz)
    FetchContent_MakeAvailable(boost)
endif()
