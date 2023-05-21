add_library(${AUTH} STATIC "")

target_sources(${AUTH} PRIVATE
        "src/AuthView.cpp"
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${AUTH} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
