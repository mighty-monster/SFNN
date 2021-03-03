#pragma once

#define NNEXCEPT_FULL_FUNCNAME

// The size of logger buffer, increase it if needed, global variables will allocate
// on .data section with most of compilers and do not wast the stack or cause heap allocation
#define NNE_LOGGER_BUFFER_GLOBAL 8192

// The length of datetime string, Example: "2021-02-12 12:00:00"
#define NNE_DATETIME_BUFFER_SIZE 20

#define NNE_MEMON_INITIAL_TABLE_SIZE 40

#define NNE_EXCEPTION_MESSAGE_SIZE 256

#ifdef NNEXCEPT_FULL_FUNCNAME
  #define NNE_EXCEPTION_FUNCNAME_SIZE 256
#else
  #define NNE_EXCEPTION_FUNCNAME_SIZE 64
#endif

#define NNE_EXCEPTION_FILENAME_SIZE 64
