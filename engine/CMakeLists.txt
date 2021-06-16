add_library(engine
  configs.hpp
  memory/memory.hpp
  memory/memory.inl
  memory/allocator/blueprint.hpp
  memory/allocator/mallocator.hpp
  memory/allocator/mallocator.cpp
  memory/linear/linear.hpp
  memory/linear/linear.inl
  memory/linear/linear_heap.hpp
  memory/linear/linear_heap.inl
  memory/block/block.hpp
  memory/block/block.inl
  memory/block/block_heap.hpp
  memory/block/block_heap.inl
  math/tensor.hpp
  math/tensor.inl
  math/backend.hpp
  math/backends/default.hpp
  math/backends/default.inl

  )

include_directories(./ ../)
