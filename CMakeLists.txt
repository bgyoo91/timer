cmake_minimum_required(VERSION 3.10.2)
project(timer)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED COMPONENTS date_time system thread)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(timer src/main.cpp)
target_link_libraries(timer ${Boost_LIBRARIES})
