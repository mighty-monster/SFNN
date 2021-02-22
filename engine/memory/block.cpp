#pragma once

#include <string>

#include "block.hpp"

using namespace nnc;


template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
size_t BlockMemory<ALLOC, DEALLOC>::Size()
{
  // "this->" is added because of lookup error when using templates
  return this->m_size;
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
void BlockMemory<ALLOC, DEALLOC>::SaveToFile(const char* file_path)
{
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
void BlockMemory<ALLOC, DEALLOC>::SaveToHexFile(const char* file_path)
{
};


template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
std::string BlockMemory<ALLOC, DEALLOC>::ToHex()
{
  return std::string("");
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
template<typename T>
T BlockMemory<ALLOC, DEALLOC>::GetAsType(const size_t& index)
{
  return nullptr;
};

template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
template<typename T>
void BlockMemory<ALLOC, DEALLOC>::SetAsType(const size_t& index, const T& value)
{

};
