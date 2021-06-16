#ifndef ENGINE_MATH_BACKENDS_DEFAULT_HPP
#define ENGINE_MATH_BACKENDS_DEFAULT_HPP

#include "math/tensor.hpp"
#include "math/backend.hpp"

namespace mnt {

  template<typename T>
  class DefaultBackend : OPBackend<T>
  {
  public:
    // Linear Algebra
    virtual Tensor<T> Transpose(Tensor<T>& _tensor);

    // Low-Level
    virtual Tensor<T> Add(Tensor<T>& _tensor_1, Tensor<T>& _tensor_2);

    // High-Level

  };

}

#include "math/backends/default.inl"

#endif
