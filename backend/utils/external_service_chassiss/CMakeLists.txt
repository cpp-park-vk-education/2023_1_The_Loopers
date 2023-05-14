add_library(${EXTERNAL_SERVICE_CHASSISS} SHARED "")

target_sources(${EXTERNAL_SERVICE_CHASSISS} PRIVATE
    "src/trash.cpp"
)

# set(PRIVATE_INCLUDE_DIRECTORIES
# "${UTILS_DIR}/include"
# )

set(PUBLIC_INCLUDE_DIRECTORIES
    "include"
    "user_exit_handler/include"
)

target_include_directories(${EXTERNAL_SERVICE_CHASSISS} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

set(PUBLIC_LINK_LIBRARIES
    "${BASE_SERVICE_CHASSISS}"
)

target_link_libraries(${EXTERNAL_SERVICE_CHASSISS} PUBLIC ${PUBLIC_LINK_LIBRARIES})
