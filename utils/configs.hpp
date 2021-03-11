// File Name:     configs.hpp
// Author:        Arash Fatehi
// Date:          4th Mar 2021
// Description:   Compile time configuration for Utils module

// If defined the full signature of fucntions will be used while logging
// otherwise just the name of function
#define MNTEXCEPT_FULL_FUNCNAME

// The size of logger buffer, increase it if needed, global variables will allocate
// on .data section with most of compilers and do not wast the stack or cause heap allocation
#define MNT_LOGGER_BUFFER_GLOBAL 8192

// The length of datetime string, Example: "2021-02-12 12:00:00"
#define MNT_DATETIME_BUFFER_SIZE 20

// The intial size of table that stores allocation records
// Note: Size will increase automatically when needed at runtime
#define MNT_MEMON_INITIAL_TABLE_SIZE 40

// The NEException class has fixd size buffer sizes for message,
// function name and filename, if needed increase buffer size here
// Note: the objects store the data on stack, so be careful with these values
#define MNT_EXCEPTION_MESSAGE_SIZE 256
#ifdef MNTEXCEPT_FULL_FUNCNAME
  #define MNT_EXCEPTION_FUNCNAME_SIZE 96
#else
  #define MNT_EXCEPTION_FUNCNAME_SIZE 48
#endif
#define MNT_EXCEPTION_FILENAME_SIZE 64
