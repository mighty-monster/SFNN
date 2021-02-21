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

#include <chrono>
#include <ctime>

#include "logger.hpp"

using namespace nnc;

// If needed create and return the Logger object
Logger& Logger::GetInstance()
{
  // Local static variables are global but only accesable within function`s scope
  static Logger instance;
  return instance;
};

// Should be called once, for configuration, before using the object
void Logger::Init(LogLevel level, bool log_to_console, bool log_to_file)
{
  SetLevel(level);
  EnableLogging(log_to_console, log_to_file);
};

void Logger::SetLevel(LogLevel level)
{
  GetInstance().m_level = level;
};

// Can be used to enable or disable logging methods
void Logger::EnableLogging(bool log_to_console, bool log_to_file)
{
  GetInstance().m_log_to_console = log_to_console;
  GetInstance().m_log_to_file = log_to_file;

  if(log_to_file)
    GetInstance().OpenLogFile();
  else
    GetInstance().CloseLogFile();
};

// Disable all logging methods
void Logger::DisableLogging()
{
  EnableLogging(false, false);
};

void Logger::Error(const char* message)
{
  if (GetInstance().m_log_to_console || GetInstance().m_log_to_file)
    Error(std::string(message));
};

void Logger::Error(const std::string& message)
{
  GetInstance().IError(message);
};

void Logger::Warn(const char* message)
{
  if (GetInstance().m_log_to_console || GetInstance().m_log_to_file)
    Warn(std::string(message));
};

void Logger::Warn(const std::string& message)
{
  GetInstance().IWarn(message);
};

void Logger::Info(const char* message)
{
  if (GetInstance().m_log_to_console || GetInstance().m_log_to_file)
    Info(std::string(message));
};

void Logger::Info(const std::string& message)
{
  GetInstance().IInfo(message);
};

void Logger::Debug(const char* message)
{
  if (GetInstance().m_log_to_console || GetInstance().m_log_to_file)
    Debug(std::string(message));
};

void Logger::Debug(const std::string& message)
{
  GetInstance().IDebug(message);
};

// Logger destructor
Logger::~Logger()
{
  CloseLogFile();
};

void Logger::IError(const std::string& message)
{
  if (m_level >= LevelError)
    ILog(std::string("[ERR]: ") + message + "\n");
};

void Logger::IWarn(const std::string& message)
{
  if (m_level >= LevelWarning)
    ILog(std::string("[WRN]: ") + message + "\n");
};

void Logger::IInfo(const std::string& message)
{
  if (m_level >= LevelInfo)
    ILog(std::string("[INF]: ") + message + "\n");
};

void Logger::IDebug(const std::string& message)
{
  if (m_level >= LevelDebug)
    ILog(std::string("[DBG]: ") + message + "\n");
};

void Logger::ILogToConsole(const std::string& message)
{
  std::cout<<message;
};

void Logger::ILogToFile(const std::string& message)
{
  if (m_log_file.is_open())
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    GetInstance().m_log_file << message;
  }
};

void Logger::ILog(const std::string& message)
{
  std::string formated_message =
       "|" + GetCurrentTime() + "| " + message;

  if(m_log_to_console)
    ILogToConsole(formated_message);

  if(m_log_to_file)
    ILogToFile(formated_message);
};

void Logger::OpenLogFile()
{
  GetInstance().m_log_file.open(GetInstance().m_log_file_name,
                                std::ofstream::out | std::ofstream::app);
  if (m_log_file.is_open())
    GetInstance().m_log_file << "\n--------------------\n";
  else
    std::cerr << "Logger::OpenLogFile() -- Failed to open log file";
}

void Logger::CloseLogFile()
{
  if (m_log_file.is_open())
    m_log_file.close();
}

std::string Logger::GetCurrentTime()
{
  // Receive a time point using chrono API
  auto time_point_now = std::chrono::system_clock::now();

  // Convert time_point to time_t
  std::time_t time_t_now = std::chrono::system_clock::to_time_t(time_point_now);

  // Buffer to store formated datetime
  char date_time_str[20];

#ifdef WIN32
  // Convert time_t to tm struct
  // localtime_s returns 0 if succesfull, that`s why the if`s logic is twisted ;)
  struct tm tm_struct;
  if (localtime_s(&tm_struct, &time_t_now))
  {
    std::cerr<<"Logger::GetCurrentTime() -- Failed to convert time_t to tm struct\n";
    return std::string("Unknown");
  }

  // Format tm struct into the buffer
  if (!std::strftime(date_time_str,
                     sizeof(date_time_str),
                     "%Y-%m-%d %H:%M:%S",
                     &tm_struct))
#elif __unix__
  // Convert time_t to tm struct
  struct tm* tm_struct = localtime(&time_t_now);
  if (!tm_struct)
  {
    std::cerr<<"Logger::GetCurrentTime() -- Failed to convert time_t to tm struct\n";
    return std::string("Unknown");
  }

  // Format tm struct into the buffer
  if (!std::strftime(date_time_str,
                     sizeof(date_time_str),
                     "%Y-%m-%d %H:%M:%S",
                     tm_struct))
#endif
  {
    std::cerr<<"Logger::GetCurrentTime() -- Failed to format tm struct\n";
    return std::string("Unknown");
  }

  return std::string(date_time_str);
};
