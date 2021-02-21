add_library(engine
  memory/model.hpp
  memory/base.hpp
  memory/simple.hpp
  memory/simple.cpp
  memory/simple_heap.hpp
  memory/simple_heap.cpp
  memory/block.hpp
  memory/block.cpp
  memory/block_heap.hpp
  memory/block_heap.cpp
  math/tensor.hpp
  math/tensor.cpp
  )

include_directories(./ ../)
