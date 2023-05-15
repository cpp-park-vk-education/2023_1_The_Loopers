add_library(${SERIALIZER} STATIC "")

target_sources(${SERIALIZER} PRIVATE
    "src/trash.cpp"
)

set(PUBLIC_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "include"
)

target_include_directories(${SERIALIZER} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

# find_package(spdlog)

# if(${spdlog_FOUND})
#     set(PRIVATE_LINK_LIBRARIES "spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>")
# else()
#     include(FetchContent)

#     FetchContent_Declare(spdlog URL https://github.com/gabime/spdlog/archive/refs/tags/v1.11.0.zip)
#     FetchContent_MakeAvailable(spdlog)

#     set(PRIVATE_LINK_LIBRARIES "spdlog::spdlog_header_only")
# endif()

# set(PRIVATE_LINK_LIBRARIES ${PRIVATE_LINK_LIBRARIES}
#     # "nlohmann_json::nlohmann_json"
# )

# target_link_libraries(${BASE_SERVICE_CHASSIS} PRIVATE ${PRIVATE_LINK_LIBRARIES})

# set(PUBLIC_LINK_LIBRARIES 
#     "${SERVER_NETWORK}"
#     "${CLIENT_NETWORK}"
# )

# target_link_libraries(${BASE_SERVICE_CHASSIS} PUBLIC ${PUBLIC_LINK_LIBRARIES})
