cmake_minimum_required(VERSION 3.28)

project(libClasses)

# Add source files
add_library(libClasses STATIC
        logger/logger.cpp
        logger/logger.h
        movie/movies.cpp
        movie/movies.h
        users/user_and_dev.cpp
        users/user_and_dev.h
)

# Ensure the include directories are available
target_include_directories(libClasses PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
