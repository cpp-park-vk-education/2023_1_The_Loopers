add_executable(${STORAGE_SERVICE} "")

target_sources(${STORAGE_SERVICE}
PRIVATE
    "src/main.cpp"
    "src/file_holder.cpp"
	"src/read_file.cpp"
	"src/save_changes.cpp"
	"src/storage.cpp"
)

set(PRIVATE_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "include"
)

target_include_directories(${STORAGE_SERVICE} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
    "${DB_ADAPTER}"
    "${EXTERNAL_SERVICE_CHASSIS}"
    "${CHASSIS_CONFIGURATOR}"
	"${SERIALIZER}"
)

target_link_libraries(${STORAGE_SERVICE} PRIVATE ${PRIVATE_LINK_LIBRARIES})