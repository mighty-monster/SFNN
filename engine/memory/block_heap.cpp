
#include "block_heap.hpp"

using namespace nnc;

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(){};

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const char* file_path)
{
  LoadFromFile(file_path);
};

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const char* file_path, const size_t& no_of_blocks)
{
  LoadFromFile(file_path, no_of_blocks);
};


template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const size_t& length)
  : BlockHeapMemory(length, m_no_of_blocks){};


template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const size_t& length, const size_t& no_of_blocks)
{
  if (length > 0)
  {
    Allocate(length, no_of_blocks);
  }
};

template <typename T>
BlockHeapMemory<T>::~BlockHeapMemory()
{
  Deallocate();
};

template <typename T>
void BlockHeapMemory<T>::Allocate(const size_t& length, const size_t& no_of_blocks)
{
  m_no_of_blocks = no_of_blocks;
  m_length = length;
  m_size = length * sizeof (T);

  m_block_array = new void*[no_of_blocks];

  // Allocating one extra byte per blocks simplifies resize() and reshape() functions
  m_block_length = length / no_of_blocks +1;
  m_block_size = m_block_length * sizeof (T);

  for(int i=0; i<no_of_blocks; i++)
  {
    *(m_block_array + i) = static_cast<void*>(new T[m_block_length]);
  }

  m_allocated = true;
};

template <typename T>
void BlockHeapMemory<T>::Deallocate()
{
  for(int i=0; i<m_no_of_blocks; i++)
  {
    delete[] (T*)(*(m_block_array + i));
  }

  delete[] m_block_array;

  m_length = 0;
  m_size = 0;
  m_allocated = false;
};

template <typename T>
void BlockHeapMemory<T>::LoadFromFile(const char* file_path)
{
  LoadFromFile(file_path, m_no_of_blocks);
};

template <typename T>
void BlockHeapMemory<T>::LoadFromFile(const char* file_path, const size_t& no_of_blocks)
{};

template <typename T>
void BlockHeapMemory<T>::LoadFromHexFile(const char* file_path)
{
  LoadFromHexFile(file_path, m_no_of_blocks);
};

template <typename T>
void BlockHeapMemory<T>::LoadFromHexFile(const char* file_path, const size_t& no_of_blocks)
{};

template <typename T>
void BlockHeapMemory<T>::Resize(const size_t& length)
{
  if (length == m_length) return;
  if (length == 0) {Deallocate(); return;}

  if ((length / m_no_of_blocks + (length % m_no_of_blocks ? 1 : 0))
      == m_block_length) return;

  if (m_allocated)
  {
    int64_t extra_memory_bytes = static_cast<int64_t>(length - m_length) * sizeof (T);

    void** previous_block_array = m_block_array;
    size_t previous_no_of_blocks = m_no_of_blocks;

    int64_t no_of_new_blocks = extra_memory_bytes / static_cast<int64_t>(m_block_size);
    no_of_new_blocks += length > m_length ? 1 : 0;


    if (no_of_new_blocks != 0)
    {
      m_no_of_blocks = m_no_of_blocks + no_of_new_blocks;

      m_block_array = new void*[m_no_of_blocks];


      if (length > m_length)
      {
        memcpy(m_block_array,
               previous_block_array,
               previous_no_of_blocks * sizeof (void*));

        for (size_t i = previous_no_of_blocks; i < m_no_of_blocks; i++)
          m_block_array[i] = new T[m_block_size];
      }
      else
      {
        memcpy(m_block_array,
               previous_block_array,
               m_no_of_blocks * sizeof (void*));

        for (size_t i = previous_no_of_blocks-1; i >= m_no_of_blocks; i--)
          delete (T*)previous_block_array[i];
      }

      delete previous_block_array;
    }

    m_length = length;
    m_size = m_no_of_blocks * m_block_size;

  }
  else
  {
    Allocate(length, m_no_of_blocks);
  }

};

template <typename T>
void BlockHeapMemory<T>::Reshape(const size_t& no_of_blocks)
{
  if (no_of_blocks == 0) {return;}
  if (no_of_blocks == m_no_of_blocks) {return;}

  size_t old_no_of_blocks = m_no_of_blocks;
  size_t old_block_size = m_block_size;
  void** old_block_array = m_block_array;

  Allocate(m_length, no_of_blocks);

  size_t old_index = 0;
  size_t new_index = 0;
  size_t copied_bytes = 0;
  void* source = nullptr;
  void* destination = nullptr;
  size_t copy_size = 0;
  size_t current_pos = 0;
  size_t next_pos = 0;

  while(copied_bytes < m_size)
  {

    size_t next_new_block_edge = (new_index + 1) * m_block_size;
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
        destination = *(m_block_array + new_index);
        new_index++;
      }
      else
      {
        size_t internal_index = copied_bytes % m_block_size;
        source = *(old_block_array + old_index);
        destination = (char*)*(m_block_array + new_index) + internal_index;
        old_index++;
      }
    }
    else if (copy_size == m_block_size)
    {
      if (current_pos + 2*copy_size < next_new_block_edge)
      {
        source = *(old_block_array + old_index);
        destination = *(m_block_array + new_index);
        old_index++;
      }
      else
      {
        size_t internal_index = copied_bytes % old_block_size;
        source = (char*)*(old_block_array + old_index) + internal_index;
        destination = *(m_block_array + new_index);
        new_index++;
      }
    }
    else if (copy_size == new_old_margin)
    {
      source = *(old_block_array + old_index);
      destination = (char*)(*(m_block_array + new_index)) + (m_block_size - copy_size);
      new_index++;
    }
    else if (copy_size == old_new_margin)
    {
      source = (char*)(*(old_block_array + old_index)) + (old_block_size - copy_size);
      destination = *(m_block_array + new_index);
      old_index++;
    }


    memcpy(destination, source , copy_size);
    current_pos = next_pos;
    copied_bytes += copy_size;
  }

  for(int i=0; i<old_no_of_blocks; i++)
  {
    delete[] (T*)(*(old_block_array + i));
  }

  delete[] old_block_array;

};

template <typename T>
T& BlockHeapMemory<T>::operator [] (const int64_t& index)
{
  size_t block_index = index / m_block_length;
  size_t in_block_index = index % m_block_length;

  void* block_address = *(m_block_array + (block_index));
  return *((T*)block_address + (in_block_index));
};

template <typename T>
const T& BlockHeapMemory<T>::operator [] (const int64_t& index) const
{
  size_t block_index = index / m_block_length;
  size_t in_block_index = index % m_block_length;

  void* block_address = *(m_block_array + (block_index));
  return *((T*)block_address + (in_block_index));
};

template <typename T>
size_t BlockHeapMemory<T>::Length()
{
  return m_length;
};
