// File Name:     simple_heap.hpp
// Author:        Arash Fatehi
// Date:          27th Feb 2021
// Description:   Typed linear heap memory class
// ---------------------
// Detail Description:
// An implementation of SimpleMemory abstract class, allocates using "new" and deallocates
// using "free" operations, it needs a type as template argument in addition to it`s length
// in order to allocate enough memory.

#pragma once

#include "memory/simple.hpp"
#include "memory/simple.cpp"

#include <cstddef>
#include <cstdint>

namespace nne {
  template <typename T>
  class SimpleHeapMemory : public SimpleMemory<nullptr, nullptr>
  {
  public:
    // To create an object without allocated memory
    SimpleHeapMemory();

    // To load the content of memory from a binary file
    SimpleHeapMemory(const char* p_file_path);

    // To allocate memory for an array with length of p_length of type T
    SimpleHeapMemory(const size_t& p_length);

    // Deallocate memory to leverage RAII for memory management
    ~SimpleHeapMemory();

    // Alocates p_length * sizeof(T) bytes of memory to m_memory
    void Allocate(const size_t& p_length) noexcept;

    // Frees the allocated memory if allocated
    void Deallocate();

    // Load content of memory from a binary file
    void LoadFromFile(const char* p_file_path) override;

    // Load content of memory from a hex file
    void LoadFromHexFile(const char* p_file_path) override;

    // Allocates new memory, copies the content and deallocates old one
    // It is a heavy operation, should be avoided unless necessary or harmless
    void Resize(const size_t& p_length) override;

    // Returns the p_index`th item of the array
    T& operator [] (const size_t& p_index);

    // Returns the const version of p_index`th item of the array
    const T& operator [] (const size_t& p_index) const;

    // Return the length of the array
    inline size_t Length();

  private:
    size_t m_length = 0;
  };
}


