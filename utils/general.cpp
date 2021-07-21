// File Name:     general.cpp
// Author:        Arash Fatehi
// Date:          19th Feb 2021
// Description:   Contains general useful fucntions

#include "general.hpp"
#include "logger.hpp"
#include "mntexcept.hpp"

#include <sstream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <cinttypes>
#include <cstring>

// __FUNC__, __FUCNTION__, __FUNCSIG__, and __PRETTY_FUNCTION__ are not macros,
// they are constant static char* variables, to add function name to
// logged error, need to use a function in combination to a macro
void mnt::LogError(const char* _filepath, int _line, const char* _function_name, const char* _message) noexcept
{
  char separator[] = ", ";
  char colon[] = ":";
  char arrow[] = " --> ";

  // -> Converting line to string
  const uint8_t no_of_digits = 6;
  char line[no_of_digits];
  int result_code = mnt::sprintf_mnt(line, no_of_digits, "%d", _line);
  // Just reporting in case of error, can not recover if something goes seriously wrong
  if (result_code < 0)
  {
    MNT_ERORR_LL("Error in mnt::sprintf_mnt, will not include line of error");
    // setting line to 0, will cause strlen(line) to be zero
    memset(line, 0, no_of_digits);
  }
  // <- Converting line to string

  size_t filepath_buffer_size = strlen(_filepath) + 1;
  char* filepath_buffer = (char*)alloca(filepath_buffer_size);
  strcpy_mnt(filepath_buffer, filepath_buffer_size, _filepath, filepath_buffer_size);

  // By extracting the filename first, less memory is allocated on stack in the next steps
  ExtractFilenameFromPath(filepath_buffer);

  // Alocating message memory on stack,
  // One byte extra added for null termination charachter,
  // ----
  // Note: "strlen()" doesn`t include null termination charachter in reported length
  // Fomrat: filename:line, function --> message
  size_t message_length = strlen(filepath_buffer) +
                          strlen(colon) +
                          strlen(line) +
                          strlen(separator) +
                          strlen(_function_name) +
                          strlen(arrow) +
                          strlen(_message) + 1;

  // MSVC doesn`t support "char message[message_length]" as valid statement
  // So "alloca" was used to allocated memory for the message
  char* message = (char*)alloca(message_length);
  memset(message, 0, message_length);

  // This error is too low level to handle, if we can not allocate few bytes on stack, chances
  // are high that we can not proceed anymore, no point in handling the error, still will try
  // to report the error
  if (!message)
  {
    MNT_ERORR_LL("Could not allocate memory on stack for the message, so can not proceed");
    return;
  }

  // Null termination charchter is copied in each function call, but rewriten on the
  // next call and only the last null charachter will remain at the end.
  // This is necessary for strcat_mnt to work.
  mnt::strcpy_mnt(message, message_length, filepath_buffer, strlen(filepath_buffer) + 1);
  mnt::strcat_mnt(message, message_length, colon, strlen(colon) + 1);
  mnt::strcat_mnt(message, message_length, line, strlen(line) + 1);
  mnt::strcat_mnt(message, message_length, separator, strlen(separator) + 1);
  mnt::strcat_mnt(message, message_length, _function_name, strlen(_function_name) + 1);
  mnt::strcat_mnt(message, message_length, arrow, strlen(arrow) + 1);
  mnt::strcat_mnt(message, message_length, _message, strlen(_message) + 1);

  Logger::Error(message);
}

// This is the low-level error log function, nothing should goes wrong in here
// If it does, we are done! We can report error if reporting cause error
void mnt::LogErrorLL(const char* _filepath, int _line, const char* _function_name, const char* _message) noexcept
{

  // -> Converting line to string
  const uint8_t no_of_digits = 6;
  char line[no_of_digits];
  int result_code = mnt::sprintf_mnt(line, no_of_digits, "%d", _line);
  // Can not recover if something goes seriously wrong, just emptying "line" variable
  if (result_code < 0)
    memset(line, 0, no_of_digits);
  // <- Converting line to string

  // Should copy _filepath to a temp buffer that is not const
  const size_t filepath_buffer_size = strlen(_filepath) + 1;
  char* filepath_buffer = (char*)alloca(filepath_buffer_size);
  strcpy_mnt(filepath_buffer, filepath_buffer_size, _filepath, filepath_buffer_size);

  ExtractFilenameFromPath(filepath_buffer);

  char current_time[MNT_DATETIME_BUFFER_SIZE];
  GetCurrentTime(current_time);

  std::cerr << "|" << current_time << "| "
            << "[LLE]: " << filepath_buffer << ":"
            << line << ", " << _function_name
            << " --> " << _message << std::endl;
}

