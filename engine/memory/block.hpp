#pragma once

#include "memory/base.hpp"

#include <cinttypes>
#include <string>

#define BLOCKMEMORY_DEFAULT_NO_OF_BLOCKS  4

namespace nne {

  template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
  class BlockMemory : public BaseMemory<ALLOC, DEALLOC>
  {
  public:
    inline size_t SizeInBytes() override;
    inline uint16_t NoOfBlocks();

    virtual void SaveToFile(const char* p_file_path);
    virtual void LoadFromFile(const char* p_file_path) = 0;
    virtual void LoadFromFile(const char* p_file_path, const uint16_t& p_no_of_blocks) = 0;

    virtual void SaveToHexFile(const char* p_file_path);
    virtual void LoadFromHexFile(const char* p_file_path) = 0;
    virtual void LoadFromHexFile(const char* p_file_path, const uint16_t& p_no_of_blocks) = 0;

    virtual void Reshape(const uint16_t& no_of_blocks) = 0;

    virtual std::string ToHex();

    template<typename T>
    T GetAsType(const size_t& p_index);

    template<typename T>
    void SetAsType(const size_t& p_index, const T& p_value);

  protected:
    BlockMemory() = default;
    virtual ~BlockMemory() = default;
    size_t m_block_size;
    size_t m_block_length;
    void** m_block_array;
    uint16_t m_no_of_blocks = BLOCKMEMORY_DEFAULT_NO_OF_BLOCKS;
  };
}
