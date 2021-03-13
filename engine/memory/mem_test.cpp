
#include "utils/memon.hpp"
#include "utils/general.hpp"
#include "utils/general.cpp"
#include "memory/memory.hpp"
#include "memory/linear/linear_heap.hpp"
#include "memory/linear/linear_heap.cpp"
#include "memory/block/block.hpp"
#include "memory/block/block.cpp"
#include "memory/block/block_heap.hpp"
#include "memory/block/block_heap.cpp"
#include "memory/allocator/mallocator.hpp"



int main()
{
  mnt::Logger::Init(Logger::LevelDebug, true, false);


  {
    MNT_PRINT_DUR;
    mnt::Mallocator mallocator;

    mnt::MNTMemory<float>* mem;
    mem = new LinearHeapMemory<float>(1024*1024*128);

    {
      MNT_PRINT_DUR;
      for (size_t i=0; i<mem[0].Length(); i++)
        mem[0][i] = 3.3f*i;

      std::cout<<"\nWriting time:\n";
    }

    {
      MNT_PRINT_DUR;
      for (size_t i=0; i<mem[0].Length(); i++)
        assert(mem[0][i] == 3.3f*i);
      std::cout<<"\nAssert time:\n";
    }
    delete mem;

    std::cout<<"\nOveral time:\n";
  }

  PRINT_ALLOC_SUMMERY

  return 0;
}
