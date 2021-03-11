// File Name:     mntexcept.hpp
// Author:        Arash Fatehi
// Date:          3th Mar 2021
// Description:   Base exception class for MNT project

// --------------------
// Detail Description:
// The only reason exceptions are chosen for error handling is they way new[] works
// otherwise, errno would be enough, the problem rises when multiple constructors are
// called by new[] and it is not possible to take any action if something happens in one
// of them unless new[] call is finished, which means errno is overwriten multiple times
// it is possible to migtate this problem by usesing combination of malloc and placement new
// but it`s too anti-pattern, when C++ RAII and OOP features are used, let accept the overhead
// of the design. But be careful to keep everything simple and safe.
// Currently (4th Mar 2021) only one type of exception class is enough for error handling,
// for cuda and OpenCL, maybe separate class be beneficial
// --------------------

// --------------------
// Note:
// Exceptions play role in both error reporting and handling, low-level classes and fucntions
// in utils like Logger and Timer don`t use exception on rely on errno, but excpet those classes
// all others, should use MNTxcept for reporting errors, if need to use C style functions are used
// include the errno in the exception, this way a unify error handling method will be used
// throw all high-level parts of software
// --------------------

// --------------------
// Note:
// When designing classes, the destructor, move operation and if necessary swap function should
// declare noexcept and any potential exception should be handled internaly
// --------------------

// --------------------
// Note:
// Fixed buffers are used to store MNTxcept data in stack to prevent any potential exception
// in calling new and also boosting performance by using just stack varibales
// The size of buffer for m_message, m_function and m_filename is defined in configs.h file
// By using stack variables, default move and copy constructors are safe to handle move and copy
// --------------------

// --------------------
// Note:
// By default m_message buffer`s size is 256 bytes, increase the buffer size if need more
// --------------------

#pragma once

#include "configs.hpp"
#include "general.hpp"

#include <exception>

// The macro to throw new exception in case of error
#define MNT_THROW(_msg) MNT_THROW_C(_msg, 0)

// The macro to include error code in the thrown exception
#define MNT_THROW_C(_msg, _code) \
  throw mnt::MNTExcept(_msg, __MNTFUNC__, __FILE__, __LINE__, _code)

namespace mnt {
  class MNTExcept : public std::exception
  {
  public:
    explicit MNTExcept(const char* p_message, 
                       const char* p_function, 
                       const char* p_filepath, 
                       int p_line, 
                       int p_errno) noexcept;

    virtual ~MNTExcept() noexcept  = default;

    // Implementing the original what() function to stay API compatible with std::exception
    const char* what() const noexcept override;

  private:
    char m_message[MNT_EXCEPTION_MESSAGE_SIZE];
    char m_function[MNT_EXCEPTION_FUNCNAME_SIZE];
    char m_filename[MNT_EXCEPTION_FILENAME_SIZE];

    int m_line;
    int m_errno;

  public:

    // =====
    // Additional inline functions that provide more information,
    // like filename, fucntion name and line
    // -----
    // Note: inline functions can not be defines in separate file
    // =====

    inline const char* What() const noexcept {return what();};
    inline const char* Func() const noexcept {return m_function;};
    inline const char* File() const noexcept {return m_filename;};
    inline int Line() const noexcept {return m_line;};
    inline int Code() const noexcept {return m_errno;};

  };
}

