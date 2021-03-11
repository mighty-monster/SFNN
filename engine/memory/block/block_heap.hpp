#pragma once

#include "memory/block/block.hpp"
#include "memory/block/block.cpp"

namespace mnt {
  template <typename T>
  class BlockHeapMemory : public BlockMemory<T>
  {
  public:
    BlockHeapMemory(Allocator* p_allocator = nullptr);
    BlockHeapMemory(const char* p_file_path, Allocator* p_allocator = nullptr);
    BlockHeapMemory(const char* p_file_path, const uint16_t p_no_of_blocks, Allocator* p_allocator = nullptr);
    BlockHeapMemory(const size_t p_length, Allocator* p_allocator = nullptr);
    BlockHeapMemory(const size_t p_length, const uint16_t p_no_of_blocks, Allocator* p_allocator = nullptr);
    ~BlockHeapMemory();

    void Allocate(const size_t p_length, const uint16_t p_no_of_blocks);
    void Deallocate();

    virtual void LoadFromFile(const char* p_file_path) override;
    virtual void LoadFromFile(const char* p_file_path, const uint16_t p_no_of_blocks) override;

    void Resize(const size_t p_length);
    void Reshape(const uint16_t p_no_of_blocks) override;

    T& operator [] (const size_t p_index);
    const T& operator [] (const size_t p_index) const;

  };

}
