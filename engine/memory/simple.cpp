#include <fstream>
#include <string>
#include <iostream>

#include "simple.hpp"
#include "utils/general.hpp"

using namespace nnc;

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
size_t SimpleMemory<ALLOC, DEALLOC>::Size()
{
  // "this->" is added because of lookup error when using templates
  return this->m_size;
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
void SimpleMemory<ALLOC, DEALLOC>::SaveToFile(const char* file_path)
{
  auto output_file = std::fstream(file_path, std::ios::out | std::ios::binary);
  output_file.write((char*)m_memory, this->m_size);
  output_file.close();
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
void SimpleMemory<ALLOC, DEALLOC>::SaveToHexFile(const char* file_path)
{
  std::string memory_as_hex = ToHex();
  auto output_file = std::fstream(file_path, std::ios::out);
  output_file << memory_as_hex;
  output_file.close();
};


template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
std::string SimpleMemory<ALLOC, DEALLOC>::ToHex()
{
  return nnc::BufferToHex(m_memory, this->m_size);
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
template<typename T>
T SimpleMemory<ALLOC, DEALLOC>::GetAsType(const size_t& index)
{
  return *((T*)m_memory + (index));
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
template<typename T>
void SimpleMemory<ALLOC, DEALLOC>::SetAsType(const size_t& index, const T& value)
{
  *((T*)m_memory + (index)) = value;
};
