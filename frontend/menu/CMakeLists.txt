add_library(${MENU} SHARED "")

target_sources(${MENU} PRIVATE
        "src/temp.cpp"
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${MENU} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
