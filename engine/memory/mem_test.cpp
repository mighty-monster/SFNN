#include "utils/memon.hpp"
#include "utils/logger.hpp"

int main()
{
  nne::Logger::Init(nne::Logger::LevelDebug, true, true);
  ENABLE_PRINT_ALLOC
  {
  }

  DISABLE_PRINT_ALLOC
  PRINT_ALLOC_SUMMERY

  return 0;
}
