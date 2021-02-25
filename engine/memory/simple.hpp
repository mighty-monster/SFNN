#pragma once

#include <string>

#include "base.hpp"

namespace nne {
  template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
  class SimpleMemory : public BaseMemory<ALLOC, DEALLOC>
  {
  public:
    size_t SizeInBytes() override;

    virtual void SaveToFile(const char* file_path);
    virtual void LoadFromFile(const char* file_path) = 0;

    virtual void SaveToHexFile(const char* file_path);
    virtual void LoadFromHexFile(const char* file_path) = 0;

    std::string ToHex();

    template<typename T>
    T GetAsType(const size_t& index);

    template<typename T>
    void SetAsType(const size_t& index, const T& value);

  protected:
    SimpleMemory() = default;
    virtual ~SimpleMemory() = default;
    void* m_memory;
  };
}
