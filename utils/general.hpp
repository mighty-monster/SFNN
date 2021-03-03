// File Name:     general.hpp
// Author:        Arash Fatehi
// Date:          19th Feb 2021
// Description:   Contains general fucntions and macros

#pragma once

#include "configs.hpp"
#include "platform.hpp"
#include "logger.hpp"

#include <cinttypes>
#include <string>
#include <cassert>
#include <iostream>
#include <stdio.h>

// Helps to get rid of unused variable warnings when dealing with
// cross-platform functions
#define NNEUSE(_exp) (void)_exp;

// Prints a message to std::cout and goes to next line
#define NNE_PRINTL(_msg) std::cout<<_msg<<std::endl

// Defining __NNEFUNC__ as the macro that returns the functions signature
// on MSVC, GCC, and Clang
#ifdef NNEXCEPT_FULL_FUNCNAME
  #ifdef _MSC_VER
    #define __NNEFUNC__ __FUNCSIG__
  #elif defined(__GNUC__) || defined(__clang__)
    #define __NNEFUNC__ __PRETTY_FUNCTION__
  #endif
#else
  #define __NNEFUNC__ __func__
#endif

// Defining __fucn__ macro in case of MSVC compiler
#ifdef _MSC_VER
  #define __func__ __FUNCTION__
#endif

// Logs the error using Logger class as per what an object of
// class NNExcept describes, should be used in catch blocks
#define NNE_ERORR_EX(_ex) \
  nne::LogError(_ex.File(), _ex.Line(), _ex.Func(), _ex.What())

// Logs low level error
// This macro logs the errors using std::cerr, it should only be used for Logger class
// and in memotitor.hpp files
// Streams don't throw exceptions by default, so using std::cerr is quiet safe here
#define NNE_ERORR_LL(_msg) \
  nne::LogErrorLL(__FILE__, __LINE__, __NNEFUNC__, _msg)

// Logs error using Logger class
#define NNE_ERORR(_msg)  \
  nne::LogError(__FILE__, __LINE__, __NNEFUNC__, _msg)


// The macro "NNE_ERRNO_PRINT_REASON" prints description about _errno code, it is platform
// and compiler specific
// On window "strerror_s" is used
// On unix "strerror_r" is used, but two implementation exits with different functionalities
// The GNU version is used when __USE_GNU is defined, otherwise Posix version will be used
#ifdef _WIN32
  #define NNE_ERRNO_PRINT_REASON(_reason, _reason_length, _errno) \
  if ( strerror_s(_reason, _reason_length, _errno) == 0 ) \
  { \
   NNE_ERORR(_reason); \
  }
#elif defined __USE_XOPEN2K && !defined __USE_GNU
  #define NNE_ERRNO_PRINT_REASON(_reason, _reason_length, _errno) \
  if ( strerror_r( _errno, _reason, _reason_length) == 0 ) \
  { \
   NNE_ERORR(_reason); \
  }
#else
  #define NNE_ERRNO_PRINT_REASON(_reason, _reason_length, _errno) \
  NNE_ERORR(strerror_r( _errno, _reason, _reason_length));

#endif

namespace nne {
  // __FUNC__, __FUCNTION__, __FUNCSIG__, and __PRETTY_FUNCTION__ are not macros,
  // they are constant static char* variables, to add function name to
  // logged error, need to use a function in combination to a macro
  void LogError(const char* p_file, int p_line, const char* p_function_name, const char* p_message) noexcept;

  void LogErrorLL(const char* p_file, int p_line, const char* p_function_name, const char* p_message) noexcept;

  void GetCurrentTime(char* p_date_time_str) noexcept;

  // Dumps a block of memory as Hex string
  std::string BufferToHex(void* p_buffer, size_t p_size);

  // Loads an hex string into a block of memory
  // The buffer should have enough allocated memory
  void HexToBuffer(void* p_buffer, const std::string& p_hex);
  void HexToBuffer(void* p_buffer, const char* p_hex, const size_t p_hex_size);

  // Convert number of bytes to Kilo Byte, Mega Byte, Giga Byte, etc
  void BytesToHumanReadableSize(uint64_t p_size, char* p_result, const size_t p_result_size) noexcept;

  const char* ExtractFilenameFromPath(char* p_path) noexcept;

  // Compiler independent strcat
  // functions strcpy_nne, strcat_nne are used in error handling mechanisms in
  // Logger class (used for reporting and logging errors as well), and also in reporting errors
  // within Logger and memon.hpp, so if they can throw expection and cause error, things gets ugly
  // Their potential errors are:
  // 1. Memory Access violation which will be handled using signals by OS and will
  // lead to crash anyway, if happens, we already lost :|
  // 2. Not having enough space in "p_dest" buffer, which cause the operation to not performed
  // In case of error 2, it will only log to std::cerr and assert if in debug mode
  // If there is a error in std::cerr, again we have already lost and
  // there is no point in reporting the error for recovery
  // P.S. Streams don't throw exceptions by default
  void strcpy_nne(char* p_dest, size_t p_dest_length, const char* p_src, size_t p_src_length, size_t p_offset = 0) noexcept;
  void strcat_nne(char* p_dest, size_t p_dest_length, const char* p_src, size_t p_src_length) noexcept;

  // Compiler independent sprintf
  // Returns a negative number if error happens
  template<typename ... Args>
  int sprintf_nne(char* p_dest, size_t p_dest_length, const char* const p_format, Args ... p_args) noexcept;
}

