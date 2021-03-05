#pragma once

#include "blueprint.hpp"

namespace nne {

  class Mallocator : public Allocator
  {
  public:
    Mallocator();
    virtual ~Mallocator() noexcept;

    virtual void* Allocate(size_t p_size) override;
    virtual void Deallocate(void* p_memory) noexcept override;
  };
}
