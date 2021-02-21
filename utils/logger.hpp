///////////////////////////////////////////////////////////////////////////////
// @File Name:     logger.hpp                                                //
// @Author:        Arash Fatehi                                              //
// @L.M.D:         12th Feb 2021                                             //
// @Description:   Logging to console and files                              //
//                                                                           //
// Detail Description:                                                       //
// Thread safe logging mechanism for console output and files compatible     //
// with windows and linux platforms.                                         //
//                                                                           //
// Use GetInstance() to recieve a singleton instance of the object           //
//                                                                           //
// Supported Log Type: Error, Warning, Info, Debug                           //
//                                                                           //
// Type std::string used for convinent, for each logging call, 4 string      //
// related allocation happens in release mode and 12 happens in debug mode   //
// on MVS2019 compiler and 5 for both debug and release on GCC 10.2.0        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <future>

namespace nnc {
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
    static void Init(LogLevel level, bool log_to_console, bool log_to_file);

    static void SetLevel(LogLevel level);

    // Can be used to enable or disable logging methods
    static void EnableLogging(bool log_to_console, bool log_to_file);

    // Disable all logging methods
    static void DisableLogging();

    static void Error(const char* message);
    static void Error(const std::string& message);

    static void Warn(const char* message);
    static void Warn(const std::string& message);

    static void Info(const char* message);
    static void Info(const std::string& message);

    static void Debug(const char* message);
    static void Debug(const std::string& message);

  private:
    Logger() = default;
    ~Logger();

    // Internal implementations of logging functions, which are used by
    // public static functions
    void IError(const std::string& message);
    void IWarn(const std::string& message);
    void IInfo(const std::string& message);
    void IDebug(const std::string& message);

    void ILogToConsole(const std::string& message);
    void ILogToFile(const std::string& message);
    void ILog(const std::string& message);

    void OpenLogFile();
    void CloseLogFile();

    std::string GetCurrentTime();

  private:
    std::ofstream m_log_file;
    const char* m_log_file_name = "log.txt";

    LogLevel m_level = LevelError;
    bool m_log_to_console = true;
    bool m_log_to_file = false;

    // The mutex used for thread safety when writing to log file
    std::mutex m_mutex;

  };
}
