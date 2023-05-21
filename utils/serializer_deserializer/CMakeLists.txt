add_library(${SERIALIZER} STATIC "")

target_sources(${SERIALIZER} PRIVATE
    "src/trash.cpp"
)

set(PUBLIC_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "include"
)

target_include_directories(${SERIALIZER} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

find_package(nlohmann_json 3.2.0)

if(${jsoncpp_FOUND})
else()
    include(FetchContent)

    FetchContent_Declare(json URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.zip)
    FetchContent_MakeAvailable(json)
endif()

set(PRIVATE_LINK_LIBRARIES ${PRIVATE_LINK_LIBRARIES}
    "nlohmann_json::nlohmann_json"
)

target_link_libraries(${BASE_SERVICE_CHASSIS} PRIVATE ${PRIVATE_LINK_LIBRARIES})
