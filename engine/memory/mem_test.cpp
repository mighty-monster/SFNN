#include "utils/memon.hpp"
#include "utils/general.hpp"

#include "math/tensor.hpp"

#include <vector>


int main()
{
  mnt::Logger::Init(Logger::LevelDebug, true, false);

  ENABLE_PRINT_ALLOC
  {
    mnt::Tensor<float> ts({1,3,4});


    MNT_PRINTL(ts[0]);

  }

  PRINT_ALLOC_SUMMERY

  return 0;
}
