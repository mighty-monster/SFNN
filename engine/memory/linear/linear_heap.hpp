// File Name:     linear_heap.hpp
// Author:        Arash Fatehi
// Date:          27th Feb 2021
// Description:   Linear heap memory class
// ---------------------

// ---------------------
// Detail Description:
// An implementation of LinearMemory abstract class, in order to allocate enough memory,
// it needs a type as template argument in addition to it`s length and uses "new" and "delete"
// ---------------------

// =====
// [LoadFromFile(p_file_path)]: Load content of memory from a binary file
// =====

// =====
// [Resize(p_length)]: Allocates new memory, copies the content and deallocates old one
// It is a heavy operation, should be avoided unless necessary or harmless
// =====


#ifndef MEMORY_LINEAR_HEAP_HPP
#define MEMORY_LINEAR_HEAP_HPP

#include "memory/linear/linear.hpp"
#include "memory/linear/linear.cpp"

#include <cstddef>
#include <cstdint>

namespace mnt {
  template <typename T>
  class LinearHeapMemory : public LinearMemory<T>
  {
  public:
    LinearHeapMemory() = default;
    LinearHeapMemory(const char* p_file_path);
    LinearHeapMemory(const size_t p_length);
    ~LinearHeapMemory() noexcept;

    void LoadFromFile(const char* p_file_path) override;
    void Resize(const size_t p_length) override;

  protected:
    void Allocate(const size_t p_length);
    void Deallocate() noexcept;

  };
}

#endif


