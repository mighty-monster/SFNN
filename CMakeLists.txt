cmake_minimum_required(VERSION 3.5)

project(NNE LANGUAGES CXX)

add_subdirectory(engine)
add_subdirectory(utils)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


IF (UNIX)
  set( CMAKE_CXX_FLAGS " -pthread" )

  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_BUILD_TYPE STREQUAL "Debug")
      #link_libraries("-stdlib=libc++")
      set( CMAKE_CXX_FLAGS " -fsanitize=memory -fno-omit-frame-pointer" )
  ENDIF()

ENDIF()

add_executable(${PROJECT_NAME}
  main.cpp
  nne.hpp
  nne.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE engine utils)

include_directories( ./ engine )


add_executable(MemTest
  engine/memory/mem_test.cpp)

target_link_libraries(MemTest PRIVATE utils )


