// File Name:     block.inl
// Author:        Arash Fatehi
// Date:          13th Mar 2021
// Description:   A base class for block memory classes

#ifndef ENGINE_MEMORY_BLOCK_INL
#define ENGINE_MEMORY_BLOCK_INL

#include "memory/block/block.hpp"

#include "utils/general.hpp"
#include "utils/mntexcept.hpp"

#include <fstream>
#include <sstream>
#include <string>

using namespace mnt;

template <typename T>
BlockMemory<T>::BlockMemory(Allocator* _allocator): MNTMemory<T> (_allocator)
{};

// =====
// Note:
// If encounters access violation errors, exceptions are not the way to handle
// the error, so "operator []"s are defined noexcept
// because they do not throw exceptions
// =====

template <typename T>
T& BlockMemory<T>::operator [] (const size_t _index) noexcept
{
  // Counting on compiler optimization for removing extra
  // charachter declaration --> better check the assembly later

  size_t block_index = _index / m_block_length;
  size_t block_offset = _index % m_block_length;

  // Same as this->m_block_array[block_index]
  void* block_address = *(m_block_array + block_index);

  // Same as (T*)block_address[block_offset]
  return *((T*)block_address + block_offset);
};

template <typename T>
const T& BlockMemory<T>::operator [] (const size_t _index) const noexcept
{
  size_t block_index = _index / m_block_length;
  size_t block_offset = _index % m_block_length;

  // Same as this->m_block_array[block_index]
  void* block_address = *(m_block_array + block_index);

  // Same as (T*)block_address[block_offset]
  return *((T*)block_address + block_offset);
};

template <typename T>
void BlockMemory<T>::Write(const size_t _offset, const void* _buffer, const size_t _buffer_length)
{
  if (_offset + _buffer_length >= this->m_length)
    MNT_THROW("This memory object doesn`t have enought memory for this operation");

  // This is inefficient - Change the implementation if become necessary
  for(size_t i=_offset; i < _buffer_length; i++)
    this[0][i] = ((T*)_buffer)[0][i];
};

// Provides strong exception safety
template <typename T>
template<typename U>
U& BlockMemory<T>::GetAsType(const size_t _index)
{
  size_t block_index = _index / (m_block_size);
  size_t block_offset = _index % m_block_size;

  if (this->m_block_size - block_offset < sizeof (U))
    MNT_THROW("Accessing unallocated memory");

  void* block_address = *(m_block_array + block_index);

  return *((U*)block_address + block_offset);
};

// Provides strong exception safety, if assignment operation of type U provides basic exception safety
template <typename T>
template<typename U>
void BlockMemory<T>::SetAsType(const size_t _index, const U& _value)
{
  size_t block_index = _index / m_block_size;
  size_t block_offset = _index % m_block_size;

  if (this->m_block_size - block_offset < sizeof (U))
    MNT_THROW("Accessing unallocated memory");

  void* block_address = *(m_block_array + block_index);

  // The assignment operation of type U can throw exceptions
  *((U*)block_address + block_offset) = _value;
};

// Provides strong exception safety
template <typename T>
void BlockMemory<T>::SaveToFile(const char* _file_path)
{
  auto output_file = std::fstream(_file_path, std::ios::out | std::ios::binary);

  if (output_file.fail())
    MNT_THROW_C("file path is not valid or filesystem error", errno);

  for(size_t i=0; i<m_no_of_blocks; i++)
  {
    void* buffer = *(m_block_array + i);
    output_file.write((char*)buffer, m_block_size);
    if (output_file.fail())
    {
      output_file.close();
      MNT_THROW_C("failed to write content to the file", errno);
    }
  }
  output_file.close();
};

#endif
