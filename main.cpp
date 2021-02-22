#include <iostream>
#include <immintrin.h>
#include <memory>

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
      auto mem = new nnc::BlockHeapMemory<int>(2048*1024, 16);

      for (size_t i=0; i < mem->Length(); i++)
        (*mem)[i] = i;

      mem->Reshape(2);
      mem->Reshape(3);
      mem->Reshape(5);
      mem->Reshape(7);
      mem->Reshape(11);
      mem->Reshape(13);
      mem->Reshape(17);
      mem->Reshape(19);
      mem->Reshape(23);
      mem->Reshape(29);
      mem->Reshape(53);
      mem->Reshape(132);
      mem->Reshape(13);
      mem->Reshape(1024);
      mem->Reshape(17);
      mem->Reshape(1);

      for (size_t i=0; i < mem->Length(); i++)
      {
        _ASSERT((*mem)[i] == i);
      }

      delete mem;
    }
     DISABLE_PRINT_ALLOC

  }

  PRINT_ALLOC_SUMMERY

  return 0;
}
