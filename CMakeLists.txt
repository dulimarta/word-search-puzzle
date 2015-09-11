cmake_minimum_required(VERSION 3.3)
project(WordSearchGenerator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp WordPuzzleGenerator.cpp WordPuzzleGenerator.h WordPuzzleSolver.cpp WordPuzzleSolver.h InfixTree.cpp InfixTree.h)
add_executable(WordSearchGenerator ${SOURCE_FILES})