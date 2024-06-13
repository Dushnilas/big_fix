# src/qt/libraries/mysql-queries/CMakeLists.txt
add_library(mysql-queries STATIC
        mysql-queries.cpp
        library.py
)

target_include_directories(mysql-queries PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
