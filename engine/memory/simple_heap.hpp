#pragma once

#include <cstddef>
#include <cstdint>

#include "simple.hpp"
#include "simple.cpp"

namespace nne {
  template <typename T>
  class SimpleHeapMemory : public SimpleMemory<nullptr, nullptr>
  {
  public:
    SimpleHeapMemory();
    SimpleHeapMemory(const char* p_file_path);
    SimpleHeapMemory(const size_t& p_length);
    ~SimpleHeapMemory();

    void Allocate(const size_t& p_length);
    void Deallocate();

    void LoadFromFile(const char* p_file_path) override;

    void LoadFromHexFile(const char* p_file_path) override;

    void Resize(const size_t& p_length) override;

    T& operator [] (const size_t& p_index);
    const T& operator [] (const size_t& p_index) const;

    size_t Length();

  private:
    size_t m_length = 0;
  };

}


