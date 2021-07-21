// File Name:     linear.inl
// Author:        Arash Fatehi
// Date:          26th Feb 2021
// Description:   A base class for simple linear memory classes

#ifndef ENGINE_MEMORY_LINEAR_INL
#define ENGINE_MEMORY_LINEAR_INL

#include "memory/linear/linear.hpp"

#include "utils/general.hpp"
#include "utils/mntexcept.hpp"

#include <fstream>
#include <string>
#include <iostream>
#include <cstring>

using namespace mnt;

template <typename T>
LinearMemory<T>::LinearMemory(Allocator* _allocator): MNTMemory<T> (_allocator)
{};

// =====
// Note:
// If encounters access violation errors, exceptions are not the way to handle
// the error, so "operator []"s are defined noexcept
// because they do not throw exceptions, same is true for "GetAsType"
// =====

template <typename T>
T& LinearMemory<T> ::operator [] (const size_t _index) noexcept
{
  return *((T*)(this->m_memory) + _index);
};

template <typename T>
const T& LinearMemory<T> ::operator [] (const size_t _index) const noexcept
{
  return *((T*)(this->m_memory) + _index);
};

template <typename T>
void LinearMemory<T>::Write(const size_t _offset, const void* _buffer,const size_t _buffer_length)
{
  if (_offset + _buffer_length >= this->m_length)
    MNT_THROW("This memory object doesn`t have enought memory for this operation");
  memcpy((T*)m_memory + _offset, _buffer, _buffer_length);
};

// Provides strong exception safety
template <typename T>
template<typename U>
U& LinearMemory<T>::GetAsType(const size_t _index)
{
  if (this->m_size - _index < sizeof (U))
    MNT_THROW("Accessing unallocated memory");

  // Caution: be aware of possible unaligned memory access
  return *(U*)((char*)m_memory + _index);
};

// Provides strong exception safety, if assignment operation of type U provides basic exception safety
template <typename T>
template<typename U>
void LinearMemory<T>::SetAsType(const size_t _index, const U& value)
{
  if (this->m_size - _index < sizeof (U))
    MNT_THROW("Accessing unallocated memory");

  // Caution: be aware of possible unaligned memory access
  *(U*)((char*)m_memory + _index) = value;
};

// Provides strong exception safety
template <typename T>
void LinearMemory<T>::SaveToFile(const char* _file_path)
{
  auto output_file = std::fstream(_file_path, std::ios::out | std::ios::binary);

  if (output_file.fail())
    MNT_THROW_C("file path is not valid or filesystem error", errno);

  output_file.write((char*)m_memory, this->m_size);

  if (output_file.fail())
  {
    output_file.close();
    MNT_THROW_C("failed to write content to the file", errno);
  }

  output_file.close();
};

#endif
