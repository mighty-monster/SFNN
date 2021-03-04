// File Name:     nnexcept.cpp
// Author:        Arash Fatehi
// Date:          3th Mar 2021
// Description:   Base exception class for NNE project

#include "nnexpect.hpp"

#include <cstring>

using namespace nne;

NNExcept::NNExcept(const char* p_message,const char* p_function,const char* p_filepath, int p_line, int p_errno) noexcept
{
  strcpy_nne(m_message, NNE_EXCEPTION_MESSAGE_SIZE, p_message, strlen(p_message) + 1);
  strcpy_nne(m_function, NNE_EXCEPTION_FUNCNAME_SIZE, p_function, strlen(p_function) + 1);

  // Copy p_filepath to a writable buffer, to extract filename
  size_t filepath_buffer_size = strlen(p_filepath) + 1;
  char* filepath_buffer = (char*)alloca(filepath_buffer_size);
  strcpy_nne(filepath_buffer, filepath_buffer_size, p_filepath, filepath_buffer_size);

  // Write back the filename into the beginning of filepath_buffer
  ExtractFilenameFromPath(filepath_buffer);

  strcpy_nne(m_filename, NNE_EXCEPTION_FILENAME_SIZE, filepath_buffer, strlen(filepath_buffer) +1);

  m_line = p_line;
  m_errno = p_errno;
};

const char* NNExcept::what() const noexcept
{
  return m_message;
};
