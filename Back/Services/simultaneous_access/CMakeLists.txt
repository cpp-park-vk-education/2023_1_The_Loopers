set(SIMULTANEOUS_ACCESS_SERVICE service_${PROJECT_NAME}_simultaneous_access)

add_executable(${SIMULTANEOUS_ACCESS_SERVICE} "")

target_sources(${SIMULTANEOUS_ACCESS_SERVICE}
PRIVATE
    "src/main.cpp"
)

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
    "$ENV{EXTERNAL_SERVICE_CHASSISS}"
    "$ENV{CHASSISS_CONFIGURATOR}"
)

target_link_libraries(${SIMULTANEOUS_ACCESS_SERVICE} PRIVATE ${PRIVATE_LINK_LIBRARIES})
