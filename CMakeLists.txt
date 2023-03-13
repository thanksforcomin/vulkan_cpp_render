cmake_minimum_required(VERSION 3.10)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /bin/clang++)
set(CMAKE_C_COMPILER /bin/clang)
set(CMAKE_C_FLAGS "-std=c++17 -O2")

project(    pretzel
    DESCRIPTION "i have no idea what i'm doing"
    LANGUAGES CXX
)

file(GLOB_RECURSE SOURCES src/*.c**)
file(GLOB_RECURSE HEADERS include/*.h**)

add_executable(test ${SOURCES} ${HEADERS})

find_package(Boost REQUIRED COMPONENTS system thread)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(test ${Boost_LIBRARIES})
endif()