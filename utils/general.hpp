// File Name:     general.hpp
// Author:        Arash Fatehi
// Date:          19th Feb 2021
// Description:   Contains general useful fucntions and macros

#pragma once

#include "platform.hpp"
#include "logger.hpp"

#include <cinttypes>
#include <string>
#include <assert.h>

// Helps to get rid of unused variable warnings when dealing with
// cross-platform functions
#define NNEUSE(_exp) (void)_exp;

// Defining __NNEFUNC__ as the macro that returns the functions signature
// on MSVC, GCC, and Clang
#ifdef _MSC_VER
  #define __NNEFUNC__ __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
  #define __NNEFUNC__ __PRETTY_FUNCTION__
#endif


// This macro logs the error in release mode and cause an assert in debug mode
#define NNERORR(_exp, _msg)  LogError(_exp, __NNEFUNC__, _msg); assert(_exp && _msg);

namespace nne {
  // __FUNC__, __FUCNTION__, __FUNCSIG__, and __PRETTY_FUNCTION__ are not macros,
  // they are constant static char* variables, to add function name to
  // logged error, need to use a function in combination to a macro
  void LogError(bool condition, const char* p_function_name, const char* p_message);

  // Dumps a block of memory as Hex string
  std::string BufferToHex(void* p_buffer, size_t p_size);

  // Loads an hex string into a block of memory
  // The buffer should have enough allocated memory
  void HexToBuffer(void* p_buffer, const std::string& p_hex);
  void HexToBuffer(void* p_buffer, const char* p_hex, const size_t& p_hex_size);

  // Convert number of bytes to Kilo Byte, Mega Byte, Giga Byte, etc
  void BytesToHumanReadableSize(uint64_t p_size, char* p_result, const size_t& p_result_size);

  // Compiler independent strcpy
  int strcpy_nne(char* p_dest, size_t p_dest_length, const char* p_src, size_t p_src_length, size_t p_offset = 0);

  // Compiler independent strcat
  int strcat_nne(char* p_dest, size_t p_dest_length, const char* p_src, size_t p_src_length);

  // Compiler independent sprintf
  template<typename ... Args>
  int sprintf_nne(char* p_dest, size_t p_dest_length, const char* const p_format, Args ... p_args);
}

