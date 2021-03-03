// File Name:     memon.hpp
// Author:        Arash Fatehi
// Date:          18th Feb 2021
// Description:   Simple header to monitor heap allocation
// --------------------
// Detail Description:
// By including this header in one of the execution units, it overloads "new" and "delete"
// operatations, in addition to detect meamory leaks, it is usefull to trace
// third party libraries memory allocation calls when debuging
// ---------------------
// Help:
// Including the header in more than one execution unit leads to linker errors, it should be
// included only once, "DISABLE_PRINT_ALLOC" and "ENABLE_PRINT_ALLOC" macros do as they suggest
// "PRINT_ALLOC_SUMMERY" macro prints allocated and deallocated bytes from beginning
// ---------------------
// Note: This is too low-level to use Logger class for reporting the errors, for example
// exceptions might allocate memory and interfere monitoring and reproting of heap allocations
// So the potential error are reported using "std::cerr" instead

#pragma once

#include "general.hpp"

#include <iostream>
#include <future>

#define DISABLE_PRINT_ALLOC nne::g_print_allocations = false;
#define ENABLE_PRINT_ALLOC nne::g_print_allocations = true;
#define PRINT_ALLOC_SUMMERY nne::PrintAllocationSummery();

namespace nne {
  // If true "PRINT_ALLOC_SUMMERY" macro does it`s trick
  bool g_print_allocations = false;

  // Prix of printed lines, for searching and filtering
  const char* g_allocation_monitor_signature = "[Heap]: ";

  // Global variable to track allocation info
  // Will cause Linking error if included in more than one
  // Execution unit, even by making them static, operators
  // can`t be static and will cause Linking error
  uint64_t g_allocation_counter = 0;
  uint64_t g_allocated_counter = 0;
  uint64_t g_deallocated_counter = 0;
  uint64_t g_allocated_bytes = 0;
  uint64_t g_deallocated_bytes = 0;
  uint64_t g_allocation_table_bytes = 0;

  // Streams don't throw exceptions by default
  void PrintAllocationSummery() noexcept
  {
    const size_t buffer_size = NNE_MEMON_INITIAL_TABLE_SIZE;
    char human_readable[buffer_size];

    std::cout << "\n---------------\n----Summery----\n";
    std::cout << "Allocation Calls: "<<nne::g_allocated_counter<<std::endl;
    std::cout << "Deallocation Calls: "<<nne::g_deallocated_counter<<std::endl;

    nne::BytesToHumanReadableSize(nne::g_allocated_bytes, human_readable, buffer_size);
    std::cout << "Allocated: "<< human_readable << ", " <<
                 nne::g_allocated_bytes << " Byte(s)" <<std::endl;

    nne::BytesToHumanReadableSize(nne::g_deallocated_bytes, human_readable, buffer_size);
    std::cout << "Deallocated: "<< human_readable << ", " <<
                 nne::g_deallocated_bytes << " Byte(s)" << std::endl;

    nne::BytesToHumanReadableSize(nne::g_allocation_table_bytes, human_readable, buffer_size);
    std::cout << "Allocation Table: "<< human_readable << ", " <<
                 nne::g_allocation_table_bytes << " Byte(s)" << std::endl;

    if (g_allocated_bytes == g_deallocated_bytes)
      std::cout << "No memory leak detected, we are safe ;)\n";
    else
      std::cout << "Memory leak detected :(, better to check it with valgrind!\n";

    std::cout << "---------------\n";
  }

  // Streams don't throw exceptions by default
  void RecordAllocation(size_t p_size) noexcept
  {
    nne::g_allocation_counter++;
    nne::g_allocated_counter++;

    nne::g_allocated_bytes += p_size;

    if (nne::g_print_allocations)
      std::cout << nne::g_allocation_monitor_signature << p_size << " bytes allcoated\n";
  }

  // Streams don't throw exceptions by default
  void RecordDeallocation(size_t p_size) noexcept
  {
    nne::g_allocation_counter--;
    nne::g_deallocated_counter++;

    nne::g_deallocated_bytes += p_size;

    if (nne::g_print_allocations)
      std::cout << nne::g_allocation_monitor_signature << p_size << " bytes deallcoated\n";
  }

  // Simple data structure to store allocated memory address and size
  class AllocationInfo
  {
  public:
    AllocationInfo() noexcept = default;
    ~AllocationInfo() noexcept = default;

    void Set(void*& p_memory, size_t& p_size) noexcept
    {
      m_memory = p_memory;
      m_size = p_size;
    };

    void* m_memory;
    size_t m_size;
  };

  // Using simple array of AllocationPair, stores allocaiton calls, because it is used
  // only in debug mode, not tree was implemented to accelarate GetSize and Remove
  // fucntions, the time complexity of GetSize and Remove is O(n)
  // Needed memory is allocated using malloc to prevent stack overflow, for the same
  // reason std::map was not used
  class AllocationTable
  {
  public:
    AllocationTable()
    {
      // Allocating array`s memory
      m_allocation_table = (AllocationInfo*)malloc(m_length*sizeof (AllocationInfo));

      if (!m_allocation_table)
      {
        NNE_ERORR_LL("Could not allocate memory for allocation table using malloc, "
                  "this realy should not happen, this is the end I guess... ");
        return;
      }
      g_allocation_table_bytes = m_length*sizeof (AllocationInfo);
    }

