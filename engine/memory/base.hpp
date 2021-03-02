// File Name:     base.hpp
// Author:        Arash Fatehi
// Date:          25th Feb 2021
// Description:   The base class for memory subsystem of engine
// ---------------------
// Detail Description:
// The memory subsystem provides a unified API for accessing memory,
// no matter where the memory is allocated (RAM, GPU Memory, RDMA, SSD, etc.)
// ---------------------
// Info:
// Below diagram is a rough approximation of memory architecture,
// which is likely to change as the project continues, also other sections will add or remove from it.
//
// BaseMemory ---> SimpleMemory  ---> SimpleHeapMemory
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
// Note:
// "Size" and "Length" are seperate concept in the context of memory classes, "Size" refers to
// actual bytes allocated and "Length" is number of items addressable by memory class, e.g. size of
// memory class of type int with the length of 10 is 40 if sizeof(int) = 4. Also, in some cases like
// BlockMemory the allocated memory might exceeds the memory available to end user.
// Always use "Length" when you want to access all items in a loop.

#pragma once

#include <cstddef>

// Defining "malloc" and "free" signatures as a types, to pass allocator and deallocator
// as template arguments to Memory classes
typedef void* (ALLOCATOR)(size_t);
typedef void (DEALLOCATOR)(void*);

namespace nne {
  // The base class of memory subsystem, it is an abstract class that needs implementation
  // Memory classes can recieve custome allocator and deallocator functions as their template params
  template <ALLOCATOR* ALLOC, DEALLOCATOR* DEALLOC>
  class BaseMemory
  {
  public:
    // Attention: Recieves the length and not the size
    virtual void Resize(const size_t& p_length) = 0;
    virtual inline size_t SizeInBytes() = 0;

  protected:
    virtual ~BaseMemory() = default;
    // Size of allocated memory in bytes
    size_t m_size = 0;
    // Determines whether the class have any memory allocated or not
    bool m_allocated = false;
  };
}


