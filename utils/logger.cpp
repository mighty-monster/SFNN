// File Name:     logger.cpp
// Author:        Arash Fatehi
// Date:          12th Feb 2021
// Description:   Facilitates logging to console and files

#include "logger.hpp"

#include "general.hpp"

#include <chrono>
#include <ctime>
#include <cstring>
#include <cerrno>

using namespace nne;

// If needed create and return the Logger object
Logger& Logger::GetInstance() noexcept
{
  // Local static variables are global but only accesable within function`s scope
  static Logger instance;
  return instance;
};

// Should be called once, for configuration, before using the object
void Logger::Init(LogLevel p_level, bool p_log_to_console, bool p_log_to_file) noexcept
{
  SetLevel(p_level);
  EnableLogging(p_log_to_console, p_log_to_file);
};

void Logger::SetLevel(LogLevel p_level) noexcept
{
  GetInstance().m_level = p_level;
};

// Can be used to enable or disable logging methods
void Logger::EnableLogging(bool p_log_to_console, bool p_log_to_file) noexcept
{
  GetInstance().m_log_to_console = p_log_to_console;
  GetInstance().m_log_to_file = p_log_to_file;

  // By defualt p_log_to_file is false, in case class is used without calling "Init"
  // File should not be opened untill user asks specificaly to use loggin to file
  // either by calling "EnableLogging" or "Init" methods.
  if(p_log_to_file)
    GetInstance().OpenLogFile();
  else
    GetInstance().CloseLogFile();
};

// Disable all logging methods
void Logger::DisableLogging() noexcept
{
  EnableLogging(false, false);
};

void Logger::Error(const char* p_message) noexcept
{
  GetInstance().IError(p_message);
};

void Logger::Error(const std::string& p_message) noexcept
{
  GetInstance().IError(p_message.c_str());
};

void Logger::Warn(const char* p_message) noexcept
{
  GetInstance().IWarn(p_message);
};

void Logger::Warn(const std::string& p_message) noexcept
{
  GetInstance().IWarn(p_message.c_str());
};

void Logger::Info(const char* p_message) noexcept
{
  GetInstance().IInfo(p_message);
};

void Logger::Info(const std::string& p_message) noexcept
{
  GetInstance().IInfo(p_message.c_str());
};

void Logger::Debug(const char* p_message) noexcept
{
  GetInstance().IDebug(p_message);
};

void Logger::Debug(const std::string& p_message) noexcept
{
  GetInstance().IDebug(p_message.c_str());
};

// Logger destructor
Logger::~Logger() noexcept
{
  CloseLogFile();
};

void Logger::IError(const char* p_message) noexcept
{
  if (m_level >= LevelError)
  {
    IAddTitle(m_str_error, p_message);
    ILog();
  }
};

void Logger::IWarn(const char* p_message) noexcept
{
  if (m_level >= LevelWarning)
  {
    IAddTitle(m_str_warning, p_message);
    ILog();
  }
};

void Logger::IInfo(const char* p_message) noexcept
{
  if (m_level >= LevelInfo)
  {
    IAddTitle(m_str_info, p_message);
    ILog();
  }
};

void Logger::IDebug(const char* p_message) noexcept
{
  if (m_level >= LevelDebug)
  {    
    IAddTitle(m_str_debug, p_message);
    ILog();
  }
};

void Logger::IAddTitle(const char* p_title,const char* p_message) noexcept
{
  size_t offset = 0;

  // One byte reduced to overwite the null termination character
  // Note: NNE_LOGGER_DATETIME_BUFFER_SIZE is the size with null charachter
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

  // Adding null chachter to the end, "strlen" wont work here
  // because it will not count null charachter
  offset += strlen(m_str_enter);
  nne::strcpy_nne(m_buffer, NNE_LOGGER_BUFFER_GLOBAL,
                  m_str_null, 1, offset);
}

void Logger::ILogToConsole(const char* p_message) noexcept
{
  // Streams don't throw exceptions by default
  std::cout << p_message;
};

void Logger::ILogToFile(const char* p_message)
{
  if (m_log_file.is_open())
  {
    // lock constructor might throw exception
    std::lock_guard<std::mutex> lock(m_mutex);

    GetInstance().m_log_file << p_message;
    if(m_log_file.fail())
       ReportOFStreamError("Failed to write to log file: ");
  }
  else
  {
    NNE_ERORR_LL("Log file is not open, can`t wite to it.");
  }
};

