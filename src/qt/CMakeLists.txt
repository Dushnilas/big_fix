# Add the executable for the Qt project
add_executable(Mase_Cinema_Project
        main.cpp
        signinwindow.cpp
        movieswindow.cpp
        backend.cpp
)

# Ensure the include directories are available
target_include_directories(Mase_Cinema_Project PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/Classes
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/mysql-queries
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/Classes/logger
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/Classes/movie
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/Classes/users
)

# Link libraries
target_link_libraries(Mase_Cinema_Project PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        libClasses
        mysql-queries
        /opt/homebrew/Cellar/python@3.12/3.12.3/Frameworks/Python.framework/Versions/3.12/lib/libpython3.12.dylib
)
