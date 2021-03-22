#ifndef ENGINE_MATH_TENSOR_M_HPP
#define ENGINE_MATH_TENSOR_M_HPP

#include "math/tensor.hpp"

#include <vector>

namespace mnt {

  template<typename T>
  class LIALOperations
  {

  };

  template<typename T>
  class LOLEOperations
  {

  };

  template<typename T>
  class HILEOperations
  {

  };

  template<typename T>
  class MNTBackend
  {
  public:
    // General


    // Low-Level
    Tensor<T> Add(Tensor<T>& _tensor_1, Tensor<T>& _tensor_2);

    // High-Level

  };

}




#endif
