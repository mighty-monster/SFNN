// File Name:     block_heap.hpp
// Author:        Arash Fatehi
// Date:          13th Mar 2021
// Description:   Block heap memory class

// ---------------------
// Detail Description:
// An implementation of BlockMemory abstract class, in order to allocate enough memory,
// it needs a type as template argument in addition to it`s length and uses "new" and "delete"
// ---------------------

// ---------------------
// Note:
// Allocates one extra item per block to simplicity, the extra allocated memory will left
// empty at the end of last block
// ---------------------

// ---------------------
// Note:
// When allocated on RAM, BlockHeapMemory was approximately 3 times slower than LinearHeapMemory
// on x86_64, GCC 10.2.0, Linux 4.19, with 4 blocks due to the needed logic
// for addressing the correct block and offset
// ---------------------

// =====
// [LoadFromFile(_file_path)]: Load content of memory from a binary file
// =====

// =====
// [Resize(_length)]: Allocate or Deallocate blocks, no allocation will happen if current
// configuration (m_no_of_blocks * m_block_size) is suitable for storing _length items of type T
// Allocate a new blocks if needs more memory, and Deallocates unnecessary blocks if _length < m_length
// =====

// =====
// [Reshape(_no_of_blocks)]: Changes the number of memory blockes, it is a very expensive operation
// when big amount of memory is allocated, despite the need for reallocations of blocks, the copy logic
// is also relatively complicated, but still the time complexity is O(n), so avoid unless necessary.
// =====

#ifndef ENGINE_MEMORY_BLOCK_HEAP_HPP
#define ENGINE_MEMORY_BLOCK_HEAP_HPP

#include "memory/block/block.hpp"

namespace mnt {
  template <typename T>
  class BlockHeapMemory : public BlockMemory<T>
  {
  public:
    BlockHeapMemory() = default;
    BlockHeapMemory(const char* _file_path);
    BlockHeapMemory(const char* _file_path, const uint16_t _no_of_blocks);
    BlockHeapMemory(const size_t _length);
    BlockHeapMemory(const size_t _length, const uint16_t _no_of_blocks);
    ~BlockHeapMemory();

    void Allocate(const size_t _length, const uint16_t _no_of_blocks);
    void Deallocate() noexcept;

    virtual void LoadFromFile(const char* _file_path) override;
    virtual void LoadFromFile(const char* _file_path, const uint16_t _no_of_blocks) override;

    void Resize(const size_t _length) override;
    void Reshape(const uint16_t _no_of_blocks) override;

  };

}

#include "memory/block/block_heap.inl"

#endif
