#ifndef MEMORY_BLOCK_CPP
#define MEMORY_BLOCK_CPP

#include "memory/block/block.hpp"

#include "utils/general.hpp"

#include <fstream>
#include <sstream>
#include <string>

using namespace mnt;

template <typename T>
BlockMemory<T>::BlockMemory(Allocator* p_allocator): MNTMemory<T> (p_allocator)
{};

template <typename T>
void BlockMemory<T>::SaveToFile(const char* p_file_path)
{
  auto output_file = std::fstream(p_file_path, std::ios::out | std::ios::binary);
  for(size_t i=0; i<m_no_of_blocks; i++)
  {
    void* buffer = *(m_block_array + i);
    output_file.write((char*)buffer, m_block_size);
  }
  output_file.close();
};


template <typename T>
std::string BlockMemory<T>::ToHex()
{
  std::stringstream string_stream;

  for(size_t i=0; i < m_no_of_blocks; i++)
    string_stream << mnt::BufferToHex(*(m_block_array + i), m_block_size);

  return string_stream.str();
};

template <typename T>
template<typename U>
U& BlockMemory<T>::GetAsType(const size_t p_index)
{
  size_t block_index = p_index / m_block_length;
  size_t in_block_index = p_index % m_block_length;

  void* block_address = *(m_block_array + (block_index));
  return *((U*)block_address + (in_block_index));
};

template <typename T>
template<typename U>
void BlockMemory<T>::SetAsType(const size_t p_index, const U& p_value)
{
  size_t block_index = p_index / m_block_length;
  size_t in_block_index = p_index % m_block_length;

  void* block_address = *(m_block_array + (block_index));
  *((U*)block_address + (in_block_index)) = p_value;
};

#endif
