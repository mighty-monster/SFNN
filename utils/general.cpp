// File Name:     general.cpp
// Author:        Arash Fatehi
// Date:          19th Feb 2021
// Description:   Contains general useful fucntions

#include "general.hpp"
#include "logger.hpp"

#include <sstream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <cinttypes>
#include <cstring>

// __FUNC__, __FUCNTION__, __FUNCSIG__, and __PRETTY_FUNCTION__ are not macros,
// they are constant static char* variables, to add function name to
// logged error, need to use a function in combination to a macro
void nne::LogError(const char* p_filepath, int p_line, const char* p_function_name, const char* p_message) noexcept
{
  char separator[] = ", ";
  char colon[] = ":";
  char arrow[] = " --> ";

  // <- Converting line to string
  const uint8_t no_of_digits = 6;
  char line[no_of_digits];
  int result_code = nne::sprintf_nne(line, no_of_digits, "%d", p_line);
  // Just reporting in case of error, can not recover if something goes seriously wrong
  if (result_code < 0)
  {
    NNE_ERORR_LL("Error in nne::sprintf_nne, will not include line of error");
    // setting line to 0, will cause strlen(line) to be zero
    memset(line, 0, no_of_digits);
  }
  // -> Converting line to string

  size_t filepath_buffer_size = strlen(p_filepath) + 1;
  char* filepath_buffer = (char*)alloca(filepath_buffer_size);
  strcpy_nne(filepath_buffer, filepath_buffer_size, p_filepath, filepath_buffer_size);

  // By extracting the filename first, less memory is allocated on stack in enxt steps
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
                          strlen(p_function_name) +
                          strlen(arrow) +
                          strlen(p_message) + 1;

  // MSVC doesn`t support "char message[message_length]" as valid statement
  // So "alloca" was used to allocated memory for the message
  char* message = (char*)alloca(message_length);
  memset(message, 0, message_length);

  // This error is too low level to handle, if we can not allocate few bytes on stack, chances
  // are high that we can not proceed anymore, no point in handling the error, still will try
  // to report the error
  if (!message)
  {
    NNE_ERORR_LL("Could not allocate memory on stack for the message, so can not proceed");
    return;
  }

  // Null termination charchter is copied in each function call, but rewriten on the
  // next call and only the last null charachter will remain at the end.
  // This is necessary for strcat_nne to work.
  nne::strcpy_nne(message, message_length, filepath_buffer, strlen(filepath_buffer) + 1);
  nne::strcat_nne(message, message_length, colon, strlen(colon) + 1);
  nne::strcat_nne(message, message_length, line, strlen(line) + 1);
  nne::strcat_nne(message, message_length, separator, strlen(separator) + 1);
  nne::strcat_nne(message, message_length, p_function_name, strlen(p_function_name) + 1);
  nne::strcat_nne(message, message_length, arrow, strlen(arrow) + 1);
  nne::strcat_nne(message, message_length, p_message, strlen(p_message) + 1);

  Logger::Error(message);
}

// This is the low-level error log function, nothing should goes wrong in here
// If it does, we are done! We can report error if reporting cause error
void nne::LogErrorLL(const char* p_filepath, int p_line, const char* p_function_name, const char* p_message) noexcept
{

  // <- Converting line to string
  const uint8_t no_of_digits = 6;
  char line[no_of_digits];
  int result_code = nne::sprintf_nne(line, no_of_digits, "%d", p_line);
  // Can not recover if something goes seriously wrong, just emptying "line" variable
  if (result_code < 0)
    memset(line, 0, no_of_digits);
  // -> Converting line to string

  // Should copy p_filepath to a temp buffer that is not const
  const size_t filepath_buffer_size = strlen(p_filepath) + 1;
  char* filepath_buffer = (char*)alloca(filepath_buffer_size);
  strcpy_nne(filepath_buffer, filepath_buffer_size, p_filepath, filepath_buffer_size);

  ExtractFilenameFromPath(filepath_buffer);

  char current_time[NNE_DATETIME_BUFFER_SIZE];
  GetCurrentTime(current_time);

  std::cerr << "|" << current_time << "| "
            << "[LLE]: " << filepath_buffer << ":"
            << line << ", " << p_function_name
            << " --> " << p_message << std::endl;
}

