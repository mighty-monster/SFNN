#ifndef MATH_TENSOR_CPP
#define MATH_TENSOR_CPP

#include "tensor.hpp"

#include <sstream>

using namespace mnt;

template <typename T>
Tensor<T>::Tensor(std::vector<TSHAPE_TYPE> _shape)
{
  m_shape = _shape;
}

template <typename T>
void Tensor<T>::Reshape(std::vector<TSHAPE_TYPE> _shape)
{
  m_shape = _shape;
}

template <typename T>
std::string Tensor<T>::Shape()
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
