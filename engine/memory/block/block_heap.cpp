#include "memory/block/block_heap.hpp"

#include <cstring>

using namespace mnt;

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(Allocator* p_allocator)
{
  this->m_allocator = p_allocator;
};

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const char* p_file_path, Allocator* p_allocator)
{
  this->m_allocator = p_allocator;
  LoadFromFile(p_file_path);
};

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const char* p_file_path, const uint16_t p_no_of_blocks, Allocator* p_allocator)
{
  this->m_allocator = p_allocator;
  LoadFromFile(p_file_path, p_no_of_blocks);
};

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const size_t p_length, Allocator* p_allocator)
  : BlockHeapMemory(p_length, BLOCKMEMORY_DEFAULT_NO_OF_BLOCKS)
{
  this->m_allocator = p_allocator;
};

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const size_t p_length, const uint16_t p_no_of_blocks, Allocator* p_allocator)
{
  this->m_allocator = p_allocator;
  if (p_length > 0)
    Allocate(p_length, p_no_of_blocks);
};

template <typename T>
BlockHeapMemory<T>::~BlockHeapMemory()
{ Deallocate(); };

template <typename T>
void BlockHeapMemory<T>::Allocate(const size_t p_length, const uint16_t p_no_of_blocks)
{

  this->m_block_array = this->m_allocator ?
        (void**)(this->m_allocator->Allocate(p_no_of_blocks * sizeof(void*))) :
        new void*[p_no_of_blocks];

  this->m_no_of_blocks = p_no_of_blocks;
  this->m_length = p_length;
  this->m_size = p_length * sizeof (T);


  // Allocating one extra byte per blocks simplifies resize() and reshape() functions
  this->m_block_length = p_length / p_no_of_blocks +1;
  this->m_block_size = this->m_block_length * sizeof (T);

  for(uint16_t i=0; i<p_no_of_blocks; i++)
    *(this->m_block_array + i) = this->m_allocator ?
          this->m_allocator->Allocate(this->m_block_length * sizeof (T)) :
          static_cast<void*>(new T[this->m_block_length]);

  this->m_allocated = true;
};

template <typename T>
void BlockHeapMemory<T>::Deallocate()
{
  for(uint16_t i=0; i<this->m_no_of_blocks; i++)
  {
    delete[] (T*)(*(this->m_block_array + i));
  }

  delete[] (this->m_block_array);

  this->m_length = 0;
  this->m_size = 0;
  this->m_no_of_blocks = BLOCKMEMORY_DEFAULT_NO_OF_BLOCKS;
  this->m_allocated = false;
};

template <typename T>
void BlockHeapMemory<T>::LoadFromFile(const char* p_file_path)
{ LoadFromFile(p_file_path, this->m_no_of_blocks); };

template <typename T>
void BlockHeapMemory<T>::LoadFromFile(const char* p_file_path, const uint16_t p_no_of_blocks)
{
  auto input_file = std::fstream(p_file_path, std::ios::in | std::ios::binary | std::ios::ate);
  size_t length = input_file.tellg()/sizeof(T);
  Resize(length);
  Reshape(p_no_of_blocks);

  for (uint16_t i=0; i<this->m_no_of_blocks; i++)
  {
    void* buffer = *(this->m_block_array + i);
    input_file.seekg( i*(this->m_block_size), std::ios::beg);
    input_file.read((char*)buffer, this->m_block_size);
  }
  input_file.close();
};

