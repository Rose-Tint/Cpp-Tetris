cmake_minimum_required(VERSION 3.10.2)

project(CppTetris VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

add_executable(CppTetris
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/Screen.cpp
    ${CMAKE_SOURCE_DIR}/Tetris/Shape.cpp
    ${CMAKE_SOURCE_DIR}/Tetris/Tetris.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(CppTetris Threads::Threads)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(CppTetris ${CURSES_LIBRARIES})

target_include_directories(CppTetris PUBLIC ${CMAKE_SOURCE_DIR})

target_compile_options(CppTetris PUBLIC
    "-std=c++17"
    # "-lpthread"
    "-O3"
    "-finline-functions"
    "-Wall"
    "-Wextra"
    "-Werror"
    "-Wno-strict-aliasing"
    "-pedantic-errors"
)
