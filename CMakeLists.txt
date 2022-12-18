cmake_minimum_required(VERSION 3.22)
project(target_assembler)

set(CMAKE_CXX_STANDARD 17)

add_executable(target_assembler
  main.cpp
  Target.cpp
  Builder.cpp
  BuildGraph.cpp
  thread_pull.cpp)
