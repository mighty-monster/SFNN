// File Name:     block.hpp
// Author:        Arash Fatehi
// Date:          13th Mar 2021
// Description:   A base class for block memory classes

// ---------------------
// Detail Description:
// Block memory classes allcote memory to an array of pointers(blockes), instead of
// a single pointer, their structure is a little bit more complex than
// linear memory classes with the cost of calculation overhead for accessing the memory.
// It can be useful if allocating a big chunk of memory fails, specialy when dealing
// with pinned memory.
// ---------------------

// ---------------------
// Note:
// "SaveToFile" and "LoadFromFile" functions work with memory ignoring the ISA`s endianness,
// they dump and load memory as it is.
// So we can`t transfer files between computers with different endianness
// ---------------------

// ---------------------
// Note:
// "GetAsType" and "SetAsType" can interpret memory as variables with unaligned memory,
// it can decrease the performance and can cause race condition problems, so be carefull
// ---------------------

// =====
// [operator []]: Returns the _index`th item if blocks where a continuous array
// =====

// =====
// [SaveToFile(_file_path)]: Save content of memory to a binary file, it is not a serrialization
// function and doesn`t save the entire class, only the content that memory class refers to
// =====

// =====
// [GetAsType(_index)]: Returns a reference to the content of memory
// from _index to _index + sizeof(U) interpreted as type U
// =====

// =====
// [SetAsType(_index, _value)]: Copies the _value to memory from _index to _index + sizeof(U)
// as it was a vairiable type U
// =====


#ifndef ENGINE_MEMORY_BLOCK_HPP
#define ENGINE_MEMORY_BLOCK_HPP

#include "memory/allocator/blueprint.hpp"
#include "memory/memory.hpp"

#include <cinttypes>
#include <string>

#define BLOCKMEMORY_DEFAULT_NO_OF_BLOCKS  4

namespace mnt {
  template <typename T>
  class BlockMemory : public MNTMemory<T>
  {
  public:
    virtual ~BlockMemory() noexcept = default;

    T& operator [] (const size_t _index) noexcept override;
    const T& operator [] (const size_t _index) const noexcept override;

    void Write(const size_t _offset, const void* _buffer,const size_t _buffer_length) override;

    template<typename U>
    U& GetAsType(const size_t _index);

    template<typename U>
    void SetAsType(const size_t _index, const U& _value);

    inline uint16_t NoOfBlocks() {return m_no_of_blocks;};

    void SaveToFile(const char* _file_path);
    virtual void LoadFromFile(const char* _file_path) = 0;
    virtual void LoadFromFile(const char* _file_path, const uint16_t _no_of_blocks) = 0;

    virtual void Reshape(const uint16_t _no_of_blocks) = 0;

  protected:
    BlockMemory(Allocator* _allocator = nullptr);

  protected:
    size_t m_block_size;
    size_t m_block_length;
    void** m_block_array;
    uint16_t m_no_of_blocks = BLOCKMEMORY_DEFAULT_NO_OF_BLOCKS;
  };
}

#include "memory/block/block.inl"

#endif
