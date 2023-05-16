add_library(${AUTH} STATIC "")

target_sources(${AUTH} PRIVATE
        "src/temp.cpp"
        "src/AuthModel.cpp"
        )

target_include_directories(${AUTH} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
