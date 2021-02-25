#pragma once

#include "memory/simple_heap.hpp"

template <typename T>
using HeapMemory = nne::SimpleHeapMemory<T>;


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



