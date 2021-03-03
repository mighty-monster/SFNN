// File Name:     logger.hpp
// Author:        Arash Fatehi
// Date:          12th Feb 2021
// Description:   Facilitates logging to console and files
// ---------------------
// Detail Description:
// Thread safe logging mechanism for console output and files compatible
// with windows and linux platforms.
// Log Levels: Error, Warning, Info, Debug
// ---------------------
// Help:
// Use "GetInstance()" to recieve a singleton instance of the object and static methods
// "Error", "Warn", "Info", and "Debug" for logging
// ---------------------
// Note:
// The Logger class will instantiate as a local static variable in "GetInstance()"
// It is a global variable that will be allocated on .data section on most compilers
// It wont wast stack memory, also wont cause heap allocations, that is why m_buffer
// used as a global buffer for the class, and any recieved message will
// be decorated in place using m_buffer, it is little bit hacky and also messy
// But I prefer this approach due to stack limitations of windows
// Heap allcoation was not used for "memmonitor.hpp" module to work properly,
// otherwise it would report the allocated memory all the time as Logger is a
// global object, this hinders the module to detect memory leaks at the end
// of program and print a useful summery.
// ---------------------
// Note:
// The Logger is used for reporting errors, as a result, if it throws exception, things
// can get ugly, causing errors when handling an error is source of evile ;p
// So methods used for logging are excpetion free, it doesn`t mean they can not have errors
// but even if they have we just report them to std::cerr, and do not handle the error
// Their potential errors:
// 1. Failing to open the file, wrong file path, insuficient permission, etc.
// 2. Failing to write to the file, insuficient permission, filesystem errors, etc.
// 3. Failing to write on std::cerr, jsut let it go ...

#pragma once

#include "configs.hpp"
#include "platform.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <future>

namespace nne {
  class Logger
  {
  public:
    enum LogLevel
    {
      LevelError = 0,
      LevelWarning,
      LevelInfo,
      LevelDebug,
    };

  public:

    // Removing copy and move constructors, to ensure that, the class will remain singleton
    Logger(const Logger&) = delete;
    Logger(Logger&) = delete;
    Logger(const Logger&&) = delete;
    Logger(Logger&&) = delete;

    // If needed create and return the Logger object
    static Logger& GetInstance() noexcept;

    // Should be called once, for configuration, before using the object
    static void Init(LogLevel p_level, bool p_log_to_console, bool p_log_to_file) noexcept;

    static void SetLevel(LogLevel p_level) noexcept;

    // Can be used to enable or disable logging methods
    static void EnableLogging(bool p_log_to_console, bool p_log_to_file) noexcept;

    // Disable all logging methods
    static void DisableLogging() noexcept;

    static void Error(const char* p_message) noexcept;
    static void Error(const std::string& p_message) noexcept;

    static void Warn(const char* p_message) noexcept;
    static void Warn(const std::string& p_message) noexcept;

    static void Info(const char* p_message) noexcept;
    static void Info(const std::string& p_message) noexcept;

    static void Debug(const char* p_message) noexcept;
    static void Debug(const std::string& p_message) noexcept;

  private:
    // Streams don't throw exceptions by default, so construtor can be noexcept

#ifdef NNE_WIN_MINGW
    Logger() = default;
#else
    Logger() noexcept = default;
#endif
    ~Logger() noexcept;

    // Internal implementations of logging functions, which are used by
    // public static functions
    void IError(const char* p_message) noexcept;
    void IWarn(const char* p_message) noexcept;
    void IInfo(const char* p_message) noexcept;
    void IDebug(const char* p_message) noexcept;

    // Decorates the message with proper title
    // Note: All used functions are noexcept, only memory access violation can
    // be the major error, in that case we are already doomed :(
    void IAddTitle(const char* p_title,const char* p_message) noexcept;

    // Streams don't throw exceptions by default
    void ILogToConsole(const char* p_message) noexcept;

    void ILogToFile(const char* p_message);
    void ILog() noexcept;

    void ReportOFStreamError(const char* p_message, bool p_include_filepath = false) noexcept;

    void OpenLogFile() noexcept;
#ifdef NDEBUG
    void CloseLogFile() noexcept;
#else
  public:
    void CloseLogFile() noexcept;
#endif

  private:
    // Streams don't throw exceptions by default
    std::ofstream m_log_file;

    const char* m_log_file_name = "./log.txt";

    const char* m_str_warning = "[WRN]: ";
    const char* m_str_error = "[ERR]: ";
    const char* m_str_info = "[INF]: ";
    const char* m_str_debug = "[DBG]: ";
    const char* m_str_enter = "\n";
    const char* m_str_null = "\0";
    const char* m_str_left_decorator = "|";
    const char* m_str_right_decorator = "| ";

    LogLevel m_level = LevelError;
    bool m_log_to_console = true;
    bool m_log_to_file = false;

    // variable to store the formated message
    char m_buffer[NNE_LOGGER_BUFFER_GLOBAL];

    // The mutex used for thread safety when writing to log file
    // Note: std::mutex constructor is a noexcept method
    std::mutex m_mutex;
  };
}
