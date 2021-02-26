// File Name:     simple.hpp
// Author:        Arash Fatehi
// Date:          26th Feb 2021
// Description:   A base class for simple linear memory classes
// ---------------------
// Detail Description:
// Simple memory class has a linear structure,  contains a single pointer with allocated memory to it,
// acts as the base class for any custom memory class that uses a user-defined Allocator,
// and Deallocator with a linear structure. Also, the MemoryViewer classes probably derive from it.
// ---------------------
// Hint:
// "Save" and "Load" functions work with memory regardless of the ISA`s endianness, they dump and load
// memory as it is. So we can`t transfer files between computers with different endianness
// "GetAsType" and "SetAsType" can interpret memory as variables with unaligned memory, it will decrease
// the performance and can cause race condition problems, so be carefull

#pragma once

#include "memory/base.hpp"

#include <string>

namespace nne {
  template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
  class SimpleMemory : public BaseMemory<ALLOC, DEALLOC>
  {
  public:
    inline size_t SizeInBytes() override;

    // Save content of memory to a binary file, it is not a serrialization function
    // and doesn`t save the entire class, only the content that memory class refers to
    void SaveToFile(const char* p_file_path);

    // Load content of memory from a binary file, should be implemented by the derived class
    // because it depends on it`s implementation
    virtual void LoadFromFile(const char* p_file_path) = 0;

    // Same as "SaveToFile", but Save content of memory to a hex file
    void SaveToHexFile(const char* p_file_path);

    // Same as "LoadFromFile", Load content of memory from a hex file
    virtual void LoadFromHexFile(const char* p_file_path) = 0;

    // Return a string which is hex equivalent of content of memory
    std::string ToHex();

    // Returns a reference to the content of memory
    // from p_index to p_index + sizeof(T) interpreted as type T
    template<typename T>
    T& GetAsType(const size_t& p_index);

    // Copies the p_value to memory from p_index to p_index + sizeof(T) as it was a vairiable type T
    template<typename T>
    void SetAsType(const size_t& p_index, const T& p_value);

  protected:
    SimpleMemory() = default;
    virtual ~SimpleMemory() = default;

    // The pointer to allcoated memory
    void* m_memory;
  };
}
