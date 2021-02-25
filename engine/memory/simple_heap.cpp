#include <cstring>
#include <fstream>

#include "simple_heap.hpp"
#include "utils/general.hpp"

using namespace nne;

template <typename T>
SimpleHeapMemory<T>::SimpleHeapMemory()
{
};

template <typename T>
SimpleHeapMemory<T>::SimpleHeapMemory(const char* file_path)
{
  assert(file_path);
  LoadFromFile(file_path);
};

template <typename T>
SimpleHeapMemory<T>::SimpleHeapMemory(const size_t& length)
{
  if (length > 0)
    Allocate(length);
};

template <typename T>
SimpleHeapMemory<T>::~SimpleHeapMemory()
{
  Deallocate();
};

template <typename T>
void SimpleHeapMemory<T>::Allocate(const size_t& length)
{
  m_length = length;
  m_size = length * sizeof(T);
  m_memory = new T[m_length];
  m_allocated = true;
};

template <typename T>
void SimpleHeapMemory<T>::Deallocate()
{
  delete[] (T*)m_memory;
  m_length = 0;
  m_size = 0;
  m_allocated = false;
};


template <typename T>
void SimpleHeapMemory<T>::LoadFromFile(const char* file_path)
{
  auto input_file = std::fstream(file_path, std::ios::in | std::ios::binary | std::ios::ate);
  Resize(input_file.tellg()/sizeof(T));
  input_file.seekg( 0, std::ios::beg);
  input_file.read((char*)m_memory, m_size);
  input_file.close();
};

template <typename T>
void SimpleHeapMemory<T>::LoadFromHexFile(const char* file_path)
{
  auto input_file = std::fstream(file_path, std::ios::in | std::ios::ate);

  size_t length = input_file.tellg()/sizeof(T)/2;
  Resize(length);

  size_t file_buffer_size = 2*m_size;
  char* file_buffer_string = new char[file_buffer_size];

  input_file.seekg(0, std::ios::beg);
  input_file.read(file_buffer_string, file_buffer_size);

  input_file.close();

  nne::HexToBuffer(m_memory, file_buffer_string, file_buffer_size);

  delete[] file_buffer_string;
};

template <typename T>
void SimpleHeapMemory<T>::Resize(const size_t& length)
{
  if (length == m_length) return;
  if (length == 0) {Deallocate(); return;}

  if (m_allocated)
  {
    void* previous_pointer = m_memory;

    m_memory = new T[length];

    size_t copy_size_bytes = sizeof (T) * (length > m_length ? m_length : length);
    memcpy(m_memory, previous_pointer, copy_size_bytes);

    delete[] (T*)previous_pointer;

    m_length = length;
    m_size = length * sizeof(T);

    m_allocated = true;
  }
  else
  {
    Allocate(length);
  }
};

template <typename T>
T& SimpleHeapMemory<T> ::operator [] (const int64_t& index)
{
  return *((T*)m_memory + index);
};

template <typename T>
const T& SimpleHeapMemory<T> ::operator [] (const int64_t& index) const
{
  return *((T*)m_memory + index);
};

template <typename T>
size_t SimpleHeapMemory<T>::Length()
{
  return m_length;
}




