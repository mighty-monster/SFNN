#include <iostream>
#include <assert.h>

#include "nne.hpp"
#include "utils/memonitor.hpp"

#include "memory/simple_heap.hpp"
#include "memory/simple_heap.cpp"

#include "memory/block_heap.hpp"
#include "memory/block_heap.cpp"

int main()
{
  nne::Logger::Init(nne::Logger::LevelDebug, true, false);
  ENABLE_PRINT_ALLOC
  {
    BlockHeapMemory<int> mem(1024);
    mem.Resize(11);
  }

  DISABLE_PRINT_ALLOC
  PRINT_ALLOC_SUMMERY

  return 0;
}
