# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 17)
project ("database")

enable_testing()

# Include sub-projects.
add_subdirectory ("database")
add_subdirectory ("tests")
add_subdirectory("src")
add_subdirectory("examples")