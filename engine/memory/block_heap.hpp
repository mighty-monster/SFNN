#pragma once

#include "block.hpp"
#include "block.cpp"

namespace nne {
  template <typename T>
  class BlockHeapMemory : public BlockMemory<nullptr, nullptr>
  {
  public:
    BlockHeapMemory();
    BlockHeapMemory(const char* file_path);
    BlockHeapMemory(const char* file_path, const size_t& no_of_blocks);
    BlockHeapMemory(const size_t& length);
    BlockHeapMemory(const size_t& length, const size_t& no_of_blocks);
    ~BlockHeapMemory();

    void Allocate(const size_t& length, const size_t& no_of_blocks);
    void Deallocate();

    virtual void LoadFromFile(const char* file_path) override;
    virtual void LoadFromFile(const char* file_path, const size_t& no_of_blocks) override;

    virtual void LoadFromHexFile(const char* file_path) override;
    virtual void LoadFromHexFile(const char* file_path, const size_t& no_of_blocks) override;

    void Resize(const size_t& length) override;
    void Reshape(const size_t& no_of_blocks) override;

    T& operator [] (const int64_t& index);
    const T& operator [] (const int64_t& index) const;

    size_t Length();

  private:
    size_t m_length = 0;
  };

}
