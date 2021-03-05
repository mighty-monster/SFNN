#include "mallocator.hpp"

#include <cstdlib>

using namespace nne;

Mallocator::Mallocator() {};
Mallocator::~Mallocator() noexcept {};

void* Mallocator::Allocate(size_t p_size)
{
  return malloc(p_size);
};

void Mallocator::Deallocate(void* p_memory) noexcept
{
  free (p_memory);
};
