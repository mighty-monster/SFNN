cmake_minimum_required(VERSION 3.5)

project(Momentum LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF (UNIX)
  set( CMAKE_CXX_FLAGS " -pthread" )
ENDIF()

include_directories( ./ engine )

add_executable(${PROJECT_NAME}
    mnt.cpp)

add_executable(VlkTest
  vulkan/vlk.cpp)

target_link_libraries(VlkTest PRIVATE glfw vulkan)

