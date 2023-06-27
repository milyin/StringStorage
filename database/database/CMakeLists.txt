# CMakeList.txt : CMake project for database, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (database 
"database_lib.cpp" 
"database_lib.h" 
"../src/Compressor.h" 
"Database.h" 
"../src/BPlusIndex.h" 
"Database.cpp" 
"../src/Compressor.cpp" 
"../src/FileHandler.cpp")

# TODO: Add tests and install targets if needed.
