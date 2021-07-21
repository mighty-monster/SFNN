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
// [LoadFromFile(_file_path)]: Load content of memory from a binary file
// =====

// =====
// [Resize(_length)]: Allocates new memory, copies the content and deallocates old one
// It is a heavy operation, should be avoided unless necessary or harmless
// =====


#ifndef ENGINE_MEMORY_LINEAR_HEA_HPP
#define ENGINE_MEMORY_LINEAR_HEA_HPP

#include "memory/linear/linear.hpp"

#include <cstddef>
#include <cstdint>

namespace mnt {
  template <typename T>
  class LinearHeapMemory : public LinearMemory<T>
  {
  public:
    LinearHeapMemory() = default;
    LinearHeapMemory(const char* _file_path);
    LinearHeapMemory(const size_t _length);
    ~LinearHeapMemory() noexcept;

    void LoadFromFile(const char* _file_path) override;
    void Resize(const size_t _length) override;

  protected:
    void Allocate(const size_t _length);
    void Deallocate() noexcept;

  };
}

#include "memory/linear/linear_heap.inl"

#endif


