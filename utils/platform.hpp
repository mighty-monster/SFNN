// File Name:     platform.hpp
// Author:        Arash Fatehi
// Date:          27th Feb 2021
// Description:   Detects platform information

// ---------------------
// Detail Description:
// Consists of macros and classes that be added as per MNT needs in future
// Currently it only detect the Compiler and OS
// --------------------

// ---------------------
// OS: Windows, Unix
// Compiler: GCC, Clang, MSVC, MinGW
// --------------------

#ifndef UTILS_PLATFORM_HPP
#define UTILS_PLATFORM_HPP

#if defined(_WIN32)
  #if defined(_MSC_VER)
    #define MNT_WIN_MSVC
  #elif defined(__clang__) || defined(__clang_version__)
    #define MNT_WIN_CLANG
  #elif defined(__MINGW32__) || defined(__GNUC__)
    #define MNT_WIN_MINGW
  #endif
#elif defined(__unix__)
  #if defined(__GNUC_MINOR__)
    #define MNT_UNIX_GCC
  #elif defined(__clang_version__)
    #define MNT_UNIX_CLANG
  #endif
#endif

#endif


