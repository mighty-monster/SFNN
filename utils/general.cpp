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
void nne::LogError(const char* p_function_name, const char* p_message) noexcept
{
  char arrow[] = " --> ";

  // Alocating message memory on stack,
  // One byte extra added for null termination charachter,
  // Note: "strlen()" doesn`t include null termination charachter in reported length
  size_t message_length = strlen(p_function_name) + strlen(arrow) + strlen(p_message) + 1;

  // MSVC doesn`t support "char message[message_length]" as valid statement
  // So "alloca" was used to allocated memory for the message
  char* message = (char*)alloca(message_length);
  memset(message, 0, message_length);

  // This error is too low level to handle, if we can not allocate few bytes on stack, chances
  // are high that we can not proceed anymore, no point in handling the error, just reporting
  if (!message)
  {
    NNELLRORR("Could not allocate memory on stack for the message, so can not proceed");
    return;
  }

  // Null termination charchter is copied in each function call, but rewriten on the
  // next call and only the last null charachter will remain at the end.
  // This is necessary for strcat_nne to work.
  nne::strcpy_nne(message, message_length, p_function_name, strlen(p_function_name) + 1);
  nne::strcat_nne(message, message_length, arrow, strlen(arrow) + 1);
  nne::strcat_nne(message, message_length, p_message, strlen(p_message) + 1);

  Logger::Error(message);
}

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

  try
  {
    if (p_size > exa)
      nne::strcpy_nne(p_result, p_result_size, "? Exabyte(s)", strlen("? Exabyte(s)"));
    else if (p_size > peta)
      nne::sprintf_nne (p_result, p_result_size ,"%0.2f Petabyte(s)", (double)p_size / exa);
    else if (p_size > tera)
      nne::sprintf_nne (p_result, p_result_size, "%0.2f Terabyte(s)", (double)p_size / tera);
    else if (p_size > giga)
      nne::sprintf_nne (p_result, p_result_size, "%0.2f Gigabyte(s)", (double)p_size / giga);
    else if (p_size > mega)
      nne::sprintf_nne (p_result, p_result_size, "%0.2f Megabyte(s)", (double)p_size / mega);
    else if (p_size > kilo)
      nne::sprintf_nne (p_result, p_result_size, "%0.2f Kilobyte(s)", (double)p_size / kilo);
    else
      nne::sprintf_nne (p_result, p_result_size, "%llu Byte(s)", p_size);
  }
  catch (std::exception& ex)
  {
    NNELLRORR(ex.what());
  }
  catch (...)
  {
    NNELLRORR("Unkown exception thrown in BytesToHumanReadableSize(uint64_t, char*, const size_t)");
  }
}

// Compiler independent strcpy
void nne::strcpy_nne(char* p_dest, size_t p_dest_length, const char* p_src, size_t p_src_length, size_t p_offset) noexcept
{
  if (p_offset + p_src_length > p_dest_length)
  {
    NNELLRORR("destination doen`t have enough space");
    return;
  }

  memcpy(p_dest + p_offset, p_src, p_src_length);
}

// Compiler independent strcat
void nne::strcat_nne(char* p_dest, size_t p_dest_length, const char* p_src, size_t p_src_length) noexcept
{
  if (p_src_length + strlen(p_dest) > p_dest_length)
  {
    NNELLRORR("destination doen`t have enough space");
    return;
  }

  strcpy_nne(p_dest, p_dest_length, p_src, p_src_length, strlen(p_dest));
}

// Compiler independent sprintf
template<typename ... Args>
void nne::sprintf_nne(char* p_dest, size_t p_dest_length, const char* const p_format, Args ... p_args)
{
#ifdef NNE_WIN_MSVC
  sprintf_s(p_dest, p_dest_length, p_format, p_args ... );
#else
  NNEUSE(p_dest_length);
  sprintf(p_dest, p_format, p_args ...);
#endif
  //Todo: Check result and Throw Exception
}

