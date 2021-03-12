
#include "utils/memon.hpp"
#include "utils/general.hpp"
#include "memory/memory.hpp"
#include "memory/linear/linear_heap.hpp"
#include "memory/linear/linear_heap.cpp"
#include "memory/block/block_heap.hpp"
#include "memory/block/block_heap.cpp"
#include "memory/allocator/mallocator.hpp"



int main()
{
  //mnt::Logger::Init(Logger::LevelDebug, true, true);

  {
    mnt::Mallocator mallocator;

    mnt::MNTMemory<float>* mem;
    mem = new BlockHeapMemory<float>(128);

    for (size_t i=0; i<mem[0].Length(); i++)
      mem[0][i] = 1.0f*i;

    for (size_t i=0; i<mem[0].Length(); i++)
      MNT_PRINTL(mem[0][i]);

    delete mem;
  }

  PRINT_ALLOC_SUMMERY

  return 0;
}
