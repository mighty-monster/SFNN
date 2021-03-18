
#include "utils/memon.hpp"
#include "utils/general.hpp"
#include "utils/general.cpp"

#include "math/tensor.hpp"
#include "math/tensor.cpp"

#include <vector>


int main()
{
  mnt::Logger::Init(Logger::LevelDebug, true, false);

  ENABLE_PRINT_ALLOC
  {
    mnt::Tensor<float> ts({1,3,4});

    //ts.Reshape({});

    MNT_PRINTL(ts.Shape());

  }

  PRINT_ALLOC_SUMMERY

  return 0;
}
