// File Name:     nnexcept.hpp
// Author:        Arash Fatehi
// Date:          3th Mar 2021
// Description:   Base exception class for NNE project

#pragma once

#include "configs.hpp"
#include "general.hpp"

#include <exception>

#define NNE_THROW(_msg) NNE_THROW_C(_msg, 0)

#define NNE_THROW_C(_msg, _code) \
  throw nne::NNExcept(_msg, __NNEFUNC__, __FILE__, __LINE__, _code)

namespace nne {
  class NNExcept : public std::exception
  {
  public:
    explicit NNExcept(const char* p_message,const char* p_function, const char* p_filename, int p_line, int p_errno) noexcept;
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

