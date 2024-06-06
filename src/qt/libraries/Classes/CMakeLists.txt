# src/qt/libraries/Classes/CMakeLists.txt
add_library(libClasses STATIC
        logger/logger.cpp
        movie/movies.cpp
        users/user_and_dev.cpp
)

target_include_directories(libClasses PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/logger
        ${CMAKE_CURRENT_SOURCE_DIR}/movie
        ${CMAKE_CURRENT_SOURCE_DIR}/users
)
