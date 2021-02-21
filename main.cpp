#include <iostream>
#include <immintrin.h>
#include <memory>

#include "utils/logger.hpp"
#include "utils/timer.hpp"
#include "utils/memonitor.hpp"

#include "memory/simple_heap.hpp"
#include "memory/simple_heap.cpp"

int main()
{
  {
    nnc::Logger::Init(nnc::Logger::LevelDebug, true, false);

    ENABLE_PRINT_ALLOC
    {
      auto mem = new nnc::SimpleHeapMemory<int>(128);
      delete mem;
    }
    DISABLE_PRINT_ALLOC
  }

  PRINT_ALLOC_SUMMERY

  return 0;
}
