// File Name:     memory/allocator/mallocator.cpp
// Author:        Arash Fatehi
// Date:          13th Mar 2021
// Description:   A simple Allocator class using malloc and free for testing purposes

#include "memory/allocator/mallocator.hpp"

#include "utils/mntexcept.hpp"

#include <cstdlib>

using namespace mnt;

void* Mallocator::Allocate(size_t _size)
{
  void* memptr = malloc(_size);

  if(!memptr)
    MNT_THROW("failed to allocate memory using malloc");

  return memptr;
};

void Mallocator::Deallocate(void* _memory) noexcept
{
  free (_memory);
};
