include_directories(${CMAKE_SOURCE_DIR}/../../libraries/mysql-queries)
include_directories(/opt/homebrew/Cellar/python@3.12/3.12.3/Frameworks/Python.framework/Versions/3.12/include/python3.12)

link_directories(/opt/homebrew/Cellar/python@3.12/3.12.3/Frameworks/Python.framework/Versions/3.12/lib)

add_executable(main main.cpp)

target_link_libraries(main user_and_dev)
target_link_libraries(main movies)
target_link_libraries(main logger)

target_link_libraries(main mysql-queries python3.12)