template <typename T>
void BlockHeapMemory<T>::Resize(const size_t p_length)
{
  if (p_length == this->m_length) return;
  if (p_length == 0) {Deallocate(); return;}

  // Check whether avaible free memory is enough for resizing
  if ((p_length / (this->m_no_of_blocks) + (p_length % (this->m_no_of_blocks) ? 1 : 0))
      == this->m_block_length)
  {
    this->m_length = p_length;
    return;
  }

  if (this->m_allocated)
  {
    int64_t extra_memory_bytes = static_cast<int64_t>(p_length - this->m_length) * sizeof (T);

    void** previous_block_array = this->m_block_array;
    uint16_t previous_no_of_blocks = this->m_no_of_blocks;

    int16_t no_of_new_blocks = (int16_t)(extra_memory_bytes / (int64_t)(this->m_block_size));
    no_of_new_blocks += p_length > this->m_length ? 1 : 0;

    if (no_of_new_blocks != 0)
    {
      this->m_no_of_blocks += no_of_new_blocks;

      this->m_block_array = new void*[this->m_no_of_blocks];

      if (p_length > this->m_length)
      {
        memcpy(this->m_block_array,
               previous_block_array,
               previous_no_of_blocks * sizeof (void*));

        for (uint16_t i = previous_no_of_blocks; i < this->m_no_of_blocks; i++)
          this->m_block_array[i] = new T[this->m_block_size];
      }
      else
      {
        memcpy(this->m_block_array,
               previous_block_array,
               this->m_no_of_blocks * sizeof (void*));

        for (uint16_t i = previous_no_of_blocks-1; i >= this->m_no_of_blocks; i--)
          delete (T*)previous_block_array[i];
      }

      delete previous_block_array;
    }

    this->m_length = p_length;
    this->m_size = this->m_no_of_blocks * this->m_block_size;

  }
  else
  {
    Allocate(p_length, this->m_no_of_blocks);
  }

};

template <typename T>
void BlockHeapMemory<T>::Reshape(const uint16_t p_no_of_blocks)
{
  if (p_no_of_blocks == 0) {return;}
  if (p_no_of_blocks == this->m_no_of_blocks) {return;}

  uint16_t old_no_of_blocks = this->m_no_of_blocks;
  size_t old_block_size = this->m_block_size;
  void** old_block_array = this->m_block_array;

  Allocate(this->m_length, p_no_of_blocks);

  size_t old_index = 0;
  size_t new_index = 0;
  size_t copied_bytes = 0;
  void* source = nullptr;
  void* destination = nullptr;
  size_t copy_size = 0;
  size_t current_pos = 0;
  size_t next_pos = 0;

  while(copied_bytes < this->m_size)
  {

    size_t next_new_block_edge = (new_index + 1) * this->m_block_size;
    size_t next_old_block_edge = (old_index + 1) * old_block_size;

    size_t new_old_margin = next_new_block_edge - copied_bytes;
    size_t old_new_margin = next_old_block_edge - copied_bytes;

    next_pos = next_new_block_edge >= next_old_block_edge
        ?  next_old_block_edge : next_new_block_edge;

    copy_size = next_pos - current_pos;

    if (copy_size == old_block_size)
    {
      if (current_pos + 2*copy_size < next_old_block_edge)
      {
        source = *(old_block_array + old_index);
        destination = *(this->m_block_array + new_index);
        new_index++;
      }
      else
      {
        size_t internal_index = copied_bytes % (this->m_block_size);
        source = *(old_block_array + old_index);
        destination = (char*)*(this->m_block_array + new_index) + internal_index;
        old_index++;
      }
    }
    else if (copy_size == this->m_block_size)
    {
      if (current_pos + 2*copy_size < next_new_block_edge)
      {
        source = *(old_block_array + old_index);
        destination = *(this->m_block_array + new_index);
        old_index++;
      }
      else
      {
        size_t internal_index = copied_bytes % old_block_size;
        source = (char*)*(old_block_array + old_index) + internal_index;
        destination = *(this->m_block_array + new_index);
        new_index++;
      }
    }
    else if (copy_size == new_old_margin)
    {
      source = *(old_block_array + old_index);
      destination = (char*)(*(this->m_block_array + new_index)) + (this->m_block_size - copy_size);
      new_index++;
    }
    else if (copy_size == old_new_margin)
    {
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
    delete[] (T*)(*(old_block_array + i));
  }

  delete[] old_block_array;

};

template <typename T>
T& BlockHeapMemory<T>::operator [] (const size_t p_index)
{
  size_t block_index = p_index / (this->m_block_length);
  size_t in_block_index = p_index % (this->m_block_length);

  void* block_address = *(this->m_block_array + (block_index));
  return *((T*)block_address + (in_block_index));
};

template <typename T>
const T& BlockHeapMemory<T>::operator [] (const size_t p_index) const
{
  size_t block_index = p_index / (this->m_block_length);
  size_t in_block_index = p_index % (this->m_block_length);

  void* block_address = *((this->m_block_array) + (block_index));
  return *((T*)block_address + (in_block_index));
};

