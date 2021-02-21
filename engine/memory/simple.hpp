#pragma once

#include "base.hpp"

namespace nnc {
  template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
  class SimpleMemory : public BaseMemory<ALLOC, DEALLOC>
  {
  public:
    size_t Size() override;

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
    SimpleMemory() {};
    virtual ~SimpleMemory(){};
    void* m_memory;
  };
}
