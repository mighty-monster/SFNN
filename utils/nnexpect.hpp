// File Name:     nnexcept.hpp
// Author:        Arash Fatehi
// Date:          3th Mar 2021
// Description:   Base exception class for NNE project

#include "general.hpp"

#include <exception>

//#define NNEXCEPT_FULL_FUNCNAME
//#define NNEXCEPT_FULL_FILENAME

#ifndef _WIN32
  #define NNEXCEPT_FULL_FUNCNAME
  #define NNEXCEPT_FULL_FILENAME
#endif

#define NNE_EXCEPTION_MESSAGE_SIZE 196

#ifdef NNEXCEPT_FULL_FUNCNAME
  #define NNEXCEPT_FUNCNAME __NNEFUNC__
  #define NNE_EXCEPTION_FUNCNAME_SIZE 256
#else
  #define NNEXCEPT_FUNCNAME __func__
  #define NNE_EXCEPTION_FUNCNAME_SIZE 50
#endif

#ifdef NNEXCEPT_FULL_FILENAME
  #define NNE_EXCEPTION_FILENAME_SIZE 512
#else
  #define NNE_EXCEPTION_FILENAME_SIZE 50
#endif

#ifdef NNEXCEPT_FULL_FUNCNAME
  #define __NNEXCEPT_FUNNAME__  __NNEFUNC__
#else
  #define __NNEXCEPT_FUNNAME__ __func__
#endif

#define NNE_THROW(_msg) NNE_THROW_C(_msg, 0)

#define NNE_THROW_C(_msg, _code) \
  throw NNExcept(_msg, __NNEXCEPT_FUNNAME__, __FILE__, __LINE__, _code)

namespace nne {
  class NNExcept : public std::exception
  {
  public:
    explicit NNExcept(const char* p_message,const char* p_function,const char* p_filename, int p_line, int p_errno = 0) noexcept;
    virtual ~NNExcept() noexcept  = default;
    const char* what() const noexcept override;
    const char* What() const noexcept;
    const char* Func() const noexcept;
    const char* File() const noexcept;
    int Line() const noexcept;
    int Code() const noexcept;

  private:
    char m_message[NNE_EXCEPTION_MESSAGE_SIZE];
    char m_function[NNE_EXCEPTION_FUNCNAME_SIZE];
    char m_filename[NNE_EXCEPTION_FILENAME_SIZE];

    int m_line;
    int m_errno;

  };
}

