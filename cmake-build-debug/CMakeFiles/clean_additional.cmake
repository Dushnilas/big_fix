# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Mase_Cinema_Project_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Mase_Cinema_Project_autogen.dir/ParseCache.txt"
  "Mase_Cinema_Project_autogen"
  "src/qt/libraries/Classes/CMakeFiles/libClasses_autogen.dir/AutogenUsed.txt"
  "src/qt/libraries/Classes/CMakeFiles/libClasses_autogen.dir/ParseCache.txt"
  "src/qt/libraries/Classes/libClasses_autogen"
  "src/qt/libraries/mysql-queries/CMakeFiles/mysql-queries_autogen.dir/AutogenUsed.txt"
  "src/qt/libraries/mysql-queries/CMakeFiles/mysql-queries_autogen.dir/ParseCache.txt"
  "src/qt/libraries/mysql-queries/mysql-queries_autogen"
  )
endif()
