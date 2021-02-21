///////////////////////////////////////////////////////////////////////////////
// @File Name:     general.hpp                                               //
// @Author:        Arash Fatehi                                              //
// @L.M.D:         19th Feb 2021                                             //
// @Description:   Contains general useful fucntions                         //
//                                                                           //
// Detail Description:                                                       //
//                                                                           //
// List of functions: BufferToHex, HexToBuffer                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <cinttypes>

#include "general.hpp"
#include "logger.hpp"

// Dumps a block of memory as Hex string
std::string nnc::BufferToHex(void* buffer, size_t size)
{
  std::stringstream string_stream;
  string_stream << std::hex;

  for(size_t i=0; i < size; i++)
    string_stream << std::setw(2)
                  << std::setfill('0')
                  << (int)((uint8_t*)buffer)[i];

  return string_stream.str();
}

// Loads an Hex string into a block of memory
// The buffer should have enough allocated memory
// Length of Hex string determines size of buffer
void nnc::HexToBuffer(void* buffer, const std::string& hex)
{
  // This implementation is not optimized, should change it if
  // the function is used in performance critical sections of code

  // Hex string should have even number of characters
  if (hex.size() % 2)
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

    for(uint64_t i=0; i < hex.size(); i=i+2)
    {
      // Two hex letters contain one byte of data
      temp_str[0] = hex[i]; temp_str[1] = hex[i+1];

      // Convert the hex value to unsigned integer
      uint8_t temp_int = std::stoi(temp_str, 0, 16);

      uint64_t index = (i+1)/2;

      // Fill memory with content of unsigned int
      // corresponding to hex letters
      ((uint8_t*)buffer)[index] = temp_int;
    }
  }
}
