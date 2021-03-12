cmake_minimum_required(VERSION 3.5)

project(Momentum LANGUAGES CXX)

add_subdirectory(gui)
add_subdirectory(cmd)
add_subdirectory(api)
add_subdirectory(engine)
add_subdirectory(utils)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


IF (UNIX)
  set( CMAKE_CXX_FLAGS " -pthread" )
ENDIF()

add_executable(${PROJECT_NAME}
  mnt.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE engine utils api gui cmd)

include_directories( ./ engine )


add_executable(MemTest
  engine/memory/mem_test.cpp)

target_link_libraries(MemTest PRIVATE engine utils)


