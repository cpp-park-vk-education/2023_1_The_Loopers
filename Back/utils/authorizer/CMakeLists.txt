set(ENV{AUTHORIZER} ${PROJECT_NAME}_authorizer)

add_library($ENV{AUTHORIZER} STATIC "")

target_sources($ENV{AUTHORIZER}
PRIVATE
# "src/main.cpp"
# "src/solve_hw1.cpp"
)

set(PUBLIC_INCLUDE_DIRECTORIES
    "${UTILS_FOLDER}/include"
    "include"
    "token_validator/include"
)

target_include_directories($ENV{AUTHORIZER} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

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

# target_link_libraries($ENV{BASE_SERVICE_CHASSISS} PRIVATE ${PRIVATE_LINK_LIBRARIES})
