// File Name:     tensor.hpp
// Author:        Arash Fatehi
// Date:          22th Mar 2021
// Description:

#ifndef ENGINE_MATH_TENSOR_HPP
#define ENGINE_MATH_TENSOR_HPP

#include "configs.hpp"

#include "memory/memory.hpp"

#include <vector>
#include <string>
#include <memory>

namespace mnt {
  template <typename T>
  class Tensor
  {
  public:
    Tensor(const std::vector<TSHAPE_TYPE>& _shape);

    T& operator [] (const size_t _index) noexcept;
    const T& operator [] (const size_t _index) const noexcept;

    Tensor Shapeshift(const std::vector<TSHAPE_TYPE>& _perm);

    std::string ShapeStr();
    std::vector<TSHAPE_TYPE> Shape();
    size_t Rank();

  private:
    std::shared_ptr<MNTMemory<T>> m_memory;

    std::vector<TSHAPE_TYPE> m_shape;
    std::unique_ptr<std::vector<TSHAPE_TYPE>> m_shapeshifter;

    bool m_shapeshift = false;
  };
}

#include "math/tensor.inl"

#endif
