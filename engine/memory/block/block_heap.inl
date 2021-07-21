// File Name:     block_heap.inl
// Author:        Arash Fatehi
// Date:          13th Mar 2021
// Description:   Block heap memory class

#ifndef ENGINE_MEMORY_BLOCK_HEAP_INL
#define ENGINE_MEMORY_BLOCK_HEAP_INL

#include "memory/block/block_heap.hpp"

#include <cstring>


// -> These macros make debuging harder, but they are needed to avoid code duplication
#define ALLOCATE_BASE_ARRAY(_base_ptr, _no_of_blocks) \
  _base_ptr = this->m_allocator ? \
        (void**)(this->m_allocator->Allocate(_no_of_blocks * sizeof(void*))) : \
        new void*[_no_of_blocks];

#define DEALLOCATE_BASE_ARRAY(_base_ptr) \
  this->m_allocator ? \
          this->m_allocator->Deallocate(_base_ptr) : \
          delete[] (_base_ptr);

#define ALLOCATE_BLOCK(_base_ptr, _block_length, _i) \
  *(_base_ptr + _i) = this->m_allocator ? \
        this->m_allocator->Allocate(_block_length * sizeof(T)) : \
        (void*)(new T[_block_length]);

#define DEALLOCATE_BLOCK(_base_ptr, _i) \
  this->m_allocator ? \
          this->m_allocator->Deallocate(*(_base_ptr + _i)) : \
          delete[] (T*)(*(_base_ptr + _i));
// <- These macros make debuging harder, but they are needed to avoid code duplication

using namespace mnt;

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const char* _file_path)
{
  LoadFromFile(_file_path);
};

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const char* _file_path, const uint16_t _no_of_blocks)
{
  LoadFromFile(_file_path, _no_of_blocks);
};

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const size_t _length)
  : BlockHeapMemory(_length, BLOCKMEMORY_DEFAULT_NO_OF_BLOCKS)
{};

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const size_t _length, const uint16_t _no_of_blocks)
{
  if (_length > 0)
    Allocate(_length, _no_of_blocks);
};

template <typename T>
BlockHeapMemory<T>::~BlockHeapMemory()
{ Deallocate(); };

// Provides strong exception safety
template <typename T>
void BlockHeapMemory<T>::Allocate(const size_t _length, const uint16_t _no_of_blocks)
{
  char exception_message[MNT_EXCEPTION_MESSAGE_SIZE];

  void** prevoius_block_array = this->m_block_array;
  size_t previous_no_of_blocks = this->m_no_of_blocks;
  size_t previous_block_length = this->m_block_length;
  size_t previous_block_size = this->m_block_size;

  ALLOCATE_BASE_ARRAY(this->m_block_array, _no_of_blocks);

  if (!this->m_block_array)
  {
    sprintf_mnt(exception_message,
                MNT_EXCEPTION_MESSAGE_SIZE,
                "Couldn`t allocate %d bytes of memory",
                this->m_no_of_blocks*sizeof(void*));
    MNT_THROW(exception_message);
  }

  this->m_no_of_blocks = _no_of_blocks;

  // Allocating one extra item per blocks simplifies resize() and reshape() functions
  this->m_block_length = _length / _no_of_blocks + 1;
  this->m_block_size = this->m_block_length * sizeof (T);

  for(uint16_t i=0; i<_no_of_blocks; i++)
  {
    try
    {
      ALLOCATE_BLOCK(this->m_block_array, this->m_block_length, i);

      if(!this->m_block_array[i])
      {
        sprintf_mnt(exception_message,
                    MNT_EXCEPTION_MESSAGE_SIZE,
                    "Couldn`t allocate %d bytes of memory",
                    this->m_block_size);
        MNT_THROW(exception_message);
      }

    }
    catch (std::exception& ex)
    {
      // Recovering previous state of object
      for (int j=i-1; j>=0; j--)
        DEALLOCATE_BLOCK(this->m_block_array, j);

      this->m_block_array = prevoius_block_array;
      this->m_no_of_blocks = previous_no_of_blocks;
      this->m_block_length = previous_block_length;
      this->m_block_size = previous_block_size;

      throw;
    }
  }

  this->m_length = _length;
  this->m_size = this->m_block_size * this->m_no_of_blocks;

  this->m_allocated = true;
};

