cmake_minimum_required(VERSION 3.28)

project(Mase_Cinema_Project VERSION 0.1 LANGUAGES CXX)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories for Qt
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Add subdirectories for backend libraries
add_subdirectory(src/qt/libraries/Classes)
add_subdirectory(src/qt/libraries/mysql-queries)

# Include directories
include_directories(src/qt/libraries/Classes)
include_directories(src/qt/libraries/mysql-queries)

# Link directories for Python
include_directories(/opt/homebrew/Cellar/python@3.12/3.12.3/Frameworks/Python.framework/Versions/3.12/include/python3.12)
link_directories(/opt/homebrew/Cellar/python@3.12/3.12.3/Frameworks/Python.framework/Versions/3.12/lib)

# Add the executable for the Qt project
add_executable(Mase_Cinema_Project
        src/qt/main.cpp
        src/qt/mainwindow.cpp
        src/qt/signinwindow.cpp
        src/qt/signupwindow.cpp
        src/qt/backend.cpp
        src/qt/movieswindow.cpp
        src/qt/userprofilewindow.cpp
        src/qt/moviewidget.cpp
)

# Ensure the include directories are available
target_include_directories(Mase_Cinema_Project PUBLIC
        src/qt/libraries/Classes
        src/qt/libraries/mysql-queries
        ${Qt6Core_INCLUDE_DIRS}
        ${Qt6Widgets_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(Mase_Cinema_Project
        Qt6::Widgets
        Qt6::Core
        Qt6::Gui
        libClasses
        mysql-queries
        /opt/homebrew/Cellar/python@3.12/3.12.3/Frameworks/Python.framework/Versions/3.12/lib/libpython3.12.dylib
)

# Ensure libClasses and mysql-queries link against Qt
target_link_libraries(libClasses
        Qt6::Core
        Qt6::Widgets
)

target_link_libraries(mysql-queries
        Qt6::Core
        Qt6::Widgets
)
