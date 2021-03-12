#ifndef MEMORY_MALLOCATOR_HPP
#define MEMORY_MALLOCATOR_HPP

#include "blueprint.hpp"

namespace mnt {

  class Mallocator : public Allocator
  {
  public:
    Mallocator() = default;
    virtual ~Mallocator() noexcept = default;

    virtual void* Allocate(size_t p_size) override;
    virtual void Deallocate(void* p_memory) noexcept override;
  };
}

#endif
