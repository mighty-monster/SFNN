#pragma once

#include "base.hpp"

namespace nnc {

  struct BlockInfo
  {
    void* m_memory;
    size_t m_size;

  };

  template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
  class BlockMemory : public BaseMemory<ALLOC, DEALLOC>
  {
  public:
    size_t Size() override;
    uint16_t NoOfBlocks();

    virtual void SaveToFile(const char* file_path);
    virtual void LoadFromFile(const char* file_path) = 0;
    virtual void LoadFromFile(const char* file_path, uint16_t no_of_blocks) = 0;

    virtual void SaveToHexFile(const char* file_path);
    virtual void LoadFromHexFile(const char* file_path) = 0;
    virtual void LoadFromHexFile(const char* file_path, uint16_t no_of_blocks) = 0;

    virtual void Reshape(const uint16_t& no_of_blocks);

    virtual std::string ToHex();

    template<typename T>
    T GetAsType(const size_t& index);

    template<typename T>
    void SetAsType(const size_t& index, const T& value);

  protected:
    BlockMemory() {};
    virtual ~BlockMemory(){};
    void* m_block_array;
    uint16_t m_no_of_blocks = default_no_of_block;
    static const uint16_t default_no_of_block = 4;
  };
}
