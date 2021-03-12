
#include "mallocator.hpp"

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
