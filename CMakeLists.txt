cmake_minimum_required(VERSION 3.1)
project(WordSearch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp WordPuzzleGenerator.cpp WordPuzzleGenerator.h WordPuzzleSolver.cpp WordPuzzleSolver.h SuffixTree.cpp SuffixTree.h)
add_executable(WordSearch ${SOURCE_FILES})