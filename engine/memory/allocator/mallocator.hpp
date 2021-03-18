// File Name:     mallocator.hpp
// Author:        Arash Fatehi
// Date:          13th Mar 2021
// Description:   A simple Allocator class using malloc and free for testing purposes

#ifndef MEMORY_MALLOCATOR_HPP
#define MEMORY_MALLOCATOR_HPP

#include "memory/allocator/blueprint.hpp"

namespace mnt {

  class Mallocator : public Allocator
  {
  public:
    Mallocator() = default;
    virtual ~Mallocator() noexcept = default;

    virtual void* Allocate(size_t _size) override;
    virtual void Deallocate(void* _memory) noexcept override;
  };
}

#endif
