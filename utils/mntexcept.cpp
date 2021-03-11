// File Name:     mntexcept.cpp 
// Author:        Arash Fatehi
// Date:          3th Mar 2021
// Description:   Base exception class for Momentum project

#include "mntexcept.hpp"

#include <cstring>

using namespace mnt;

MNTExcept::MNTExcept(const char* p_message,const char* p_function,const char* p_filepath, int p_line, int p_errno) noexcept
{
  strcpy_mnt(m_message, MNT_EXCEPTION_MESSAGE_SIZE, p_message, strlen(p_message) + 1);
  strcpy_mnt(m_function, MNT_EXCEPTION_FUNCNAME_SIZE, p_function, strlen(p_function) + 1);

  // Copy p_filepath to a writable buffer, to extract filename
  size_t filepath_buffer_size = strlen(p_filepath) + 1;
  char* filepath_buffer = (char*)alloca(filepath_buffer_size);
  strcpy_mnt(filepath_buffer, filepath_buffer_size, p_filepath, filepath_buffer_size);

  // Write back the filename into the beginning of filepath_buffer
  ExtractFilenameFromPath(filepath_buffer);

  strcpy_mnt(m_filename, MNT_EXCEPTION_FILENAME_SIZE, filepath_buffer, strlen(filepath_buffer) +1);

  m_line = p_line;
  m_errno = p_errno;
};

const char* MNTExcept::what() const noexcept
{
  return m_message;
};
