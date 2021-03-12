// File Name:     general.hpp
// Author:        Arash Fatehi
// Date:          19th Feb 2021
// Description:   Contains general fucntions and macros

#ifndef UTILS_GENERAL_HPP
#define UTILS_GENERAL_HPP

#include "configs.hpp"
#include "platform.hpp"
#include "logger.hpp"
#include "timer.hpp"

#include <cinttypes>
#include <string>
#include <cassert>
#include <iostream>
#include <stdio.h>

// Helps to get rid of unused variable warnings when dealing with
// cross-platform functions
#define MNTUSE(_exp) (void)_exp;

// Prints a message to std::cout and goes to next line
#define MNT_PRINTL(_msg) std::cout<<_msg<<std::endl

// Prints execution time from declaration till end of scope at Debug log level
#define MNT_PRINT_DUR mnt::Timer mnt_print_dur_timer;

// Defining __MNTFUNC__ as the macro that returns the functions signature
// on MSVC, GCC, and Clang
#ifdef MNTEXCEPT_FULL_FUNCNAME
  #ifdef _MSC_VER
    #define __MNTFUNC__ __FUNCSIG__
  #elif defined(__GNUC__) || defined(__clang__)
    #define __MNTFUNC__ __PRETTY_FUNCTION__
  #endif
#else
  #define __MNTFUNC__ __func__
#endif

// Defining __fucn__ macro in case of MSVC compiler
#ifdef _MSC_VER
  #define __func__ __FUNCTION__
#endif


// Macros to use Logger class out of the box
#define MNT_WARN(_msg) mnt::Logger::Warn(_msg)
#define MNT_INFO(_msg) mnt::Logger::Info(_msg)
#define MNT_DEBUG(_msg) mnt::Logger::Debug(_msg)

// Logs the error using Logger class as per what an object of
// class MNTxcept describes, should be used in catch blocks
#define MNT_ERORR_EX(_ex) \
  mnt::LogError(_ex.File(), _ex.Line(), _ex.Func(), _ex.What())

// Logs low level error
// This macro logs the errors using std::cerr, it should only be used for Logger class
// and in memotitor.hpp files
// ----
// For error reporting we use our Logger mechanism, but it is quiet high leve,
// for example support strings, etc. if something goes wrong within low-level mechanism
// like Logger and memon.hpp, mnt::LogErrorLL function is used to report directly to std::cerr
#define MNT_ERORR_LL(_msg) \
  mnt::LogErrorLL(__FILE__, __LINE__, __MNTFUNC__, _msg)

// Logs error using Logger class
#define MNT_ERORR(_msg)  \
  mnt::LogError(__FILE__, __LINE__, __MNTFUNC__, _msg)

// The macro "MNT_ERRNO_PRINT_REASON" prints description about _errno code, it is platform
// and compiler specific
// On window "strerror_s" is used
// On unix "strerror_r" is used, but two implementation exits with different functionalities
// The GNU version is used when __USE_GNU is defined, otherwise Posix version will be used
#ifdef _WIN32
  #define MNT_ERRNO_PRINT_REASON(_reason, _reason_length, _errno) \
  if ( strerror_s(_reason, _reason_length, _errno) == 0 ) \
  { \
   MNT_ERORR(_reason); \
  }
#elif defined __USE_XOPEN2K && !defined __USE_GNU
  #define MNT_ERRNO_PRINT_REASON(_reason, _reason_length, _errno) \
  if ( strerror_r( _errno, _reason, _reason_length) == 0 ) \
  { \
   MNT_ERORR(_reason); \
  }
#else
  #define MNT_ERRNO_PRINT_REASON(_reason, _reason_length, _errno) \
  MNT_ERORR(strerror_r( _errno, _reason, _reason_length))
#endif

namespace mnt {
  // __FUNC__, __FUCNTION__, __FUNCSIG__, and __PRETTY_FUNCTION__ are not macros,
  // they are constant static char* variables, to add function name to
  // logged error, need to use a function in combination to a macro
  void LogError(const char* p_filepath, int p_line, const char* p_function_name, const char* p_message) noexcept;

  // Same as "LogError", but used for low level logging to std::cerr
  void LogErrorLL(const char* p_filepath, int p_line, const char* p_function_name, const char* p_message) noexcept;

  // Fills current datatime with format of  YYYY-MM-DD HH:MM:SS
  // in provided buffer, it needs atleast 20 bytes
  void GetCurrentTime(char* p_date_time_str) noexcept;

  // Dumps a block of memory as Hex string
  std::string BufferToHex(void* p_buffer, size_t p_size);

  // Loads an hex string into a block of memory
  // The buffer should have enough allocated memory
  void HexToBuffer(void* p_buffer, const std::string& p_hex);
  void HexToBuffer(void* p_buffer, const char* p_hex, const size_t p_hex_size);

  // Convert number of bytes to Kilo Byte, Mega Byte, Giga Byte, etc
  void BytesToHumanReadableSize(uint64_t p_size, char* p_result, const size_t p_result_size) noexcept;

  // Given the full path of a file, extracts the file name and stores it
  // at the begining of the given buffer, so it act destructive and alter
  // the input
  // ----
  // Note: All used functions are noexcept
  void ExtractFilenameFromPath(char* p_filepath) noexcept;

  // Compiler independent strcat and strcpy
  // functions strcpy_mnt, strcat_mnt are used in error handling mechanisms in
  // Logger class, and also in reporting potential
  // errors within Logger and memon.hpp using "LogErrorLL" function,
  // so if they can throw expection and cause error, things gets ugly
  // ----
  // Note: Their potential errors are
  // 1. Memory Access violation which will be handled using signals by OS and will
  // lead to crash anyway, if happens, we already lost :|
  // 2. Not having enough space in "p_dest" buffer, which causes the operation to not perform at all
  // In case of error 2, it will only log to std::cerr using "LogErrorLL"
  // ----
  // "LogErrorLL" uses strcpy_mnt and vise-versa, but it wont happen if paramters are sane, 
  // so having an error in strcpy_mnt wont cause infite loop of error reporting, 
  // because used parameters in "LogErrorLL" are sane, hopefuly ;p
  // ----
  // P.S. Streams don't throw exceptions by default
  void strcpy_mnt(char* p_dest, size_t p_dest_length, const char* p_src, size_t p_src_length, size_t p_offset = 0) noexcept;
  void strcat_mnt(char* p_dest, size_t p_dest_length, const char* p_src, size_t p_src_length) noexcept;

  // Compiler independent sprintf
  // Returns a negative number if error happens
  template<typename ... Args>
  int sprintf_mnt(char* p_dest, size_t p_dest_length, const char* const p_format, Args ... p_args) noexcept;
}

#endif

