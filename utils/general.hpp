// File Name:     general.hpp
// Author:        Arash Fatehi
// Date:          19th Feb 2021
// Description:   Contains general useful fucntions

#pragma once

#include "logger.hpp"

#include <cinttypes>
#include <string>
#include <assert.h>


#ifdef _MSC_VER
  #define __NNEFUNC__ __FUNCSIG__
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
  #define __NNEFUNC__ __PRETTY_FUNCTION__
#endif

#define NNERORR(_exp, _msg)  LogError(_exp, __NNEFUNC__, _msg); assert(_exp && _msg);

namespace nne {
  // __FUNC__ is not a macro, it`s constant static char*, to add function name to
  // error, need to use a function
  void LogError(bool condition, const char* p_function_name, const char* p_message);

  // Dumps a block of memory as Hex string
  std::string BufferToHex(void* p_buffer, size_t p_size);

  // Loads an hex string into a block of memory
  // The buffer should have enough allocated memory
  void HexToBuffer(void* p_buffer, const std::string& p_hex);
  void HexToBuffer(void* p_buffer, const char* p_hex, const size_t& p_hex_size);

  // Convert number of bytes to Kilo Byte, Mega Byte, Giga Byte, etc
  void BytesToHumanReadableSize(size_t p_size, char* p_result, const size_t& p_result_size);
}

