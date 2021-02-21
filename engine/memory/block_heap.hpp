#pragma once

#include "block.hpp"

namespace nnc {
  template <typename T>
  class BlockHeapMemory : public BlockMemory<nullptr, nullptr>
  {
  public:
    BlockHeapMemory(const char* file_path);
    BlockHeapMemory(const char* file_path, const uint16_t& no_of_blocks);
    BlockHeapMemory(const size_t& length);
    BlockHeapMemory(const size_t& length, const uint16_t& no_of_blocks);
    ~BlockHeapMemory();

    virtual void LoadFromFile(const char* file_path) override;
    virtual void LoadFromFile(const char* file_path, uint16_t no_of_blocks) override;

    virtual void LoadFromHexFile(const char* file_path) override;
    virtual void LoadFromHexFile(const char* file_path, uint16_t no_of_blocks) override;

    void Resize(const size_t& length) override;
    void Resize(const size_t& length, const uint16_t& block_size);

    T& operator [] (const int64_t& index);
    const T& operator [] (const int64_t& index) const;

    size_t Length();

  private:
    size_t m_length = 0;
  };

}
