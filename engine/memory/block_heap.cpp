
#include "block_heap.hpp"

using namespace nnc;

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const char* file_path)
{
  LoadFromFile(file_path);
};

template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const char* file_path, const uint16_t& no_of_blocks)
{
  LoadFromFile(file_path, no_of_blocks);
};


template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const size_t& length)
{
    BlockHeapMemory(length, default_no_of_block);
};


template <typename T>
BlockHeapMemory<T>::BlockHeapMemory(const size_t& length, const uint16_t& no_of_blocks)
{
  if (length > 0)
  {
    m_no_of_blocks = no_of_blocks;
    m_length = length;
    m_size = length * sizeof (T);

    m_allocated = true;
  }
};


template <typename T>
BlockHeapMemory<T>::~BlockHeapMemory()
{};

template <typename T>
void BlockHeapMemory<T>::LoadFromFile(const char* file_path)
{};

template <typename T>
void BlockHeapMemory<T>::LoadFromFile(const char* file_path, uint16_t no_of_blocks)
{};

template <typename T>
void BlockHeapMemory<T>::LoadFromHexFile(const char* file_path)
{};

template <typename T>
void BlockHeapMemory<T>::LoadFromHexFile(const char* file_path, uint16_t no_of_blocks)
{};

template <typename T>
void BlockHeapMemory<T>::Resize(const size_t& length)
{};

template <typename T>
void BlockHeapMemory<T>::Resize(const size_t& length, const uint16_t& block_size)
{};

template <typename T>
T& BlockHeapMemory<T>::operator [] (const int64_t& index)
{};

template <typename T>
const T& BlockHeapMemory<T>::operator [] (const int64_t& index) const
{};

template <typename T>
size_t BlockHeapMemory<T>::Length()
{};
