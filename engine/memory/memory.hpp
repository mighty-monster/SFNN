// File Name:     memory.hpp
// Author:        Arash Fatehi
// Date:          25th Feb 2021
// Description:   The base class for memory subsystem of engine

// ---------------------
// Detail Description:
// The memory subsystem provides a unified API for accessing memory,
// no matter where the memory is located (RAM, GPU Memory, RDMA, SSD, etc.)
// ---------------------

// ---------------------
// Info:
// Below diagram is a rough approximation of memory architecture,
// which is likely to change as the project continues.
//
// BaseMemory ---> LinearMemory  ---> LinearHeapMemory
//            |
//            |--> BlockkMemory  ---> BlockHeapMemory
//            |
//            |--> AlignedMemory ---> AlignedHeapMemory
//            |
//            |--> HeteroMemory  ---> GPUMemory ---> CUDAUnifiedMemory
//            |                                 |
//            |                                 |--> CUDAPinnedMemory
//            |                                 |
//            |                                 |--> CUDAPagedMemory
//            |                                 |
//            |                                 |--> OpenCLMemory
//            |--> RemoteMemory
//            |--> SSDMemory
//
// ---------------------

// ---------------------
// Note:
// "Size" and "Length" are seperate concept in the context of these memory classes, "Size" refers to
// actual available memory in bytes and "Length" is number of items addressable by memory class,
// e.g. size of "LinearHeapMemory" class of type int with the length of 10 is 40 if sizeof(int) = 4.
// Also, in some cases like "BlockMemory" classes the allocated memory might exceeds the memory available to end user.
// Always use "Length" when you want to access all items in a loop.
// ---------------------

// =====
// [MNTMemory]:
// The base class of memory subsystem, it is an abstract class that needs implementation
// Memory classes can recieve custome "mnt::Allocator" class as their constructor parameter,
// If no Allocator class provided, "new" and "delete" will be used.
// =====


#ifndef MEMORY_MEMORY_HPP
#define MEMORY_MEMORY_HPP

#include "memory/allocator/blueprint.hpp"

#include <cstddef>

namespace mnt {

  template<typename T>
  class MNTMemory
  {
  public:
    virtual ~MNTMemory() noexcept = default;

    // Removing copy, move constructors and assignment operator
    MNTMemory(const MNTMemory&) = delete;
    MNTMemory(MNTMemory&) = delete;
    MNTMemory(const MNTMemory&&) = delete;
    MNTMemory(MNTMemory&&) = delete;
    void operator = (const MNTMemory&) = delete;
    void operator = (const MNTMemory&&) = delete;

    virtual T& operator [] (const size_t _index) noexcept = 0;
    virtual const T& operator [] (const size_t _index) const noexcept = 0;

    inline size_t Size() noexcept {return this->m_size;}
    inline size_t Length() {return m_length;};

    // Attention: "Resize" gets the length as parameter, not the size
    virtual void Resize(const size_t _length) = 0;

    // Attention: Be careful about _buffer_length ... it is not the size, it is the length
    virtual void Write(const size_t _offset, const void* _buffer,const size_t _buffer_length) =0;

  protected:
    MNTMemory(Allocator* _allocator = nullptr) noexcept;

  protected:
    size_t m_size = 0;
    size_t m_length = 0;

    bool m_allocated = false;
    Allocator* m_allocator = nullptr;
  };
}

#endif


