// File Name:     simple.hpp
// Author:        Arash Fatehi
// Date:          26th Feb 2021
// Description:   A base class for simple linear memory classes

#ifndef MEMORY_LINEAR_CPP
#define MEMORY_LINEAR_CPP

#include "memory/linear/linear.hpp"

#include "utils/general.hpp"
#include "utils/mntexcept.hpp"

#include <fstream>
#include <string>
#include <iostream>

using namespace mnt;

template <typename T>
LinearMemory<T>::LinearMemory(Allocator* p_allocator): MNTMemory<T> (p_allocator)
{};

// =====
// Note:
// If encounters access violation errors, exceptions are not the way to handle
// the error, so "operator []"s are defined noexcept
// because they do not throw exceptions, same is true for "GetAsType"
// =====

template <typename T>
T& LinearMemory<T> ::operator [] (const size_t p_index) noexcept
{
  return *((T*)(this->m_memory) + p_index);
};

template <typename T>
const T& LinearMemory<T> ::operator [] (const size_t p_index) const noexcept
{
  return *((T*)(this->m_memory) + p_index);
};

// Provides strong exception safety
template <typename T>
template<typename U>
U& LinearMemory<T>::GetAsType(const size_t p_index)
{
  if (this->m_size - p_index < sizeof (U))
    MNT_THROW("Accessing unallocated memory");

  // Caution: be aware of possible unaligned memory access
  return *(U*)((char*)m_memory + p_index);
};

// Provides strong exception safety, if assignment operation of type U provides basic exception safety
template <typename T>
template<typename U>
void LinearMemory<T>::SetAsType(const size_t p_index, const U& value)
{
  if (this->m_size - p_index < sizeof (U))
    MNT_THROW("Accessing unallocated memory");

  // Caution: be aware of possible unaligned memory access
  *(U*)((char*)m_memory + p_index) = value;
};

// Provides strong exception safety
template <typename T>
void LinearMemory<T>::SaveToFile(const char* p_file_path)
{
  auto output_file = std::fstream(p_file_path, std::ios::out | std::ios::binary);

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