// date_time_str size should atleast have 20 bytes
// -----
// Note: "nne::GetCurrentTime" is used in "LogErrorLL" to prevent, infinite recursive
// loops, no error reporting is possible in this function, if somthing goes wrong,
// "Unkown" is the answer
void nne::GetCurrentTime(char* p_date_time_str) noexcept
{
  static const char* m_str_unknown = "Unknown";

  // Receive a time point using chrono API
  auto time_point_now = std::chrono::system_clock::now();

  // Convert time_point to time_t
  std::time_t time_t_now = std::chrono::system_clock::to_time_t(time_point_now);

#if defined(NNE_WIN_MSVC)
  // MSVC prefers localtime_s

  // Convert time_t to tm struct
  // localtime_s returns 0 if succesfull, that`s why the if`s logic is twisted ;)
  struct tm tm_struct;
  if (localtime_s(&tm_struct, &time_t_now))
  {
    strcpy_nne(p_date_time_str, NNE_DATETIME_BUFFER_SIZE, m_str_unknown, strlen(m_str_unknown) + 1);
    return;
  }

  // Format tm struct into the buffer
  if (!std::strftime(p_date_time_str,
                     NNE_DATETIME_BUFFER_SIZE,
                     "%Y-%m-%d %H:%M:%S",
                     &tm_struct))

#else
  // GCC doesn`t suport localtime_s, others might not as well

  // Convert time_t to tm struct
  struct tm* tm_struct = localtime(&time_t_now);
  if (!tm_struct)
  {
     strcpy_nne(p_date_time_str, NNE_DATETIME_BUFFER_SIZE, m_str_unknown, strlen(m_str_unknown) + 1);
    return;
  }

  // Format tm struct into the buffer
  if (!std::strftime(p_date_time_str,
                     NNE_DATETIME_BUFFER_SIZE,
                     "%Y-%m-%d %H:%M:%S",
                     tm_struct))
#endif
  {
    strcpy_nne(p_date_time_str, NNE_DATETIME_BUFFER_SIZE, m_str_unknown, strlen(m_str_unknown) + 1);
  }
};

// Dumps a block of memory as Hex string
std::string nne::BufferToHex(void* p_buffer, size_t p_size)
{
  std::stringstream string_stream;
  string_stream << std::hex;

  for(size_t i=0; i < p_size; i++)
    string_stream << std::setw(2)
                  << std::setfill('0')
                  << (int)((uint8_t*)p_buffer)[i];

  return string_stream.str();
}

// Loads an Hex string into a block of memory
// The buffer should have enough allocated memory
void nne::HexToBuffer(void* p_buffer, const std::string& p_hex)
{
  // This implementation is not optimized, should change it if
  // the function is used in performance critical sections of code

  // Hex string should have even number of characters
  if (p_hex.size() % 2 != 0)
  {
    //Todo: Add error message
  }
  else
  {
    // Act as a stack allocated buffer, because short strings
    // allocate memory on stack in release mode
    std::string temp_str("01");

    for(uint64_t i=0; i < p_hex.size(); i=i+2)
    {
      // Two hex letters contain one byte of data
      temp_str[0] = p_hex[i]; temp_str[1] = p_hex[i+1];

      // Convert the hex value to unsigned integer
      uint8_t temp_int = std::stoi(temp_str, 0, 16);

      uint64_t index = (i+1)/2;

      // Fill memory with content of unsigned int
      // corresponding to hex letters
      ((uint8_t*)p_buffer)[index] = temp_int;
    }
  }
}

// Loads an Hex string into a block of memory
// The buffer should have enough allocated memory
void nne::HexToBuffer(void* p_buffer, const char* p_hex, const size_t p_hex_size)
{
  // This implementation is not optimized, should change it if
  // the function is used in performance critical sections of code

  // Hex string should have even number of characters
  if (p_hex_size % 2)
  {
    Logger::Error(std::string(__func__) +
                  " --> Length of hex string should be an even number. "
                  "Nothing was writen into the buffer");
  }
  else
  {
    // Act as a stack allocated buffer, because short strings
    // allocate memory on stack in release mode
    std::string temp_str("01");

    for(uint64_t i=0; i < p_hex_size; i=i+2)
    {
      // Two hex letters contain one byte of data
      temp_str[0] = p_hex[i]; temp_str[1] = p_hex[i+1];

      // Convert the hex value to unsigned integer
      uint8_t temp_int = std::stoi(temp_str, 0, 16);

      uint64_t index = (i+1)/2;

      // Fill memory with content of unsigned int
      // corresponding to hex letters
      ((uint8_t*)p_buffer)[index] = temp_int;
    }
  }
}

