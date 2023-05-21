add_executable(${SERVICE_REGISTRY} "")

target_sources(${SERVICE_REGISTRY}
PRIVATE
    "src/main.cpp"
    "src/service_registry.cpp"
)

set(PRIVATE_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    # "protocol_decoder/include"
    "include"
)

target_include_directories(${SERVICE_REGISTRY} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
    "${EXTERNAL_SERVICE_CHASSIS}"
    "${CHASSIS_CONFIGURATOR}"
)

target_link_libraries(${SERVICE_REGISTRY} PRIVATE ${PRIVATE_LINK_LIBRARIES})
