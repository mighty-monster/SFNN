#include <fstream>
#include <string>
#include <iostream>

#include "memory/simple.hpp"
#include "utils/general.hpp"

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
  output_file.write((char*)m_memory, this->m_size);
  output_file.close();
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
void SimpleMemory<ALLOC, DEALLOC>::SaveToHexFile(const char* p_file_path)
{
  std::string memory_as_hex = ToHex();
  auto output_file = std::fstream(p_file_path, std::ios::out);
  output_file << memory_as_hex;
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
  return *(T*)((char*)m_memory + (p_index));
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
template<typename T>
void SimpleMemory<ALLOC, DEALLOC>::SetAsType(const size_t& p_index, const T& value)
{
  *(T*)((char*)m_memory + (p_index)) = value;
};