// Note: Method "ILog" can throw only one kind of exceptions
// "std::lock_guard" can throw exception in "ILogToFile" method
// By catching and reporting these exceptions, "ILog" will be noexcept
// The reseaon for this is, Logger is used in reporting error of application
// If it throws exception itself, it will be a mess, if somthing happens, we just report
// to "std::cerr" and wont handle it`s errors by changing the flow of execution, as
// there is nothing to recover from at this point
void Logger::ILog() noexcept
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
  {
    try
    {
      ILogToFile(m_buffer);
    }
    catch (std::exception& ex)
    {
      NNE_ERORR_LL(ex.what());
    }
    catch (...)
    {
      NNE_ERORR_LL("Unkown exception thrown in ILogToFile(char*)");
    }
  }

};

void Logger::OpenLogFile() noexcept
{
  GetInstance().m_log_file.open(GetInstance().m_log_file_name,
                                std::ofstream::out | std::ofstream::app);
  if (!m_log_file.fail())
    GetInstance().m_log_file << "\n--------------------\n";
  else
    ReportOFStreamError("Failed to open log file", true);
}

void Logger::CloseLogFile() noexcept
{
  if (m_log_file.is_open())
  {
    m_log_file.close();

    if (m_log_file.fail())
      ReportOFStreamError("Something went wrong when closing log file, "
                          "it is possible that stream buffer couldn`t get flashed to file");
  }
}

// date_time_str size should atleast have 20 bytes
void Logger::GetCurrentTime(char* p_date_time_str) noexcept
{

  // Receive a time point using chrono API
  auto time_point_now = std::chrono::system_clock::now();

  // Convert time_point to time_t
  std::time_t time_t_now = std::chrono::system_clock::to_time_t(time_point_now);

#if defined(NNE_WIN_MSVC)
  // MSVC prefers localtime_s

  // Convert time_t to tm struct
  // localtime_s returns 0 if succesfull, that`s why the if`s logic is twisted ;)
  struct tm tm_struct;
  if (localtime_s(&tm_struct, &time_t_now))
  {
    NNELLRORR("Failed to convert time_t to tm struct");
    strcpy_nne(p_date_time_str, NNE_LOGGER_DATETIME_BUFFER_SIZE, m_str_unknown, strlen(m_str_unknown) + 1);
    return;
  }

  // Format tm struct into the buffer
  if (!std::strftime(p_date_time_str,
                     NNE_LOGGER_DATETIME_BUFFER_SIZE,
                     "%Y-%m-%d %H:%M:%S",
                     &tm_struct))

#else
  // GCC doesn`t suport localtime_s, others might not as well

  // Convert time_t to tm struct
  struct tm* tm_struct = localtime(&time_t_now);
  if (!tm_struct)
  {
    NNE_ERORR_LL("Failed to convert time_t to tm struct");
    strcpy_nne(p_date_time_str, NNE_LOGGER_DATETIME_BUFFER_SIZE, m_str_unknown, strlen(m_str_unknown) + 1);
    return;
  }

  // Format tm struct into the buffer
  if (!std::strftime(p_date_time_str,
                     NNE_LOGGER_DATETIME_BUFFER_SIZE,
                     "%Y-%m-%d %H:%M:%S",
                     tm_struct))
#endif
  {
    NNE_ERORR_LL("Failed to format tm struct");
    strcpy_nne(p_date_time_str, NNE_LOGGER_DATETIME_BUFFER_SIZE, m_str_unknown, strlen(m_str_unknown) + 1);
  }
};

void Logger::ReportOFStreamError(const char* p_message, bool p_include_filepath) noexcept
{
  // getting errno can thorw exception, again, if that happens, let us be doomed ...

  const char* separator = ", ";

  size_t reason_length = 256;
  char reason[reason_length];

  size_t final_message_length = strlen(p_message) + strlen(separator) + reason_length + 1;
  final_message_length += p_include_filepath ? strlen(m_log_file_name) + strlen(reason) : 0;

  char* final_message = (char*)alloca(final_message_length);

  // This error is too low level to handle, if we can not allocate few bytes on stack, chances
  // are high that we can not proceed anymore, no point in handling the error, just reporting
  if (!final_message)
  {
    NNE_ERORR_LL("Could not allocate memory on stack for the message, so can not proceed");
    return;
  }

  strcpy_nne(final_message, final_message_length, p_message, strlen(p_message) + 1);

  if (p_include_filepath)
  {
    strcat_nne(final_message, final_message_length, separator, strlen(separator) + 1);
    strcat_nne(final_message, final_message_length, m_log_file_name, strlen(m_log_file_name) + 1);
  }

#ifdef _WIN32
  if ( strerror_s(reason, reason_length, errno) == 0 )
  {
#elif __USE_XOPEN2K
  if ( strerror_r( errno, reason, reason_length) == 0 )
  {
#else
  if(strerror_r( errno, reason, reason_length))
  {
#endif
    strcat_nne(final_message, final_message_length, separator, strlen(separator) + 1);
    strcat_nne(final_message, final_message_length, reason, strlen(reason) + 1);
  }

  NNE_ERORR_LL(final_message);
}
