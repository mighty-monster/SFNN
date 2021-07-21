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

// ---------------------
// Note:
// Use "GetInstance()" to recieve a singleton instance of the object and static methods
// "Error", "Warn", "Info", and "Debug" for logging
// ---------------------

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
// ---------------------

#ifndef UTILS_LOGGER_HPP
#define UTILS_LOGGER_HPP

#include "configs.hpp"
#include "platform.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <future>

namespace mnt {
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
    static void Init(LogLevel _level, bool _log_to_console, bool _log_to_file) noexcept;

    static void SetLevel(LogLevel _level) noexcept;

    static void EnableLogging(bool _log_to_console, bool _log_to_file) noexcept;
    static void DisableLogging() noexcept;

    static void Error(const char* _message) noexcept;
    static void Error(const std::string& _message) noexcept;

    static void Warn(const char* _message) noexcept;
    static void Warn(const std::string& _message) noexcept;

    static void Info(const char* _message) noexcept;
    static void Info(const std::string& _message) noexcept;

    static void Debug(const char* _message) noexcept;
    static void Debug(const std::string& _message) noexcept;

  private:
    Logger() = default;
    ~Logger() noexcept;

    // Internal implementations of logging functions, which are used by
    // public static functions
    void IError(const char* _message) noexcept;
    void IWarn(const char* _message) noexcept;
    void IInfo(const char* _message) noexcept;
    void IDebug(const char* _message) noexcept;

    // Decorates the message with proper title
    // Note: All used functions are noexcept, only memory access violation can
    // be the major error, in that case we are already doomed :(
    void IAddTitle(const char* _title,const char* _message) noexcept;

    // Streams don't throw exceptions by default
    void ILogToConsole(const char* _message) noexcept;

    void ILogToFile(const char* _message);
    void ILog() noexcept;

    void ReportOFStreamError(const char* _message, bool _include_filepath = false) noexcept;

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

    LogLevel m_level = LevelInfo;
    bool m_log_to_console = true;
    bool m_log_to_file = false;

    // variable to store the formated message
    char m_buffer[MNT_LOGGER_BUFFER_GLOBAL];

    // The mutex used for thread safety when writing to log file
    // Note: std::mutex constructor is a noexcept method
    std::mutex m_mutex;
  };
}

#endif
