// File Name:     general.cpp
// Author:        Arash Fatehi
// Date:          19th Feb 2021
// Description:   Contains general useful fucntions


#include <sstream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <cinttypes>
#include <cstring>

#include "general.hpp"
#include "logger.hpp"

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
  if (p_hex.size() % 2)
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
void nne::HexToBuffer(void* p_buffer, const char* p_hex, const size_t& p_hex_size)
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
void nne::BytesToHumanReadableSize(size_t p_size, char* p_result, const size_t& p_result_size)
{

  uint64_t exa  = 1000LL*1000*1000*1000*1000*1000;
  uint64_t peta = 1000LL*1000*1000*1000*1000;
  uint64_t tera = 1000LL*1000*1000*1000;
  uint64_t giga = 1000LL*1000*1000;
  uint64_t mega = 1000LL*1000;
  uint64_t kilo = 1000LL;

  // Silences unused warning in __unix__ mode where the paramter is not used
  (void)p_result_size;

#ifdef WIN32

  if (p_size > exa)
    strncpy_s(p_result, p_result_size, "? Exabyte(s)", strlen("? Exabyte(s)"));
  else if (p_size > peta)
    sprintf_s (p_result, p_result_size ,"%0.2f Petabyte(s)", (double)p_size / exa);
  else if (p_size > tera)
    sprintf_s (p_result, p_result_size, "%0.2f Terabyte(s)", (double)p_size / tera);
  else if (p_size > giga)
    sprintf_s (p_result, p_result_size, "%0.2f Gigabyte(s)", (double)p_size / giga);
  else if (p_size > mega)
    sprintf_s (p_result, p_result_size, "%0.2f Megabyte(s)", (double)p_size / mega);
  else if (p_size > kilo)
    sprintf_s (p_result, p_result_size, "%0.2f Kilobyte(s)", (double)p_size / kilo);
  else
    sprintf_s (p_result, p_result_size, "%llu Byte(s)", p_size);

#elif __unix__

  if (p_size > exa)
    sprintf(p_result, "? Exabyte(s)");
  else if (p_size > peta)
    sprintf(p_result, "%0.2f Petabyte(s)", (double)p_size / exa);
  else if (p_size > tera)
    sprintf(p_result, "%0.2f Terabyte(s)", (double)p_size / tera);
  else if (p_size > giga)
    sprintf(p_result, "%0.2f Gigabyte(s)", (double)p_size / giga);
  else if (p_size > mega)
    sprintf(p_result, "%0.2f Megabyte(s)", (double)p_size / mega);
  else if (p_size > kilo)
    sprintf(p_result, "%0.2f Kilobyte(s)", (double)p_size / kilo);
  else
    sprintf(p_result, "%lu Byte(s)", p_size);

#endif
}
