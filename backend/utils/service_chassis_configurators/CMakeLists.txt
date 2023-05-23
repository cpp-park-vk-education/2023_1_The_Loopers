add_library(${CHASSIS_CONFIGURATOR} STATIC "")

target_sources(${CHASSIS_CONFIGURATOR} PRIVATE
    "src/base_websocket_configurator.cpp"
)

set(PUBLIC_INCLUDE_DIRECTORIES
    "include"
    "${UTILS_DIR}/include"
)

target_include_directories(${CHASSIS_CONFIGURATOR} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

set(PUBLIC_LINK_LIBRARIES
    "${BASE_SERVICE_CHASSIS}"
    "${EXTERNAL_SERVICE_CHASSIS}"
)

target_link_libraries(${CHASSIS_CONFIGURATOR} PUBLIC ${PUBLIC_LINK_LIBRARIES})
