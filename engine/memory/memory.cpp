// File Name:     memory/memory.hpp
// Author:        Arash Fatehi
// Date:          12th Mar 2021
// Description:   The base class for memory subsystem of engine

#ifndef MEMORY_MEMORY_CPP
#define MEMORY_MEMORY_CPP

#include "memory/memory.hpp"

using namespace mnt;

template<typename T>
MNTMemory<T>::MNTMemory(Allocator* p_allocator) noexcept
{
  m_allocator = p_allocator;
};

#endif



