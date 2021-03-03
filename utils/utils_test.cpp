#include "utils/general.hpp"
#include "utils/memon.hpp"
#include "utils/nnexpect.hpp"

int main()
{
  nne::Logger::Init(nne::Logger::LevelDebug, true, true);
  ENABLE_PRINT_ALLOC
  {
    NNE_ERORR("Normal error");
    NNE_ERORR_LL("Low level error");

    try
    {
      NNE_THROW("Exception error");
    }
    catch (nne::NNExcept ex)
    {
      NNE_ERORR_EX(ex);
    }

    int* a = new int[10];
      a[5] = 0;
      if (a[20])
        printf("xx\n");
  }

  nne::Logger::GetInstance().CloseLogFile();
  DISABLE_PRINT_ALLOC
  PRINT_ALLOC_SUMMERY

  return 0;
}
