// File Name:     logger.cpp
// Author:        Arash Fatehi
// Date:          12th Feb 2021
// Description:   Facilitates logging to console and files

#include "logger.hpp"

#include "general.hpp"

#include <chrono>
#include <ctime>
#include <cstring>

using namespace nne;

// If needed create and return the Logger object
Logger& Logger::GetInstance()
{
  // Local static variables are global but only accesable within function`s scope
  static Logger instance;
  return instance;
};

// Should be called once, for configuration, before using the object
void Logger::Init(LogLevel p_level, bool p_log_to_console, bool p_log_to_file)
{
  SetLevel(p_level);
  EnableLogging(p_log_to_console, p_log_to_file);
};

void Logger::SetLevel(LogLevel p_level)
{
  GetInstance().m_level = p_level;
};

// Can be used to enable or disable logging methods
void Logger::EnableLogging(bool p_log_to_console, bool p_log_to_file)
{
  GetInstance().m_log_to_console = p_log_to_console;
  GetInstance().m_log_to_file = p_log_to_file;

  if(p_log_to_file)
    GetInstance().OpenLogFile();
  else
    GetInstance().CloseLogFile();
};

// Disable all logging methods
void Logger::DisableLogging()
{
  EnableLogging(false, false);
};

void Logger::Error(const char* p_message)
{
  GetInstance().IError(p_message);
};

void Logger::Error(const std::string& p_message)
{
  GetInstance().IError(p_message.c_str());
};

void Logger::Warn(const char* p_message)
{
  GetInstance().IWarn(p_message);
};

void Logger::Warn(const std::string& p_message)
{
  GetInstance().IWarn(p_message.c_str());
};

void Logger::Info(const char* p_message)
{
  GetInstance().IInfo(p_message);
};

void Logger::Info(const std::string& p_message)
{
  GetInstance().IInfo(p_message.c_str());
};

void Logger::Debug(const char* p_message)
{
  GetInstance().IDebug(p_message);
};

void Logger::Debug(const std::string& p_message)
{
  GetInstance().IDebug(p_message.c_str());
};

// Logger destructor
Logger::~Logger()
{
  CloseLogFile();
};

void Logger::IError(const char* p_message)
{
  if (m_level >= LevelError)
  {
    IAddTitle(m_str_error, p_message);
    ILog();
  }
};

void Logger::IWarn(const char* p_message)
{
  if (m_level >= LevelWarning)
  {
    IAddTitle(m_str_warning, p_message);
    ILog();
  }
};

void Logger::IInfo(const char* p_message)
{
  if (m_level >= LevelInfo)
  {
    IAddTitle(m_str_info, p_message);
    ILog();
  }
};

void Logger::IDebug(const char* p_message)
{
  if (m_level >= LevelDebug)
  {    
    IAddTitle(m_str_debug, p_message);
    ILog();
  }
};

void Logger::IAddTitle(const char* p_title,const char* p_message)
{
  size_t offset = 0;

  // One byte reduced to overwite the null termination character
  // Hint: NNE_LOGGER_DATETIME_BUFFER_SIZE is the size with null charachter
  offset += strlen(m_str_left_decorator) +
      NNE_LOGGER_DATETIME_BUFFER_SIZE + strlen(m_str_right_decorator) - 1;

  // Add content in place to m_buffer by calculating the proper offset
  // and copying the content in the right place

  nne::strcpy_nne(m_buffer, NNE_LOGGER_BUFFER_GLOBAL,
                  p_title, strlen(p_title), offset);

  offset += strlen(p_title);
  nne::strcpy_nne(m_buffer, NNE_LOGGER_BUFFER_GLOBAL,
                  p_message,   strlen(p_message),   offset);

  offset += strlen(p_message);
  nne::strcpy_nne(m_buffer, NNE_LOGGER_BUFFER_GLOBAL,
                  m_str_enter, strlen(m_str_enter), offset);

  // Adding null chachter to the end
  offset += strlen(m_str_enter);
  nne::strcpy_nne(m_buffer, NNE_LOGGER_BUFFER_GLOBAL,
                  m_str_null, strlen(m_str_null), offset);
}

void Logger::ILogToConsole(const char* p_message)
{
  std::cout<<p_message;
};

void Logger::ILogToFile(const char* p_message)
{
  if (m_log_file.is_open())
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    GetInstance().m_log_file << p_message;
  }
};

void Logger::ILog()
{
  char current_time[NNE_LOGGER_DATETIME_BUFFER_SIZE];
  GetCurrentTime(current_time);

  // Add content in place to m_buffer by calculating the proper offset
  // and copying the content in the right place
  size_t offset = 0;
  nne::strcpy_nne(m_buffer, NNE_LOGGER_BUFFER_GLOBAL,
                  m_str_left_decorator, strlen(m_str_left_decorator), offset);

  offset += strlen(m_str_left_decorator);
  nne::strcpy_nne(m_buffer, NNE_LOGGER_BUFFER_GLOBAL,
                  current_time, strlen(current_time), offset);

  offset += strlen(current_time);
  nne::strcpy_nne(m_buffer, NNE_LOGGER_BUFFER_GLOBAL,
                  m_str_right_decorator, strlen(m_str_right_decorator), offset);

  if(m_log_to_console)
    ILogToConsole(m_buffer);

  if(m_log_to_file)
    ILogToFile(m_buffer);
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

// date_time_str size should atleast have 20 bytes
void Logger::GetCurrentTime(char* p_date_time_str)
{

  // Receive a time point using chrono API
  auto time_point_now = std::chrono::system_clock::now();

  // Convert time_point to time_t
  std::time_t time_t_now = std::chrono::system_clock::to_time_t(time_point_now);

// MSVC prefers localtime_s
#if defined(NNE_WIN_MSVC)
  // Convert time_t to tm struct
  // localtime_s returns 0 if succesfull, that`s why the if`s logic is twisted ;)
  struct tm tm_struct;
  if (localtime_s(&tm_struct, &time_t_now))
  {
    std::cerr<<"Logger::GetCurrentTime() -- Failed to convert time_t to tm struct\n";
    strncpy_s(p_date_time_str, NNE_LOGGER_DATETIME_BUFFER_SIZE, m_str_unknown, strlen(m_str_unknown) + 1);
  }

  // Format tm struct into the buffer
  if (!std::strftime(p_date_time_str,
                     NNE_LOGGER_DATETIME_BUFFER_SIZE,
                     "%Y-%m-%d %H:%M:%S",
                     &tm_struct))
  {
    std::cerr<<"Logger::GetCurrentTime() -- Failed to format tm struct\n";
    strncpy_s(p_date_time_str, NNE_LOGGER_DATETIME_BUFFER_SIZE, m_str_unknown, strlen(m_str_unknown) + 1);
  }
// GCC doesn`t currently suport localtime_s, others might not as well
#else
  // Convert time_t to tm struct
  struct tm* tm_struct = localtime(&time_t_now);
  if (!tm_struct)
  {
    std::cerr<<"Logger::GetCurrentTime() -- Failed to convert time_t to tm struct\n";
    strncpy(p_date_time_str, m_str_unknown, strlen(m_str_unknown) + 1);
  }

  // Format tm struct into the buffer
  if (!std::strftime(p_date_time_str,
                     NNE_LOGGER_DATETIME_BUFFER_SIZE,
                     "%Y-%m-%d %H:%M:%S",
                     tm_struct))
  {
    std::cerr<<"Logger::GetCurrentTime() -- Failed to format tm struct\n";
    strncpy(p_date_time_str, m_str_unknown, strlen(m_str_unknown) + 1);
  }
#endif

};
