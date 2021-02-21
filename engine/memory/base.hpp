#pragma once

#include <string>

#include "model.hpp"

typedef void* (ALLOCATOR)(size_t);
typedef void (DEALLOCATOR)(void*);

namespace nnc {
  template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
  class BaseMemory
  {
  public:
    virtual void Resize(const size_t& length) = 0;
    virtual size_t Size() = 0;

  protected:
    virtual ~BaseMemory() {};
    size_t m_size = 0;
    bool m_allocated = false;
  };
}


