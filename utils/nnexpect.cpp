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

#ifdef NNEXCEPT_FULL_FILENAME
  strcpy_nne(m_filename, NNE_EXCEPTION_FILENAME_SIZE, p_filename, strlen(p_filename) + 1);
#else
  const char* last_pos = nullptr;

  // Finding the position of last "\" or "/" charachters in the p_filename
  last_pos = strrchr(p_filename, '\\') ;
  if (!last_pos)
    last_pos = strrchr(p_filename, '/');

  // last_pos contain the pointer to the /, we want to start from the charachter after it
  // so "last_pos - p_filename" is the length of desired substring plus one
  // that is why we didn`t add +1 to include null termination, it is already calculated
  size_t filename_length = strlen(p_filename) - (last_pos - p_filename);

  if (last_pos)
    // Note: used ++last_pos to start copying after "/" till end of string
    strcpy_nne(m_filename, NNE_EXCEPTION_FILENAME_SIZE, ++last_pos, filename_length);
  else
    NNE_ERORR_LL("Invalid filepath string, not including file name in exception object");

#endif

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
