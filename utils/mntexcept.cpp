// File Name:     mntexcept.cpp 
// Author:        Arash Fatehi
// Date:          3th Mar 2021
// Description:   Base exception class for Momentum project

#include "mntexcept.hpp"

#include <cstring>

using namespace mnt;

MNTExcept::MNTExcept(const char* _message,const char* _function,const char* _filepath, int _line, int _errno) noexcept
{
  strcpy_mnt(m_message, MNT_EXCEPTION_MESSAGE_SIZE, _message, strlen(_message) + 1);
  strcpy_mnt(m_function, MNT_EXCEPTION_FUNCNAME_SIZE, _function, strlen(_function) + 1);

  // Copy _filepath to a writable buffer, to extract filename
  size_t filepath_buffer_size = strlen(_filepath) + 1;
  char* filepath_buffer = (char*)alloca(filepath_buffer_size);
  strcpy_mnt(filepath_buffer, filepath_buffer_size, _filepath, filepath_buffer_size);

  // Write back the filename into the beginning of filepath_buffer
  ExtractFilenameFromPath(filepath_buffer);

  strcpy_mnt(m_filename, MNT_EXCEPTION_FILENAME_SIZE, filepath_buffer, strlen(filepath_buffer) +1);

  m_line = _line;
  m_errno = _errno;
};

const char* MNTExcept::what() const noexcept
{
  return m_message;
};
