#include <iostream>
#include <immintrin.h>
#include <memory>
#include <assert.h>

#include "utils/logger.hpp"
#include "utils/timer.hpp"
#include "utils/memonitor.hpp"

#include "memory/block_heap.hpp"
#include "memory/block_heap.cpp"

#include "memory/simple_heap.hpp"
#include "memory/simple_heap.cpp"

int main()
{
  {
    nnc::Logger::Init(nnc::Logger::LevelDebug, true, false);

    ENABLE_PRINT_ALLOC
    {
      auto mem = new nnc::BlockHeapMemory<int>(1024, 2);

      for (size_t i=0; i < mem->Length(); i++)
        (*mem)[i] = i;

      mem->SaveToFile("file.bin");
      mem->SaveToHexFile("file.hex");

      nnc::BlockHeapMemory<int> mem2;
      mem2.LoadFromHexFile("file.hex");

      for (size_t i=0; i < mem->Length(); i++)
      {
        assert(mem2[i] == i);
      }

      delete mem;
    }
     DISABLE_PRINT_ALLOC

  }

  PRINT_ALLOC_SUMMERY

  return 0;
}
