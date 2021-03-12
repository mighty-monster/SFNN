add_library(engine
  memory/memory.hpp
  memory/memory.cpp
  memory/allocator/blueprint.hpp
  memory/allocator/mallocator.hpp
  memory/allocator/mallocator.cpp
  memory/linear/linear.hpp
  memory/linear/linear.cpp
  memory/linear/linear_heap.hpp
  memory/linear/linear_heap.cpp
  memory/block/block.hpp
  memory/block/block.cpp
  memory/block/block_heap.hpp
  memory/block/block_heap.cpp
  math/tensor.hpp
  math/tensor.cpp
  )

include_directories(./ ../)
