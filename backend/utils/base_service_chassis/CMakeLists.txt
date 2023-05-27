add_library(${BASE_SERVICE_CHASSIS} STATIC "")

target_sources(${BASE_SERVICE_CHASSIS} PRIVATE
    "src/trash.cpp"
    "message_broker_connector/src/icommon_connection.cpp"
    "message_broker_connector/src/websocket_common_connection.cpp"
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

target_include_directories(${BASE_SERVICE_CHASSIS} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

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

target_link_libraries(${BASE_SERVICE_CHASSIS} PRIVATE ${PRIVATE_LINK_LIBRARIES})

set(PUBLIC_LINK_LIBRARIES 
    "${SERVER_NETWORK}"
    "${CLIENT_NETWORK}"
)

target_link_libraries(${BASE_SERVICE_CHASSIS} PUBLIC ${PUBLIC_LINK_LIBRARIES})
