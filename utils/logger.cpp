// File Name:     logger.cpp
// Author:        Arash Fatehi
// Date:          12th Feb 2021
// Description:   Facilitates logging to console and files

#include "logger.hpp"

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

#ifdef _MSC_VER
    char temp_str[WIN32_MESSAGE_MAX_LENGTH];
    std::fill(temp_str, temp_str + strlen(temp_str) ,0);

    strncpy_s(temp_str, m_str_error, strlen(m_str_error));
    strncpy_s(temp_str, p_message, strlen(p_message));
    strncpy_s(temp_str, m_str_enter, strlen(m_str_enter));
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
    char temp_str[strlen(m_str_error) + strlen(p_message) + strlen(m_str_enter)];
    std::fill(temp_str, temp_str + strlen(temp_str) ,0);

    strncpy(temp_str, m_str_error, strlen(m_str_error));
    strncat(temp_str, p_message, strlen(p_message));
    strncat(temp_str, m_str_enter, strlen(m_str_enter));
#endif

    ILog(temp_str);
  }
};

void Logger::IWarn(const char* p_message)
{
  if (m_level >= LevelWarning)
  {
#ifdef _MSC_VER
    char temp_str[WIN32_MESSAGE_MAX_LENGTH];
    std::fill(temp_str, temp_str + strlen(temp_str) ,0);

    strncpy_s(temp_str, m_str_warning, strlen(m_str_warning));
    strncat_s(temp_str, p_message, strlen(p_message));
    strncat_s(temp_str, m_str_enter, strlen(m_str_enter));
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
    char temp_str[strlen(m_str_warning) + strlen(p_message) + strlen(m_str_enter)];
    std::fill(temp_str, temp_str + strlen(temp_str) ,0);

    strncpy(temp_str, m_str_warning, strlen(m_str_warning));
    strncat(temp_str, p_message, strlen(p_message));
    strncat(temp_str, m_str_enter, strlen(m_str_enter));
#endif
    ILog(temp_str);
  }
};

void Logger::IInfo(const char* p_message)
{
  if (m_level >= LevelInfo)
  {
#ifdef _MSC_VER
    char temp_str[WIN32_MESSAGE_MAX_LENGTH];
    std::fill(temp_str, temp_str + strlen(temp_str) ,0);

    strncpy_s(temp_str, m_str_info, strlen(m_str_info));
    strncat_s(temp_str, p_message, strlen(p_message));
    strncat_s(temp_str, m_str_enter, strlen(m_str_enter));
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
    char temp_str[strlen(m_str_info) + strlen(p_message) + strlen(m_str_enter)];
    std::fill(temp_str, temp_str + strlen(temp_str) ,0);

    strncpy(temp_str, m_str_info, strlen(m_str_info));
    strncat(temp_str, p_message, strlen(p_message));
    strncat(temp_str, m_str_enter, strlen(m_str_enter));
#endif
    ILog(temp_str);
  }
};

void Logger::IDebug(const char* p_message)
{
  if (m_level >= LevelDebug)
  {
#ifdef _MSC_VER
    char temp_str[WIN32_MESSAGE_MAX_LENGTH];
    std::fill(temp_str, temp_str + strlen(temp_str) ,0);

    strncpy_s(temp_str, m_str_debug, strlen(m_str_debug));
    strncat_s(temp_str, p_message, strlen(p_message));
    strncat_s(temp_str, m_str_enter, strlen(m_str_enter));
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
    char temp_str[strlen(m_str_debug) + strlen(p_message) + strlen(m_str_enter)];
    std::fill(temp_str, temp_str + strlen(temp_str) ,0);

    strncpy(temp_str, m_str_debug, strlen(m_str_debug));
    strncat(temp_str, p_message, strlen(p_message));
    strncat(temp_str, m_str_enter, strlen(m_str_enter));
#endif
    ILog(temp_str);
  }
};

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

void Logger::ILog(const char* p_message)
{

  char current_time[DATETIME_BUFFER_SIZE];
  GetCurrentTime(current_time);

#ifdef _MSC_VER
  char temp_str[WIN32_MESSAGE_MAX_LENGTH];

  std::fill(temp_str, temp_str + strlen(temp_str) ,0);

  strncpy_s(temp_str, m_str_left_decorator, strlen(m_str_left_decorator));
  strncat_s(temp_str, current_time, strlen(current_time));
  strncat_s(temp_str, m_str_right_decorator, strlen(m_str_right_decorator));
  strncat_s(temp_str, p_message, strlen(p_message));
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
  char temp_str[strlen(m_str_left_decorator) + strlen(current_time) +
      strlen(m_str_right_decorator) + strlen(p_message)];

  std::fill(temp_str, temp_str + strlen(temp_str) ,0);

  strncpy(temp_str, m_str_left_decorator, strlen(m_str_left_decorator));
  strncat(temp_str, current_time, strlen(current_time));
  strncat(temp_str, m_str_right_decorator, strlen(m_str_right_decorator));
  strncat(temp_str, p_message, strlen(p_message));
#endif
  if(m_log_to_console)
    ILogToConsole(temp_str);

  if(m_log_to_file)
    ILogToFile(temp_str);
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

  // Buffer to store formated datetime

// MinGW and Clang support strncpy_s on windows, so checking for
#ifdef WIN32
  // Convert time_t to tm struct
  // localtime_s returns 0 if succesfull, that`s why the if`s logic is twisted ;)
  struct tm tm_struct;
  if (localtime_s(&tm_struct, &time_t_now))
  {
    std::cerr<<"Logger::GetCurrentTime() -- Failed to convert time_t to tm struct\n";
    strncpy_s(p_date_time_str, DATETIME_BUFFER_SIZE, m_str_unknown, strlen(m_str_unknown));
  }

  // Format tm struct into the buffer
  if (!std::strftime(p_date_time_str,
                     DATETIME_BUFFER_SIZE,
                     "%Y-%m-%d %H:%M:%S",
                     &tm_struct))
  {
    std::cerr<<"Logger::GetCurrentTime() -- Failed to format tm struct\n";
    strncpy_s(p_date_time_str, DATETIME_BUFFER_SIZE, m_str_unknown, strlen(m_str_unknown));
  }
#elif __unix__
  // Convert time_t to tm struct
  struct tm* tm_struct = localtime(&time_t_now);
  if (!tm_struct)
  {
    std::cerr<<"Logger::GetCurrentTime() -- Failed to convert time_t to tm struct\n";
    strncpy(p_date_time_str, m_str_unknown, strlen(m_str_unknown));
  }

  // Format tm struct into the buffer
  if (!std::strftime(p_date_time_str,
                     DATETIME_BUFFER_SIZE,
                     "%Y-%m-%d %H:%M:%S",
                     tm_struct))
  {
    std::cerr<<"Logger::GetCurrentTime() -- Failed to format tm struct\n";
    strncpy(p_date_time_str, m_str_unknown, strlen(m_str_unknown));
  }
#endif

};
