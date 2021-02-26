// File Name:     simple.hpp
// Author:        Arash Fatehi
// Date:          26th Feb 2021
// Description:   A base class for simple linear memory classes

#include "memory/simple.hpp"

#include "utils/general.hpp"

#include <fstream>
#include <string>
#include <iostream>

using namespace nne;

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
inline size_t SimpleMemory<ALLOC, DEALLOC>::SizeInBytes()
{
  // "this->" is added because of lookup error when using templates
  return this->m_size;
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
void SimpleMemory<ALLOC, DEALLOC>::SaveToFile(const char* p_file_path)
{
  auto output_file = std::fstream(p_file_path, std::ios::out | std::ios::binary);

  if (output_file.fail())
  {
    NNERORR(!output_file.fail(), "file path is not valid");
    return;
  }

  output_file.write((char*)m_memory, this->m_size);

  if (output_file.fail())
  {
    NNERORR(!output_file.fail(), "failed to write content to file");
    return;
  }

  output_file.close();
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
void SimpleMemory<ALLOC, DEALLOC>::SaveToHexFile(const char* p_file_path)
{
  auto output_file = std::fstream(p_file_path, std::ios::out);

  if (!output_file)
  {
    NNERORR(!output_file.fail(), "file path is not valid");
    return;
  }

  std::string memory_as_hex = ToHex();
  output_file << memory_as_hex;

  if (output_file.fail())
  {
    NNERORR(!output_file.fail(), "failed to write content to file");
    return;
  }

  output_file.close();
};


template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
std::string SimpleMemory<ALLOC, DEALLOC>::ToHex()
{
  return nne::BufferToHex(m_memory, this->m_size);
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
template<typename T>
T& SimpleMemory<ALLOC, DEALLOC>::GetAsType(const size_t& p_index)
{
  // By casting m_memory to (char*), p_index is the offset from m_memory in bytes
  // and ((char*)m_memory + (p_index)) is the pointer to beginnig of the
  // address that we want to interpret as a variable
  // and finally *(T*) do the casting and dereferencing that pointer
  // Caution: be aware of possible unaligned memory access
  return *(T*)((char*)m_memory + (p_index));
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
template<typename T>
void SimpleMemory<ALLOC, DEALLOC>::SetAsType(const size_t& p_index, const T& value)
{
  // By casting m_memory to (char*), p_index is the offset from m_memory in bytes
  // and ((char*)m_memory + (p_index)) is the pointer to beginnig of the
  // address that we want to interpret as a variable
  // and finally *(T*) do the casting and dereferencing that pointer
  // Caution: be aware of possible unaligned memory access
  *(T*)((char*)m_memory + (p_index)) = value;
};
