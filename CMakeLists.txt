cmake_minimum_required(VERSION 3.28)
project(minesweeper)

set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -std=c++20")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
#set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")
#option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_executable(minesweeper WIN32 main.cpp)
find_package(SFML COMPONENTS system window graphics audio REQUIRED)
target_link_libraries(minesweeper sfml-window sfml-system sfml-graphics)
#target_compile_features(minesweeper PRIVATE cxx_std_20)
