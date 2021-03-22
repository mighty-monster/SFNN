// File Name:     memory/memory.inl
// Author:        Arash Fatehi
// Date:          12th Mar 2021
// Description:   The base class for memory subsystem of engine

#ifndef ENGINE_MEMORY_MEMORY_INL
#define ENGINE_MEMORY_MEMORY_INL

#include "memory/memory.hpp"

using namespace mnt;

template<typename T>
MNTMemory<T>::MNTMemory(Allocator* _allocator) noexcept
{
  m_allocator = _allocator;
};

#endif



