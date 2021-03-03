// File Name:     nnexcept.cpp
// Author:        Arash Fatehi
// Date:          3th Mar 2021
// Description:   Base exception class for NNE project

#include "nnexpect.hpp"

#include <cstring>

using namespace nne;

NNExcept::NNExcept(const char* p_message,const char* p_function,const char* p_filename, int p_line, int p_errno) noexcept
{
  strcpy_nne(m_message, NNE_EXCEPTION_MESSAGE_SIZE, p_message, strlen(p_message) + 1);
  strcpy_nne(m_function, NNE_EXCEPTION_FUNCNAME_SIZE, p_function, strlen(p_function) + 1);

  size_t filename_buffer_size = strlen(p_filename) + 1;
  char* filename_buffer = (char*)alloca(filename_buffer_size);
  strcpy_nne(filename_buffer, filename_buffer_size, p_filename, filename_buffer_size);

  const char* last_pos = ExtractFilenameFromPath(filename_buffer);

  strcpy_nne(m_filename, NNE_EXCEPTION_FILENAME_SIZE, filename_buffer, strlen(last_pos) +1);


  m_line = p_line;
  m_errno = p_errno;
};

const char* NNExcept::what() const noexcept
{
  return m_message;
};

const char* NNExcept::What() const noexcept
{
  return what();
};

const char* NNExcept::Func() const noexcept
{
  return m_function;
};

const char* NNExcept::File() const noexcept
{
  return m_filename;
};

int NNExcept::Line() const noexcept
{
  return m_line;
};

int NNExcept::Code() const noexcept
{
  return m_errno;
};
