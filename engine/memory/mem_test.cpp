#include <vector>
//#include "utils/memon.hpp"
#include "utils/general.hpp"
#include "memory/linear/linear_heap.hpp"
#include "memory/linear/linear_heap.cpp"
#include "memory/block/block_heap.hpp"
#include "memory/block/block_heap.cpp"
#include "memory/allocator/mallocator.hpp"

int main()
{
  //mnt::Logger::Init(Logger::LevelDebug, true, true);

  mnt::Mallocator mallocator;

  BlockHeapMemory<float> mem(128, &mallocator);

  for (size_t i=0; i<mem.Length(); i++)
    mem[i] = 1.0f*i;

  for (size_t i=0; i<mem.Length(); i++)
    MNT_PRINTL( mem[i]);

  return 0;
}
