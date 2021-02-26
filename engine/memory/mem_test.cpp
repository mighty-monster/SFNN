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
    SimpleHeapMemory<float> mem(1024);

    for (size_t i=0; i<mem.Length(); i++)
      mem[i] = i + 0.5f;

    float ref = mem.GetAsType<float>(8);

    std::cout << ref << std::endl;
    ref = 18.0f;

    std::cout << mem.GetAsType<float>(8) << std::endl;

    std::cout << ref << std::endl;

  }

  DISABLE_PRINT_ALLOC
  PRINT_ALLOC_SUMMERY

  return 0;
}