template <typename T>
void BlockHeapMemory<T>::Deallocate() noexcept
{
  for(uint16_t i=0; i<this->m_no_of_blocks; i++)
    DEALLOCATE_BLOCK(this->m_block_array, i);

  DEALLOCATE_BASE_ARRAY(this->m_block_array);

  this->m_length = 0;
  this->m_size = 0;
  this->m_no_of_blocks = BLOCKMEMORY_DEFAULT_NO_OF_BLOCKS;
  this->m_allocated = false;
};

// Provides basic exception safety
template <typename T>
void BlockHeapMemory<T>::LoadFromFile(const char* _file_path)
{ LoadFromFile(_file_path, this->m_no_of_blocks); };

// Provides basic exception safety
template <typename T>
void BlockHeapMemory<T>::LoadFromFile(const char* _file_path, const uint16_t _no_of_blocks)
{
  auto input_file = std::fstream(_file_path, std::ios::in | std::ios::binary | std::ios::ate);

  if (input_file.fail())
    MNT_THROW_C("file path is not valid or filesystem error", errno);

  size_t file_size = input_file.tellg();

  if (input_file.fail())
  {
    input_file.close();
    MNT_THROW_C( "failed to get file size", errno);
  }

  size_t length = file_size/sizeof(T);

  size_t previous_length = this->m_length;

  try
  {
    Resize(length);
    Reshape(_no_of_blocks);
  }
  catch (std::exception& ex)
  {
    input_file.close();
    throw;
  }


  for (uint16_t i=0; i<this->m_no_of_blocks; i++)
  {
    void* buffer = *(this->m_block_array + i);
    input_file.seekg( i*(this->m_block_size), std::ios::beg);
    input_file.read((char*)buffer, this->m_block_size);

    if (input_file.fail())
    {
      input_file.close();

      Resize(previous_length);

      MNT_THROW_C( "failed to read the file", errno);
    }
  }

  input_file.close();
};

// Provides strong exception safety
template <typename T>
void BlockHeapMemory<T>::Resize(const size_t _length)
{
  if (_length == this->m_length) return;
  if (_length == 0) {Deallocate(); return;}

  size_t new_size = _length * sizeof(T);

  // Check if allocation and deallocation is unnecessary
  if (new_size > (this->m_size - this->m_block_size ) && new_size <= this->m_size)
  {
    this->m_length = _length;
    return;
  }

  if (this->m_allocated)
  {
    char exception_message[MNT_EXCEPTION_MESSAGE_SIZE];

    int64_t extra_memory_bytes = (int64_t)(_length - this->m_length) * sizeof (T);

    void** previous_block_array = this->m_block_array;
    uint16_t previous_no_of_blocks = this->m_no_of_blocks;

    int16_t no_of_new_blocks = (int16_t)(extra_memory_bytes / (int64_t)(this->m_block_size));
    no_of_new_blocks += _length > this->m_length ? 1 : 0;

    if (no_of_new_blocks != 0)
    {
      this->m_no_of_blocks += no_of_new_blocks;

      try
      {
        ALLOCATE_BASE_ARRAY(this->m_block_array, this->m_no_of_blocks);
        if (!this->m_block_array)
        {
          sprintf_mnt(exception_message,
                      MNT_EXCEPTION_MESSAGE_SIZE,
                      "Couldn`t allocate %d bytes of memory",
                      this->m_no_of_blocks*sizeof(void*));
          MNT_THROW(exception_message);
        }
      }
      catch (std::exception& ex)
      {
        this->m_block_array = previous_block_array;
        this->m_no_of_blocks = previous_no_of_blocks;
        throw;
      }


      if (_length > this->m_length)
      {
        memcpy(this->m_block_array,
               previous_block_array,
               previous_no_of_blocks * sizeof (void*));

        for (uint16_t i = previous_no_of_blocks; i < this->m_no_of_blocks; i++)
        {
          try
          {
            ALLOCATE_BLOCK(this->m_block_array, this->m_block_length, i);
            if (!this->m_block_array)
            {
              sprintf_mnt(exception_message,
                          MNT_EXCEPTION_MESSAGE_SIZE,
                          "Couldn`t allocate %d bytes of memory",
                          this->m_block_size);
              MNT_THROW(exception_message);
            }
          }
          catch (std::exception& ex)
          {
            // Recovering previous state of the object
            for (int j = i-1; j>=previous_no_of_blocks; j--)
              DEALLOCATE_BLOCK(this->m_block_array, j);

            DEALLOCATE_BASE_ARRAY(this->m_block_array);

            this->m_block_array = previous_block_array;
            this->m_no_of_blocks = previous_no_of_blocks;
            throw;
          }
        }
      }
      else
      {
        memcpy(this->m_block_array,
               previous_block_array,
               this->m_no_of_blocks * sizeof (void*));

        for (uint16_t i = previous_no_of_blocks-1; i >= this->m_no_of_blocks; i--)
          DEALLOCATE_BLOCK(previous_block_array, i);
      }

      DEALLOCATE_BASE_ARRAY(previous_block_array);
    }

    this->m_length = _length;
    this->m_size = this->m_no_of_blocks * this->m_block_size;

  }
  else
  {
    Allocate(_length, this->m_no_of_blocks);
  }

};

