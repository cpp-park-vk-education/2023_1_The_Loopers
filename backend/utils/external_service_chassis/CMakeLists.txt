add_library(${EXTERNAL_SERVICE_CHASSIS} STATIC "")

target_sources(${EXTERNAL_SERVICE_CHASSIS} PRIVATE
    "src/trash.cpp"
)

# set(PRIVATE_INCLUDE_DIRECTORIES
# "${UTILS_DIR}/include"
# )

set(PUBLIC_INCLUDE_DIRECTORIES
    "include"
    "user_exit_handler/include"
)

target_include_directories(${EXTERNAL_SERVICE_CHASSIS} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

set(PUBLIC_LINK_LIBRARIES
    "${BASE_SERVICE_CHASSIS}"
)

target_link_libraries(${EXTERNAL_SERVICE_CHASSIS} PUBLIC ${PUBLIC_LINK_LIBRARIES})
