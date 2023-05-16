cmake_minimum_required(VERSION 3.10)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER /bin/clang++)
set(CMAKE_C_COMPILER /bin/clang)

project(    pretzel
    DESCRIPTION "i have no idea what i'm doing"
    LANGUAGES CXX
)

#add_compile_options(-g -fsanitize=address)
#add_link_options(-fsanitize=address)
#i will use it someday

file(GLOB_RECURSE SOURCES src/*.c**)
file(GLOB_RECURSE HEADERS include/*.h**)

add_executable(test ${SOURCES} ${HEADERS})

if(WIN32)
    message("windows not really supported yet, stay tuned")
else()
    #boost 
    find_package(Boost REQUIRED COMPONENTS system thread)
    if(Boost_FOUND)
        include_directories(PUBLIC ${Boost_INCLUDE_DIRS})
        target_link_libraries(test ${Boost_LIBRARIES})
    else()
        message("no boost???")
    endif()
endif()

#vulkan
if(WIN32)
    set(ENV{VULKAN_SDK} "C:/VulkanSDK/1.3.239.0")
endif()
find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
    message("vulkan found")
    include_directories(PUBLIC ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(test Vulkan::Vulkan)
else()
    message("no vulkan???")
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if(WIN32)
    add_subdirectory("libs/glfw")
    target_link_libraries(test glfw)
else()
    find_package(glfw3 REQUIRED)
    if(glfw3_FOUND)
        target_link_libraries(test glfw)
    else()
        message("no glfw???")
    endif()
endif()

set(GLM_DIR "libs/glm")
add_subdirectory(${GLM_DIR})
target_link_libraries(test glm::glm)