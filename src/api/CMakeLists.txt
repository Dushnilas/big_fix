include_directories(${CMAKE_SOURCE_DIR}/../../libraries/mysql-queries)
include_directories(/opt/homebrew/Cellar/python@3.12/3.12.3/Frameworks/Python.framework/Versions/3.12/include/python3.12)

link_directories(/opt/homebrew/Cellar/python@3.12/3.12.3/Frameworks/Python.framework/Versions/3.12/lib)

#add_subdirectory(${CMAKE_SOURCE_DIR}/../../libraries/mysql-queries ${CMAKE_BINARY_DIR}/mysql-queries)

add_executable(api main.cpp)

target_link_libraries(api mysql-queries python3.12)