    ~AllocationTable() noexcept
    {
      // Deallocating array`s memory
      free(m_allocation_table);
    }

    // Add new record of info to the table
    void Insert(void*& p_memory, size_t& p_size)
    {
      // "std::lock_guard" might throw exception, as per it`s definition
      // could not find any resource to suggest otherwise
      std::lock_guard<std::mutex> lock(m_mutex);

      // Increase size of array if it is full
      if (m_occupied >= m_length)
      {
        // Size of will increase 3/2 of it`s previous size
        // each time it become full
        m_length = m_length * 3 / 2;

        // realloc call to resize the array
        m_allocation_table = (AllocationInfo*)realloc(m_allocation_table, m_length*sizeof (AllocationInfo));
        if (!m_allocation_table)
        {
          NNE_ERORR_LL("Insert to allocation table failed. Not enough memory!");
          NNE_ERORR_LL("realloc failed, maybe a lot memory was allocated before and we are out of "
                    "memory, or it`s not possible to allocate a continous chunk, "
                    "anyway, this realy should not happen, this is the end I guess... ");
          return;
        }

        g_allocation_table_bytes = m_length*sizeof (AllocationInfo);
      }

      // Inserting the allocation info to last item in the array
      m_allocation_table[m_occupied].Set(p_memory, p_size);

      // Increase last item`s index
      m_occupied++;
    }

    // Remove record of info related to address "memory"
    // Is called in delete ... so it should be noexcept
    // It`s not a good idea to throw error on deleteing or destructing
    // It interfere the RAII resource deallocation
    void Remove(void* p_memory) noexcept
    {
      try
      {
        // "std::lock_guard" might throw exception
        std::lock_guard<std::mutex> lock(m_mutex);

        for (uint64_t i=0; i<m_occupied; i++)
          if (m_allocation_table[i].m_memory == p_memory)
          {
            //Exchange the last item with the one to remove
            m_allocation_table[i].Set(
                  m_allocation_table[m_occupied-1].m_memory,
                  m_allocation_table[m_occupied-1].m_size
                  );

            //Decreasing the last item`s index, results in deleting the record
            m_occupied--;
            break;
          }
      }
      catch (...)
      {
        NNE_ERORR_LL("Something went wront with the mutex!");
        NNE_ERORR_LL("Trying to remove without mutex");

        for (uint64_t i=0; i<m_occupied; i++)
          if (m_allocation_table[i].m_memory == p_memory)
          {
            //Exchange the last item with the one to remove
            m_allocation_table[i].Set(
                  m_allocation_table[m_occupied-1].m_memory,
                  m_allocation_table[m_occupied-1].m_size
                  );

            //Decreasing the last item`s index, results in deleting the record
            m_occupied--;
            break;
          }
      }

    };

    // Get size of allocated memory to address "memory"
    // Is called in delete ... so it should be noexcept
    size_t GetSize(void* p_memory) noexcept
    {
      size_t result = 0;
      try
      {
        std::lock_guard<std::mutex> lock(m_mutex);

        for (uint64_t i=0; i<m_occupied; i++)
          if (m_allocation_table[i].m_memory == p_memory)
          {
            result = m_allocation_table[i].m_size;
            break;
          }
      }
      catch (...)
      {
        NNE_ERORR_LL("Something went wront with the mutex!");
        NNE_ERORR_LL("Trying to get size without mutex");

        for (uint64_t i=0; i<m_occupied; i++)
          if (m_allocation_table[i].m_memory == p_memory)
          {
            result = m_allocation_table[i].m_size;
            break;
          }
      }

      if (!result)
        NNE_ERORR_LL("Bad news, memory entry not found in allocation table, memory monitoring "
                  "module is not reliable anymore.");

      return result;
    };

  private:
    // Storing and reading from table should have lock to be thread safe
    std::mutex m_mutex;

    // The length of array
    uint64_t m_length = NNE_MEMON_INITIAL_TABLE_SIZE;

    // Last item`s index, no of occupied records in the array
    uint64_t m_occupied = 0;

    // Pointer to begining of the array
    AllocationInfo* m_allocation_table;
  };

  // The AllocationTable object to store allocation info
  AllocationTable g_allocation_table;

};


// operator overload for "new"
void* operator new (size_t p_size)
{
  nne::RecordAllocation(p_size);

  void* memory = malloc(p_size);

  if (!memory)
    throw std::bad_alloc();

  nne::g_allocation_table.Insert(memory, p_size);

  return memory;
}

// operator overload for "new[]"
void* operator new[] (size_t p_size)
{
  nne::RecordAllocation(p_size);

  void* memory = malloc(p_size);

  if (!memory)
    throw std::bad_alloc();

  nne::g_allocation_table.Insert(memory, p_size);

  return memory;
}

// operator overload for "delete"
void operator delete(void* p_memory) noexcept
{
  size_t size = nne::g_allocation_table.GetSize(p_memory);

  nne::RecordDeallocation(size);

  nne::g_allocation_table.Remove(p_memory);

  free(p_memory);
}

// operator overload for "delete[]"
void operator delete[](void* p_memory) noexcept
{
  size_t size = nne::g_allocation_table.GetSize(p_memory);

  nne::RecordDeallocation(size);

  nne::g_allocation_table.Remove(p_memory);

  free(p_memory);
}
