# CMake minimum version required

cmake_minimum_required(VERSION 3.10)



# Project name

project(CHESS)



# Add the executable

add_executable(chess.o main.cpp pieces.cpp player.cpp board.cpp game.cpp)



# Include this directory for header files

target_include_directories(chess.o PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})