#ifndef ENGINE_MATH_BACKEND_HPP
#define ENGINE_MATH_BACKEND_HPP

#include "math/tensor.hpp"

#include <vector>

namespace mnt {

  template<typename T>
  class OPBackend
  {
  public:
    // Linear Algebra
    virtual Tensor<T> Transpose(Tensor<T>& _tensor) = 0;

    // Low-Level
    virtual Tensor<T> Add(Tensor<T>& _tensor_1, Tensor<T>& _tensor_2) = 0;

    // High-Level

  };

}




#endif