// date_time_str size should atleast have 20 bytes
// -----
// Note: "mnt::GetCurrentTime" is used in "LogErrorLL" to prevent, infinite recursive
// loops, no error reporting is possible in this function, if somthing goes wrong,
// "Unkown" is the answer
void mnt::GetCurrentTime(char* _date_time_str) noexcept
{
  static const char* m_str_unknown = "Unknown";

  // Receive a time point using chrono API
  auto time_point_now = std::chrono::system_clock::now();

  // Convert time_point to time_t
  std::time_t time_t_now = std::chrono::system_clock::to_time_t(time_point_now);

#if defined(MNT_WIN_MSVC)

  // =====
  // MSVC prefers localtime_s
  // =====

  // Convert time_t to tm struct
  // localtime_s returns 0 if succesfull, that`s why the if`s logic is twisted ;)
  struct tm tm_struct;
  if (localtime_s(&tm_struct, &time_t_now))
  {
    strcpy_mnt(_date_time_str, MNT_DATETIME_BUFFER_SIZE, m_str_unknown, strlen(m_str_unknown) + 1);
    return;
  }

  // Format tm struct into the buffer
  if (!std::strftime(_date_time_str,
                     MNT_DATETIME_BUFFER_SIZE,
                     "%Y-%m-%d %H:%M:%S",
                     &tm_struct))

#else

  // =====
  // GCC doesn`t suport localtime_s, others might not as well
  // =====

  // Convert time_t to tm struct
  struct tm* tm_struct = localtime(&time_t_now);
  if (!tm_struct)
  {
     strcpy_mnt(_date_time_str, MNT_DATETIME_BUFFER_SIZE, m_str_unknown, strlen(m_str_unknown) + 1);
    return;
  }

  // Format tm struct into the buffer
  if (!std::strftime(_date_time_str,
                     MNT_DATETIME_BUFFER_SIZE,
                     "%Y-%m-%d %H:%M:%S",
                     tm_struct))
#endif
  {
    strcpy_mnt(_date_time_str, MNT_DATETIME_BUFFER_SIZE, m_str_unknown, strlen(m_str_unknown) + 1);
  }
};

// Dumps a buffer as an Hex string
std::string mnt::BufferToHex(void* _buffer, size_t _size)
{
  std::stringstream string_stream;
  string_stream << std::hex;

  for(size_t i=0; i < _size; i++)
    string_stream << std::setw(2)
                  << std::setfill('0')
                  << (int)((uint8_t*)_buffer)[i];

  return string_stream.str();
}

// Loads an Hex string into a buffer
// The buffer should have enough allocated memory
void mnt::HexToBuffer(void* _buffer, const std::string& _hex)
{
  // This implementation is not optimized, should change it if
  // the function is used in performance critical sections of code

  // Hex string should have even number of characters
  if (_hex.size() % 2 != 0)
  {
    MNT_THROW("Length of hex string should be even, 2 hex chars = 1 Byte");
  }
  else
  {
    // Act as a stack allocated buffer, because short strings
    // allocate memory on stack in release mode
    std::string tem_str("01");

    for(uint64_t i=0; i < _hex.size(); i=i+2)
    {
      // Two hex letters contain one byte of data
      tem_str[0] = _hex[i]; tem_str[1] = _hex[i+1];

      // Convert the hex value to unsigned integer
      uint8_t tem_int = std::stoi(tem_str, 0, 16);

      uint64_t index = (i+1)/2;

      // Fill memory with content of unsigned int
      // corresponding to hex letters
      ((uint8_t*)_buffer)[index] = tem_int;
    }
  }
}

// Loads an Hex string into a block of memory
// The buffer should have enough allocated memory
void mnt::HexToBuffer(void* _buffer, const char* _hex, const size_t _hex_size)
{
  // This implementation is not optimized, should change it if
  // the function is used in performance critical sections of code

  // Hex string should have even number of characters
  if (_hex_size % 2)
  {
    Logger::Error(std::string(__func__) +
                  " --> Length of hex string should be an even number. "
                  "Nothing was writen into the buffer");
  }
  else
  {
    // Act as a stack allocated buffer, because short strings
    // allocate memory on stack in release mode
    std::string tem_str("01");

    for(uint64_t i=0; i < _hex_size; i=i+2)
    {
      // Two hex letters contain one byte of data
      tem_str[0] = _hex[i]; tem_str[1] = _hex[i+1];

      // Convert the hex value to unsigned integer
      uint8_t tem_int = std::stoi(tem_str, 0, 16);

      uint64_t index = (i+1)/2;

      // Fill memory with content of unsigned int
      // corresponding to hex letters
      ((uint8_t*)_buffer)[index] = tem_int;
    }
  }
}

