#pragma once

#include <cinttypes>
#include <string>

#include "base.hpp"

namespace nne {

  template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
  class BlockMemory : public BaseMemory<ALLOC, DEALLOC>
  {
  public:
    size_t SizeInBytes() override;
    uint16_t NoOfBlocks();

    virtual void SaveToFile(const char* file_path);
    virtual void LoadFromFile(const char* file_path) = 0;
    virtual void LoadFromFile(const char* file_path, const size_t& no_of_blocks) = 0;

    virtual void SaveToHexFile(const char* file_path);
    virtual void LoadFromHexFile(const char* file_path) = 0;
    virtual void LoadFromHexFile(const char* file_path, const size_t& no_of_blocks) = 0;

    virtual void Reshape(const size_t& no_of_blocks) = 0;

    virtual std::string ToHex();

    template<typename T>
    T GetAsType(const size_t& index);

    template<typename T>
    void SetAsType(const size_t& index, const T& value);

  protected:
    BlockMemory() = default;
    virtual ~BlockMemory() = default;
    size_t m_block_size;
    size_t m_block_length;
    void** m_block_array;
    size_t m_no_of_blocks = default_no_of_block;
    static const size_t default_no_of_block = 2;
  };
}
