cmake_minimum_required(VERSION 3.26)
project(B_Tree_Index_System)

set(CMAKE_CXX_STANDARD 17)

add_executable(B_Tree_Index_System main.cpp
                CustomFunctions.cpp
                IndexHelper.cpp
                IndexInterface.cpp
                IndexSystem.cpp)