// Convert  bytes to Kilo Byte, Mega Byte, Giga Byte, etc
void mnt::BytesToHumanReadableSize(uint64_t _size, char* _result, const size_t _result_size) noexcept
{
  const uint64_t exa  = 1000LL*1000*1000*1000*1000*1000;
  const uint64_t peta = 1000LL*1000*1000*1000*1000;
  const uint64_t tera = 1000LL*1000*1000*1000;
  const uint64_t giga = 1000LL*1000*1000;
  const uint64_t mega = 1000LL*1000;
  const uint64_t kilo = 1000LL;

  int result_code = 0;
  if (_size > exa)
    mnt::strcpy_mnt(_result, _result_size, "? Exabyte(s)", strlen("? Exabyte(s)"));
  else if (_size > peta)
    result_code = mnt::sprintf_mnt (_result, _result_size ,"%0.2f Petabyte(s)", (double)_size / exa);
  else if (_size > tera)
    result_code = mnt::sprintf_mnt (_result, _result_size, "%0.2f Terabyte(s)", (double)_size / tera);
  else if (_size > giga)
    result_code = mnt::sprintf_mnt (_result, _result_size, "%0.2f Gigabyte(s)", (double)_size / giga);
  else if (_size > mega)
    result_code = mnt::sprintf_mnt (_result, _result_size, "%0.2f Megabyte(s)", (double)_size / mega);
  else if (_size > kilo)
    result_code = mnt::sprintf_mnt (_result, _result_size, "%0.2f Kilobyte(s)", (double)_size / kilo);
  else
    result_code = mnt::sprintf_mnt (_result, _result_size, "%llu Byte(s)", _size);

  // Just reporting in case of error, can not recover if something goes seroiusly wrong
  if (result_code < 0)
    MNT_ERORR("Error in mnt::sprintf_mnt");
}

// If applicable copies a substring of the input (filename) into begging of input buffer
// -----
// Note: "strcpy_mnt" parameter should 100% be correct in this function, because it is
// used in "LogErrorLL", if fails, we have infinite recursive loop of reporting errors
void mnt::ExtractFilenameFromPath(char* _filepath) noexcept
{
  const char* last_pos = nullptr;

  // Finding the position of last "\" or "/" charachters in the _filename
  last_pos = strrchr(_filepath, '\\') ;
  if (!last_pos)
    last_pos = strrchr(_filepath, '/');

  size_t length;
  if (last_pos)
  {
    // last_pos contain the pointer to the /, we want to start from the charachter after it
    // so "last_pos - _filename" is the length of desired substring plus one
    // that is why we didn`t add +1 for null termination, it is already calculated
    length = strlen(_filepath) - (last_pos - _filepath);
    mnt::strcpy_mnt(_filepath, strlen(_filepath)+1, ++last_pos, length);
  }
}

// Compiler independent strcpy
void mnt::strcpy_mnt(char* _dest, size_t _dest_length, const char* _src, size_t _src_length, size_t _offset) noexcept
{
  if (_offset + _src_length > _dest_length)
  {
    MNT_ERORR_LL("destination doesn`t have enough space");
    return;
  }
  memcpy(_dest + _offset, _src, _src_length);
}

// Compiler independent strcat
void mnt::strcat_mnt(char* _dest, size_t _dest_length, const char* _src, size_t _src_length) noexcept
{
  if (_src_length + strlen(_dest) > _dest_length)
  {
    MNT_ERORR_LL("destination doesn`t have enough space");
    return;
  }

  strcpy_mnt(_dest, _dest_length, _src, _src_length, strlen(_dest));
}

// Compiler independent sprintf
// Returns a negative number if error happens
template<typename ... Args>
int mnt::sprintf_mnt(char* _dest, size_t _dest_length, const char* const _format, Args ... _args) noexcept
{
#ifdef MNT_WIN_MSVC
  // MSVC Complains about using _snprintf
  #pragma warning(disable: 4996)
#endif

#ifdef _WIN32
  #define snprintf _snprintf
#endif

  // unix snprintf returns length output would actually require;
  // windows _snprintf returns actual output length if output fits, else negative
  int needed_length = snprintf( _dest, _dest_length, _format, _args ... );
  if (needed_length >= (int)_dest_length)
    needed_length = -1;

  return needed_length;
}

