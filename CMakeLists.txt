cmake_minimum_required(VERSION 3.5)

project(NNC LANGUAGES CXX)

add_subdirectory(engine)
add_subdirectory(utils)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF (UNIX)
  set( CMAKE_CXX_FLAGS " -pthread" )
ENDIF()

add_executable(${PROJECT_NAME}
  main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE engine utils)

include_directories(./ engine)

