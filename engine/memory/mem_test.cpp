#include <iostream>
#include <assert.h>

#include "nne.hpp"
#include "utils/memon.hpp"
#include "utils/nnexpect.hpp"

#include "memory/simple_heap.hpp"
#include "memory/simple_heap.cpp"

#include "memory/block_heap.hpp"
#include "memory/block_heap.cpp"

#include "utils/general.cpp"


int main()
{
  nne::Logger::Init(nne::Logger::LevelDebug, true, true);
  ENABLE_PRINT_ALLOC
  {
    std::ofstream filestrwa;
    filestrwa.open("./sdfsdf/sdfsd");
    int size = 100;
    char res[100];
    NNE_ERRNO_PRINT_REASON(res, size, errno);
  }

  DISABLE_PRINT_ALLOC
  PRINT_ALLOC_SUMMERY

  return 0;
}
