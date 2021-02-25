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
// Use GetInstance() to recieve a singleton instance of the object and static methods "Error", "Warn",
// "Info", and "Debug" for logging

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <future>

#define DATETIME_BUFFER_SIZE 20

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

    // Removing copy constructor, to ensure the class will remain singleton
    Logger(const Logger&) = delete;

    // If needed create and return the Logger object
    static Logger& GetInstance();

    // Should be called once, for configuration, before using the object
    static void Init(LogLevel p_level, bool p_log_to_console, bool p_log_to_file);

    static void SetLevel(LogLevel p_level);

    // Can be used to enable or disable logging methods
    static void EnableLogging(bool p_log_to_console, bool p_log_to_file);

    // Disable all logging methods
    static void DisableLogging();

    static void Error(const char* p_message);
    static void Error(const std::string& p_message);

    static void Warn(const char* p_message);
    static void Warn(const std::string& p_message);

    static void Info(const char* p_message);
    static void Info(const std::string& p_message);

    static void Debug(const char* p_message);
    static void Debug(const std::string& p_message);

  private:
    Logger() = default;
    ~Logger();

    // Internal implementations of logging functions, which are used by
    // public static functions
    void IError(const char* p_message);
    void IWarn(const char* p_message);
    void IInfo(const char* p_message);
    void IDebug(const char* p_message);

    void ILogToConsole(const char* p_message);
    void ILogToFile(const char* p_message);
    void ILog(const char* p_message);

    void OpenLogFile();
    void CloseLogFile();

    void GetCurrentTime(char* p_date_time_str);

  private:
    std::ofstream m_log_file;
    const char* m_log_file_name = "log.txt";

    const char* m_str_warning = "[WRN]: ";
    const char* m_str_error = "[ERR]: ";
    const char* m_str_info = "[INF]: ";
    const char* m_str_debug = "[DBG]: ";
    const char* m_str_enter = "\n";
    const char* m_str_left_decorator = "|";
    const char* m_str_right_decorator = "| ";
    const char* m_str_unknown = "Unknown";

    LogLevel m_level = LevelError;
    bool m_log_to_console = true;
    bool m_log_to_file = false;

    // The mutex used for thread safety when writing to log file
    std::mutex m_mutex;

  };
}
