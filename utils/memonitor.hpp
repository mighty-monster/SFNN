///////////////////////////////////////////////////////////////////////////////
// @File Name:     memonitor.hpp                                             //
// @Author:        Arash Fatehi                                              //
// @L.M.D:         18th Feb 2021                                             //
// @Description:   Simple header to monitor heap allocation                  //
//                                                                           //
// Detail Description:                                                       //
// By including in one the execution units, it overloads "new" and "delete"  //
// operatations, in addition to detect meamory leaks, it is usefull to trace //
// third party libraries memory allocation calls when debuging               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <future>

#define DISABLE_PRINT_ALLOC nnc::s_print_allocations = false;
#define ENABLE_PRINT_ALLOC nnc::s_print_allocations = true;
#define PRINT_ALLOC_SUMMERY nnc::PrintAllocationSummery();

namespace nnc {
  // Simple data structure to store allocated memory address and size
  class AllocationInfo
  {
  public:
    void Set(void*& memory, size_t& size)
    {
      m_memory = memory;
      m_size = size;
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
    }

    ~AllocationTable()
    {
      // Deallocating array`s memory
      free(m_allocation_table);
    }

    // Add new record of info to the table
    void Insert(void*& memory, size_t& size)
    {
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
          std::cerr << "AllocationTable::Insert() -- realloc failed\n";
      }

      // Inserting the allocation info to last item in the array
      m_allocation_table[m_occupied].Set(memory, size);

      // Increase last item`s index
      m_occupied++;
    }

    // Remove record of info related to address "memory"
    void Remove(void*& memory)
    {
      std::lock_guard<std::mutex> lock(m_mutex);

      for (uint64_t i=0; i<m_occupied; i++)
        if (m_allocation_table[i].m_memory == memory)
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
    };

    // Get size of allocated memory to address "memory"
    size_t GetSize(void*& memory)
    {
      std::lock_guard<std::mutex> lock(m_mutex);

      size_t result = 0;
      for (uint64_t i=0; i<m_occupied; i++)
        if (m_allocation_table[i].m_memory == memory)
        {
          result = m_allocation_table[i].m_size;
          break;
        }
      if (!result)
        std::cerr << "AllocationTable::GetSize(void* memory) -- memory entry not found\n";

      return result;
    };

  private:
    // Storing and reading from table should have lock to be thread safe
    std::mutex m_mutex;

    // The length of array
    uint64_t m_length = 100;

    // Last item`s index, no of occupied records in the array
    uint64_t m_occupied = 0;

    // Pointer to begining of the array
    AllocationInfo* m_allocation_table;
  };

  // Global variable to track allocation info
  // Will cause Linking error if included in more than one
  // Execution unit, even by making them static, operators
  // can`t be static and will cause Linking error
  uint64_t g_allocation_counter = 0;
  uint64_t g_allcoated_bytes = 0;
  uint64_t g_deallcoated_bytes = 0;

  // Determine print allocations into cout or not
  bool s_print_allocations = false;

  const char* s_allocation_monitor_signature = "[Heap]: ";

  // The AllocationTable object to store allocation info
  AllocationTable s_allocation_table;

  void PrintAllocationSummery()
  {
    std::cout << "\n---------------\n----Summery----\n";
    std::cout << "Allocation Counter: "<<nnc::g_allocation_counter<<std::endl;
    std::cout << "Allocated: "<<nnc::g_allcoated_bytes<< " bytes" <<std::endl;
    std::cout << "Deallocated: "<<nnc::g_deallcoated_bytes<< " bytes"<<std::endl;
    std::cout << "---------------\n";
  }
};

// operator overload for "new"
void* operator new (size_t size)
{
  nnc::g_allocation_counter++;
  nnc::g_allcoated_bytes += size;

  if (nnc::s_print_allocations)
    std::cout << nnc::s_allocation_monitor_signature << size << " bytes allcoated\n";

  void* memory = malloc(size);

  if (!memory)
    throw std::bad_alloc();

  nnc::s_allocation_table.Insert(memory, size);

  return memory;
}

// operator overload for "delete"
void operator delete(void* memory) noexcept
{
  nnc::g_allocation_counter--;

  size_t size = nnc::s_allocation_table.GetSize(memory);

  nnc::g_deallcoated_bytes += size;

  if (nnc::s_print_allocations)
    std::cout << nnc::s_allocation_monitor_signature << size << " bytes deallcoated\n";

  nnc::s_allocation_table.Remove(memory);

  free(memory);
}
