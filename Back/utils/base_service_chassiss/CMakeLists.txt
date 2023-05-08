set(ENV{BASE_SERVICE_CHASSISS} ${PROJECT_NAME}_base_service_chassiss)

add_library($ENV{BASE_SERVICE_CHASSISS} SHARED "")

target_sources($ENV{BASE_SERVICE_CHASSISS} PRIVATE
    "src/trash.cpp"
)

set(PUBLIC_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "include"
    "internal_sessions_manager/include"
    "local_service_registry/include"
    "logger_adapter/include"
    "message_broker_connector/include"
    "service_registator/include"
)

target_include_directories($ENV{BASE_SERVICE_CHASSISS} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

find_package(spdlog)

if(${spdlog_FOUND})
    set(PRIVATE_LINK_LIBRARIES "spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>")
else()
    include(FetchContent)

    FetchContent_Declare(spdlog URL https://github.com/gabime/spdlog/archive/refs/tags/v1.11.0.zip)
    FetchContent_MakeAvailable(spdlog)

    set(PRIVATE_LINK_LIBRARIES "spdlog::spdlog_header_only")
endif()

set(PRIVATE_LINK_LIBRARIES ${PRIVATE_LINK_LIBRARIES}
    # "nlohmann_json::nlohmann_json"
)

target_link_libraries($ENV{BASE_SERVICE_CHASSISS} PRIVATE ${PRIVATE_LINK_LIBRARIES})

set(PUBLIC_LINK_LIBRARIES 
    "$ENV{SERVER_NETWORK}"
    "${CLIENT_NETWORK}"
)

target_link_libraries($ENV{BASE_SERVICE_CHASSISS} PUBLIC ${PUBLIC_LINK_LIBRARIES})
