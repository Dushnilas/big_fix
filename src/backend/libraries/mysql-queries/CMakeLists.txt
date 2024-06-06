cmake_minimum_required(VERSION 3.28)

project(mysql-queries)

# Add Python library
set(PYTHON_LIB /opt/homebrew/Cellar/python@3.12/3.12.3/Frameworks/Python.framework/Versions/3.12/lib/libpython3.12.dylib)

# Add source files
add_library(mysql-queries STATIC
        mysql-queries.cpp
        mysql-queries.h
        library.py
)

# Link Python library
target_link_libraries(mysql-queries PRIVATE ${PYTHON_LIB})

# Ensure the include directories are available
target_include_directories(mysql-queries PUBLIC
        /opt/homebrew/Cellar/python@3.12/3.12.3/Frameworks/Python.framework/Versions/3.12/include/python3.12
)
