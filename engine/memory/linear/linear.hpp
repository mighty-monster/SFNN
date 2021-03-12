// File Name:     memory/linear/linear.hpp
// Author:        Arash Fatehi
// Date:          26th Feb 2021
// Description:   A base class for simple linear memory classes

// ---------------------
// Detail Description:
// Simple memory class has a linear structure,  contains a single pointer with allocated memory to it,
// acts as the base class for any custom memory class that uses a user-defined Allocator,
// and Deallocator with a linear structure. Also, the MemoryViewer classes (probably?) derive from it.
// ---------------------

// ---------------------
// Note:
// "SaveToFile" and "LoadFromFile" functions work with memory ignoring the ISA`s endiamntss, they dump and load
// memory as it is. So we can`t transfer files between computers with different endiamntss
// ---------------------

// ---------------------
// Note:
// "GetAsType" and "SetAsType" can interpret memory as variables with unaligned memory,
// it can decrease the performance and can cause race condition problems, so be carefull
// ---------------------

// =====
// [SaveToFile(p_file_path)]: Save content of memory to a binary file, it is not a serrialization function
// and doesn`t save the entire class, only the content that memory class refers to
// =====

// =====
// [GetAsType(p_index)]: Returns a reference to the content of memory
// from p_index to p_index + sizeof(U) interpreted as type U
// =====

// =====
// [SetAsType(p_index, p_value)]: Copies the p_value to memory from p_index to p_index + sizeof(U)
// as it was a vairiable type U
// =====

#ifndef MEMORY_LINEAR_HPP
#define MEMORY_LINEAR_HPP

#include "memory/memory.hpp"
#include "memory/memory.cpp"
#include "memory/allocator/blueprint.hpp"

#include <string>

namespace mnt {
  template <typename T>
  class LinearMemory : public MNTMemory<T>
  {
  public:

    void SaveToFile(const char* p_file_path);
    virtual void LoadFromFile(const char* p_file_path) = 0;

    template<typename U>
    U& GetAsType(const size_t p_index) noexcept;

    template<typename U>
    void SetAsType(const size_t p_index, const U& p_value) noexcept;

    virtual T& operator [] (const size_t p_index) noexcept = 0;
    virtual const T& operator [] (const size_t p_index) const noexcept = 0;

  protected:
    LinearMemory(Allocator* p_allocator = nullptr);
    virtual ~LinearMemory() noexcept = default;

    void* m_memory;
  };
}

#endif
