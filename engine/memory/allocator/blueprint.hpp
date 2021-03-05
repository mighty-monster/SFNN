#pragma once

#include <cstddef>
#include <stdlib.h>

namespace nne {

  class Allocator
  {
  public:
    virtual void* Allocate(size_t p_size) = 0;
    virtual void Deallocate(void* p_memory) noexcept = 0;

    Allocator() = default;
    virtual ~Allocator() noexcept = default;
  };
}

