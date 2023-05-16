add_executable(${SIMULTANEOUS_ACCESS_SERVICE} "")

set(PRIVATE_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "protocol_decoder/include"
    "resolvers/draw_conflict_resolver/include"
    "resolvers/text_conflict_resolver/include"
    "resolvers_factory/include"
    "include"
)

target_include_directories(${SIMULTANEOUS_ACCESS_SERVICE} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
    "${DB_ADAPTER}"
    "${EXTERNAL_SERVICE_CHASSIS}"
    "${CHASSIS_CONFIGURATOR}"
)

target_link_libraries(${SIMULTANEOUS_ACCESS_SERVICE} PRIVATE ${PRIVATE_LINK_LIBRARIES})

target_sources(${SIMULTANEOUS_ACCESS_SERVICE}
PRIVATE
    "src/main.cpp"
    "src/simultaneous_access_service.cpp"
)
