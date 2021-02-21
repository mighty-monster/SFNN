#pragma once

#include <cstddef>
#include <cstdint>

#include "simple.hpp"
#include "simple.cpp"

namespace nnc {
  template <typename T>
  class SimpleHeapMemory : public SimpleMemory<nullptr, nullptr>
  {
  public:
    SimpleHeapMemory(const char* file_path);
    SimpleHeapMemory(const size_t& length);
    ~SimpleHeapMemory();

    void LoadFromFile(const char* file_path) override;

    void LoadFromHexFile(const char* file_path) override;

    void Resize(const size_t& length) override;

    T& operator [] (const int64_t& index);
    const T& operator [] (const int64_t& index) const;

    size_t Length();

  private:
    size_t m_length = 0;
  };

}


