cmake_minimum_required(VERSION 3.18)
project(LabourDynamics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
find_package(CURL REQUIRED)

include_directories(include ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

file(GLOB SOURCES src/*.cpp)
add_executable(LabourDynamics ${SOURCES})

target_link_libraries(LabourDynamics PRIVATE ${Python3_LIBRARIES} CURL::libcurl)
