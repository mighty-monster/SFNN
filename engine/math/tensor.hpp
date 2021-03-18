#ifndef MATH_TENSOR_HPP
#define MATH_TENSOR_HPP

#include "configs.hpp"

#include "memory/memory.hpp"
#include "memory/memory.cpp"

#include <vector>
#include <string>
#include <memory>

namespace mnt {
  template <typename T>
  class Tensor
  {
  public:
    Tensor(std::vector<TSHAPE_TYPE> _shape);

    // These should be separate functions or part of class ?? And why ??
    void Reshape(std::vector<TSHAPE_TYPE> _shape);

    Tensor View(std::vector<TSHAPE_TYPE> _shape);

    virtual Tensor Transpose(std::vector<TSHAPE_TYPE> _perm);

    std::string Shape();

  private:
    std::shared_ptr<MNTMemory<T>> m_memory;
    std::vector<TSHAPE_TYPE> m_shape;
  };
}

#endif