// Convert  bytes to Kilo Byte, Mega Byte, Giga Byte, etc
void nne::BytesToHumanReadableSize(uint64_t p_size, char* p_result, const size_t p_result_size) noexcept
{
  const uint64_t exa  = 1000LL*1000*1000*1000*1000*1000;
  const uint64_t peta = 1000LL*1000*1000*1000*1000;
  const uint64_t tera = 1000LL*1000*1000*1000;
  const uint64_t giga = 1000LL*1000*1000;
  const uint64_t mega = 1000LL*1000;
  const uint64_t kilo = 1000LL;

  int result_code = 0;
  if (p_size > exa)
    nne::strcpy_nne(p_result, p_result_size, "? Exabyte(s)", strlen("? Exabyte(s)"));
  else if (p_size > peta)
    result_code = nne::sprintf_nne (p_result, p_result_size ,"%0.2f Petabyte(s)", (double)p_size / exa);
  else if (p_size > tera)
    result_code = nne::sprintf_nne (p_result, p_result_size, "%0.2f Terabyte(s)", (double)p_size / tera);
  else if (p_size > giga)
    result_code = nne::sprintf_nne (p_result, p_result_size, "%0.2f Gigabyte(s)", (double)p_size / giga);
  else if (p_size > mega)
    result_code = nne::sprintf_nne (p_result, p_result_size, "%0.2f Megabyte(s)", (double)p_size / mega);
  else if (p_size > kilo)
    result_code = nne::sprintf_nne (p_result, p_result_size, "%0.2f Kilobyte(s)", (double)p_size / kilo);
  else
    result_code = nne::sprintf_nne (p_result, p_result_size, "%llu Byte(s)", p_size);

  // Just reporting in case of error, can not recover if something goes seroiusly wrong
  if (result_code < 0)
    NNE_ERORR("Error in nne::sprintf_nne");
}

// If applicable copies a substring of the input (filename) into begging of input buffer
// -----
// Note: "strcpy_nne" parameter should 100% be correct in this function, because it is
// used in "LogErrorLL", if fails, we have infinite recursive loop of reporting errors
void nne::ExtractFilenameFromPath(char* p_filepath) noexcept
{
  const char* last_pos = nullptr;

  // Finding the position of last "\" or "/" charachters in the p_filename
  last_pos = strrchr(p_filepath, '\\') ;
  if (!last_pos)
    last_pos = strrchr(p_filepath, '/');

  size_t length;
  if (last_pos)
  {
    // last_pos contain the pointer to the /, we want to start from the charachter after it
    // so "last_pos - p_filename" is the length of desired substring plus one
    // that is why we didn`t add +1 for null termination, it is already calculated
    length = strlen(p_filepath) - (last_pos - p_filepath);
    nne::strcpy_nne(p_filepath, strlen(p_filepath)+1, ++last_pos, length);
  }
}

// Compiler independent strcpy
void nne::strcpy_nne(char* p_dest, size_t p_dest_length, const char* p_src, size_t p_src_length, size_t p_offset) noexcept
{
  if (p_offset + p_src_length > p_dest_length)
  {
    NNE_ERORR_LL("destination doesn`t have enough space");
    return;
  }
  memcpy(p_dest + p_offset, p_src, p_src_length);
}

// Compiler independent strcat
void nne::strcat_nne(char* p_dest, size_t p_dest_length, const char* p_src, size_t p_src_length) noexcept
{
  if (p_src_length + strlen(p_dest) > p_dest_length)
  {
    NNE_ERORR_LL("destination doesn`t have enough space");
    return;
  }

  strcpy_nne(p_dest, p_dest_length, p_src, p_src_length, strlen(p_dest));
}

// Compiler independent sprintf
// Returns a negative number if error happens
template<typename ... Args>
int nne::sprintf_nne(char* p_dest, size_t p_dest_length, const char* const p_format, Args ... p_args) noexcept
{
#ifdef NNE_WIN_MSVC
  // MSVC Complains about using _snprintf
  #pragma warning(disable: 4996)
#endif

#ifdef _WIN32
  #define snprintf _snprintf
#endif

  // unix snprintf returns length output would actually require;
  // windows _snprintf returns actual output length if output fits, else negative
  int needed_length = snprintf( p_dest, p_dest_length, p_format, p_args ... );
  if (needed_length >= (int)p_dest_length)
    needed_length = -1;

  return needed_length;
}

