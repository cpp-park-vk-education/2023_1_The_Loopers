add_library(${DRAW} STATIC "")

target_sources(${DRAW} PRIVATE
        "src/temp.cpp"
        "src/DrawView.cpp"
        )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        "itool/include"
        "tool_box/include"
        )

target_include_directories(${DRAW} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})
