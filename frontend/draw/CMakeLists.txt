add_library(${DRAW} SHARED "")

target_sources(${DRAW} PRIVATE
        "src/temp.cpp"
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        "itool/include"
        "tool_box/include"
        )

target_include_directories(${DRAW} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