template <typename T>
void BlockHeapMemory<T>::Reshape(const uint16_t _no_of_blocks)
{
  if (_no_of_blocks == 0) {return;}
  if (_no_of_blocks == this->m_no_of_blocks) {return;}

  uint16_t old_no_of_blocks = this->m_no_of_blocks;
  size_t old_block_size = this->m_block_size;
  void** old_block_array = this->m_block_array;
  size_t old_size = this->m_size;

  Allocate(this->m_length, _no_of_blocks);

  size_t old_index = 0;
  size_t new_index = 0;
  size_t copied_bytes = 0;
  void* source = nullptr;
  void* destination = nullptr;
  size_t copy_size = 0;
  size_t current_pos = 0;
  size_t next_pos = 0;

  // Because of one extra item allocated for each block, m_size will changes
  // so the smaller between m_size and old_size is the number of bytes to copy
  size_t content_size = !(this->m_size > old_size) * this->m_size +
                         (this->m_size > old_size) * old_size;

  // This section copies the content of previous blocks to the new ones
  // The number of previous and new blocks are arbitrary
  // Imaging each - is a byte, | are block edges, and difference between next edges
  // till the copied index are margines
  // |------|------|------|------|
  // |----|----|----|----|----|
  while(copied_bytes < content_size)
  {

    size_t next_new_block_edge = (new_index + 1) * this->m_block_size;
    size_t next_old_block_edge = (old_index + 1) * old_block_size;

    size_t new_old_margin = next_new_block_edge - copied_bytes;
    size_t old_new_margin = next_old_block_edge - copied_bytes;

    // Nearest edge from current position is chosen for next copy
    next_pos = next_new_block_edge >= next_old_block_edge ?
               next_old_block_edge : next_new_block_edge;

    // Copy size is determined accordingly
    copy_size = next_pos - current_pos;

    // Four different scenarios are considered, and proper indexes for memcpy is calculated
    if (copy_size == old_block_size)
    {
      // size: <------>
      // old:  |p-----|------|------|
      // new:  |p---------|----------|
      size_t internal_index = copied_bytes % (this->m_block_size);
      source = *(old_block_array + old_index);
      destination = (char*)*(this->m_block_array + new_index) + internal_index;
      old_index++;
    }
    else if (copy_size == this->m_block_size)
    {
      // size: <------>
      // old:  |p---------|----------|
      // new:  |p-----|------|------|
      size_t internal_index = copied_bytes % old_block_size;
      source = (char*)*(old_block_array + old_index) + internal_index;
      destination = *(this->m_block_array + new_index);
      new_index++;
    }
    else if (copy_size == new_old_margin)
    {
      // size:     <-->
      // old:  |---p------|----------|
      // new:  |---p--|------|------|
      source = *(old_block_array + old_index);
      destination = (char*)(*(this->m_block_array + new_index)) + (this->m_block_size - copy_size);
      new_index++;
    }
    else if (copy_size == old_new_margin)
    {
      // size:     <-->
      // old:  |---p--|------|------|
      // new:  |---p------|----------|
      source = (char*)(*(old_block_array + old_index)) + (old_block_size - copy_size);
      destination = *(this->m_block_array + new_index);
      old_index++;
    }

    memcpy(destination, source , copy_size);
    current_pos = next_pos;
    copied_bytes += copy_size;
  }

  for(uint16_t i=0; i<old_no_of_blocks; i++)
  {
    DEALLOCATE_BLOCK(old_block_array, i);
  }

  DEALLOCATE_BASE_ARRAY(old_block_array);

};


#endif
