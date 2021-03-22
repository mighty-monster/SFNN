// File Name:     simple_heap.inl
// Author:        Arash Fatehi
// Date:          27th Feb 2021
// Description:   Typed linear heap memory class

#ifndef ENGINE_MEMORY_LINEAR_HEAP_INL
#define ENGINE_MEMORY_LINEAR_HEAP_INL

#include "memory/linear/linear_heap.hpp"

#include "utils/general.hpp"

#include <cstring>
#include <fstream>

using namespace mnt;

template <typename T>
LinearHeapMemory<T>::LinearHeapMemory(const char* _file_path)
{
  LoadFromFile(_file_path);
};

template <typename T>
LinearHeapMemory<T>::LinearHeapMemory(const size_t _length)
{
  if (_length > 0)
    Allocate(_length);
};

template <typename T>
LinearHeapMemory<T>::~LinearHeapMemory() noexcept
{
  Deallocate();
};

// Provides strong exception safety
template <typename T>
void LinearHeapMemory<T>::Allocate(const size_t _length)
{

  this->m_memory = this->m_allocator ?
          (this->m_allocator->Allocate(_length * sizeof(T))) :
          new T[_length];

  if (!this->m_memory)
    MNT_THROW("new operation failed, this is a severe error!");

  this->m_length = _length;
  this->m_size = _length * sizeof(T);
  this->m_allocated = true;
};

template <typename T>
void LinearHeapMemory<T>::Deallocate() noexcept
{

  this->m_allocator ?
            this->m_allocator->Deallocate(this->m_memory) :
            delete[] (T*)this->m_memory;

  this->m_length = 0;
  this->m_size = 0;
  this->m_allocated = false;
};

// Provides basic exception safety
template <typename T>
void LinearHeapMemory<T>::LoadFromFile(const char* _file_path)
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

  size_t previous_length = this->m_length;

  try
  {
    Resize(file_size/sizeof(T));
  }
  catch (std::exception& ex)
  {
    input_file.close();
    throw;
  }

  input_file.seekg( 0, std::ios::beg);
  input_file.read((char*)this->m_memory, this->m_size);
  if (input_file.fail())
  {
    input_file.close();

    Resize(previous_length);

    MNT_THROW_C( "failed to read the file", errno);
  }

  input_file.close();
};

// "Resize" function provides strong exception safety
template <typename T>
void LinearHeapMemory<T>::Resize(const size_t _length)
{
  if (_length == this->m_length) return;
  if (_length == 0) {Deallocate(); return;}

  if (this->m_allocated)
  {
    void* previous_pointer = this->m_memory;

    this->m_memory = new T[_length];

    size_t copy_size_bytes = sizeof (T) * (_length > this->m_length ? this->m_length : _length);
    memcpy(this->m_memory, previous_pointer, copy_size_bytes);

    delete[] (T*)previous_pointer;

    this->m_length = _length;
    this->m_size = _length * sizeof(T);

    this->m_allocated = true;
  }
  else
  {
    Allocate(_length);
  }
};

#endif




