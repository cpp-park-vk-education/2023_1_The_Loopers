add_library(${TOKEN_GENERATOR} STATIC "")

target_sources(${TOKEN_GENERATOR} PRIVATE
    "src/trash.cpp"
)


# set(PRIVATE_INCLUDE_DIRECTORIES
# "${UTILS_DIR}/include"
# )

set(PUBLIC_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "include"
)

target_include_directories(${TOKEN_GENERATOR} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

# depends on some crypto library/ jwt token library

# find_package(spdlog)

# if(${spdlog_FOUND})
#     set(PRIVATE_LINK_LIBRARIES "spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>")
# else()
#     include(FetchContent)
# #https://github.com/gabime/spdlog/releases/download/v1.11.0/spdlog.tar.xz
#     FetchContent_Declare(spdlog URL https://github.com/gabime/spdlog/archive/refs/tags/v1.11.0.zip)
#     FetchContent_MakeAvailable(spdlog)

#     set(PRIVATE_LINK_LIBRARIES "spdlog::spdlog_header_only")
# endif()

# set(PRIVATE_LINK_LIBRARIES ${PRIVATE_LINK_LIBRARIES}
#     # "nlohmann_json::nlohmann_json"
# )

# target_link_libraries(${BASE_SERVICE_CHASSIS} PRIVATE ${PRIVATE_LINK_LIBRARIES})
