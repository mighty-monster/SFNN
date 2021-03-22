#ifndef ENGINE_MATH_TENSOR_INL
#define ENGINE_MATH_TENSOR_INL

#include "memory/linear/linear_heap.hpp"

#include "math/tensor.hpp"

#include <sstream>

using namespace mnt;

template <typename T>
Tensor<T>::Tensor(const std::vector<TSHAPE_TYPE>& _shape)
{
  m_shape = _shape;

  m_memory = std::make_shared<LinearHeapMemory<T>>(10);
}

template <typename T>
T& Tensor<T>::operator [] (const size_t _index) noexcept
{
  return m_memory.get()[0][_index];
}

template <typename T>
const T& Tensor<T>::operator [] (const size_t _index) const noexcept
{
  return m_memory.get()[0][_index];
}


template <typename T>
std::string Tensor<T>::ShapeStr()
{
  std::stringstream string_stream;

  string_stream << "{";
  if (m_shape.size() > 0)
  {
    for (auto i : m_shape)
      string_stream << i << ", ";
    string_stream.seekp(-2, std::ios_base::end);
  }
  string_stream << "}";

  return string_stream.str();
}

#endif
