// File Name:     simple_heap.hpp
// Author:        Arash Fatehi
// Date:          27th Feb 2021
// Description:   Typed linear heap memory class

#include "memory/simple_heap.hpp"

#include "utils/general.hpp"

#include <cstring>
#include <fstream>

using namespace nne;

template <typename T>
SimpleHeapMemory<T>::SimpleHeapMemory()
{};

template <typename T>
SimpleHeapMemory<T>::SimpleHeapMemory(const char* p_file_path)
{
  LoadFromFile(p_file_path);
};

template <typename T>
SimpleHeapMemory<T>::SimpleHeapMemory(const size_t& p_length)
{
  if (p_length > 0)
    Allocate(p_length);
};

template <typename T>
SimpleHeapMemory<T>::~SimpleHeapMemory()
{ Deallocate(); };

template <typename T>
void SimpleHeapMemory<T>::Allocate(const size_t& p_length) noexcept
{
  m_memory = new T[p_length];
  if (!m_memory)
  {
    NNERORR( "new operation failed, this is a severe error!");
    return;
  }

  m_length = p_length;
  m_size = p_length * sizeof(T);
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
void SimpleHeapMemory<T>::LoadFromFile(const char* p_file_path)
{
  auto input_file = std::fstream(p_file_path, std::ios::in | std::ios::binary | std::ios::ate);

  if (input_file.fail())
  {
    NNERORR("file path is not valid");
    return;
  }

  size_t file_size = input_file.tellg();

  if (input_file.fail())
  {
    NNERORR( "failed to get file size");
    return;
  }

  Resize(file_size/sizeof(T));

  input_file.seekg( 0, std::ios::beg);
  input_file.read((char*)m_memory, m_size);
  if (input_file.fail())
  {
    NNERORR( "failed to read the file");
    return;
  }

  input_file.close();
};

template <typename T>
void SimpleHeapMemory<T>::LoadFromHexFile(const char* p_file_path)
{
  auto input_file = std::fstream(p_file_path, std::ios::in | std::ios::ate);

  if (input_file.fail())
  {
    NNERORR( "file path is not valid");
    return;
  }

  size_t length = input_file.tellg()/sizeof(T)/2;
  if (input_file.fail())
  {
    NNERORR("failed to get file size");
    return;
  }

  Resize(length);

  // The whole file is read at once, if used extensively, this should be done in
  // a loop that read the file gradually
  size_t file_buffer_size = 2*m_size;
  char* file_buffer_string = new char[file_buffer_size];

  input_file.seekg(0, std::ios::beg);
  input_file.read(file_buffer_string, file_buffer_size);

  input_file.close();

  nne::HexToBuffer(m_memory, file_buffer_string, file_buffer_size);

  delete[] file_buffer_string;
};

template <typename T>
void SimpleHeapMemory<T>::Resize(const size_t& p_length)
{
  if (p_length == m_length) return;
  if (p_length == 0) {Deallocate(); return;}

  if (m_allocated)
  {
    void* previous_pointer = m_memory;

    m_memory = new T[p_length];

    size_t copy_size_bytes = sizeof (T) * (p_length > m_length ? m_length : p_length);
    memcpy(m_memory, previous_pointer, copy_size_bytes);

    delete[] (T*)previous_pointer;

    m_length = p_length;
    m_size = p_length * sizeof(T);

    m_allocated = true;
  }
  else
  {
    Allocate(p_length);
  }
};

template <typename T>
T& SimpleHeapMemory<T> ::operator [] (const size_t& p_index)
{
  return *((T*)m_memory + p_index);
};

template <typename T>
const T& SimpleHeapMemory<T> ::operator [] (const size_t& p_index) const
{
  return *((T*)m_memory + p_index);
};

template <typename T>
inline size_t SimpleHeapMemory<T>::Length()
{
  return m_length;
}




