add_library(memory
  base_memory.hpp
  simple_memory.hpp
  simple_memory.cpp
  blocked_memory.hpp
  blocked_memory.cpp
  typed_heap_memory.hpp
  typed_heap_memory.cpp
  simple_heap_memory.hpp
  simple_heap_memory.cpp
  )

include_directories(../utils)

