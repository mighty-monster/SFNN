#pragma once

#include "memory/linear/linear_heap.hpp"

template <typename T>
using HeapMemory = nne::LinearHeapMemory<T>;


namespace nnc {
  template <typename T>
  class Tensor
  {
  public:
    Tensor();
  private:
    HeapMemory<T> m_memory;
  };
}



