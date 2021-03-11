// File Name:     simple.hpp
// Author:        Arash Fatehi
// Date:          26th Feb 2021
// Description:   A base class for simple linear memory classes

#include "memory/linear/linear.hpp"

#include "utils/general.hpp"
#include "utils/mntexcept.hpp"

#include <fstream>
#include <string>
#include <iostream>

using namespace mnt;

template <typename T>
void LinearMemory<T>::SaveToFile(const char* p_file_path)
{
  auto output_file = std::fstream(p_file_path, std::ios::out | std::ios::binary);

  if (output_file.fail())
    MNT_THROW_C("file path is not valid", errno);

  output_file.write((char*)m_memory, this->m_size);

  if (output_file.fail())
  {
    output_file.close();
    MNT_THROW_C("failed to write content to the file", errno);
  }

  output_file.close();
};

// =====
// Note:
// If encounters access violation errors, exceptions are not the way to handle
// the error, so "GetAsType" and "SetAsType" are defined noexcept
// because they do not throw exceptions
// =====

template <typename T>
template<typename U>
U& LinearMemory<T>::GetAsType(const size_t p_index) noexcept
{
  // By casting m_memory to (char*), p_index is the offset from m_memory in bytes
  // and ((char*)m_memory + (p_index)) is the pointer to beginnig of the
  // address that we want to interpret as a variable
  // and finally *(T*) do the casting and dereferencing that pointer
  // Caution: be aware of possible unaligned memory access
  return *(U*)((char*)m_memory + (p_index));
};

template <typename T>
template<typename U>
void LinearMemory<T>::SetAsType(const size_t p_index, const U& value) noexcept
{
  // By casting m_memory to (char*), p_index is the offset from m_memory in bytes
  // and ((char*)m_memory + (p_index)) is the pointer to beginnig of the
  // address that we want to interpret as a variable
  // and finally *(T*) do the casting and dereferencing that pointer
  // Caution: be aware of possible unaligned memory access
  *(U*)((char*)m_memory + (p_index)) = value;
};
