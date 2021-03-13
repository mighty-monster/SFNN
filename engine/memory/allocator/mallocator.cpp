// File Name:     memory/allocator/mallocator.cpp
// Author:        Arash Fatehi
// Date:          13th Mar 2021
// Description:   A simple Allocator class using malloc and free for testing purposes

#include "memory/allocator/mallocator.hpp"

#include "utils/mntexcept.hpp"

#include <cstdlib>

using namespace mnt;

void* Mallocator::Allocate(size_t p_size)
{
  void* memptr = malloc(p_size);

  if(!memptr)
    MNT_THROW("failed to allocate memory using malloc");

  return memptr;
};

void Mallocator::Deallocate(void* p_memory) noexcept
{
  free (p_memory);
};
