// File Name:     blueprint.hpp
// Author:        Arash Fatehi
// Date:          13th Mar 2021
// Description:   Abstract class that encapsulates allocation and deallocation functions

// ---------------------
// Detail Description:
// Can be a simple malloc, free call, or it can implement a custome memory allcoation
// mechanism with compositional strategies, also it can handle memory allcoation
// and transfer to GPU memory, remote devices, etc.
// ---------------------

#ifndef ENGINE_MEMORY_BLUEPRINT_HPP
#define ENGINE_MEMORY_BLUEPRINT_HPP

#include <cstddef>
#include <stdlib.h>

namespace mnt {

  class Allocator
  {
  public:
    virtual void* Allocate(size_t _size) = 0;
    virtual void Deallocate(void* _memory) noexcept = 0;

    Allocator() = default;
    virtual ~Allocator() noexcept = default;
  };
}

#endif
