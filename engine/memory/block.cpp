#include "memory/block.hpp"

#include "utils/general.hpp"

#include <fstream>
#include <sstream>
#include <string>

using namespace nne;

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
inline size_t BlockMemory<ALLOC, DEALLOC>::SizeInBytes()
{
  // "this->" is added because of lookup error when using templates
  return this->m_size;
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
inline uint16_t BlockMemory<ALLOC, DEALLOC>::NoOfBlocks()
{
  // "this->" is added because of lookup error when using templates
  return this->m_no_of_blocks;
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
void BlockMemory<ALLOC, DEALLOC>::SaveToFile(const char* p_file_path)
{
  auto output_file = std::fstream(p_file_path, std::ios::out | std::ios::binary);
  for(size_t i=0; i<m_no_of_blocks; i++)
  {
    void* buffer = *(m_block_array + i);
    output_file.write((char*)buffer, this->m_block_size);
  }
  output_file.close();
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
void BlockMemory<ALLOC, DEALLOC>::SaveToHexFile(const char* p_file_path)
{
  std::string memory_as_hex = ToHex();
  auto output_file = std::fstream(p_file_path, std::ios::out);
  output_file << memory_as_hex;
  output_file.close();
};


template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
std::string BlockMemory<ALLOC, DEALLOC>::ToHex()
{
  std::stringstream string_stream;

  for(size_t i=0; i < m_no_of_blocks; i++)
    string_stream << nne::BufferToHex(*(m_block_array + i), m_block_size);

  return string_stream.str();
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
template<typename T>
T BlockMemory<ALLOC, DEALLOC>::GetAsType(const size_t& p_index)
{
  size_t block_index = p_index / m_block_length;
  size_t in_block_index = p_index % m_block_length;

  void* block_address = *(m_block_array + (block_index));
  return *((T*)block_address + (in_block_index));
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
template<typename T>
void BlockMemory<ALLOC, DEALLOC>::SetAsType(const size_t& p_index, const T& p_value)
{
  size_t block_index = p_index / m_block_length;
  size_t in_block_index = p_index % m_block_length;

  void* block_address = *(m_block_array + (block_index));
  *((T*)block_address + (in_block_index)) = p_value;
};
