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

using namespace mnt;

// If needed create and return the Logger object
Logger& Logger::GetInstance() noexcept
{
  // Local static variables are global but only accesable within function`s scope
  static Logger instance;
  return instance;
};

// Should be called once, for configuration, before using the object
void Logger::Init(LogLevel _level, bool _log_to_console, bool _log_to_file) noexcept
{
  SetLevel(_level);
  EnableLogging(_log_to_console, _log_to_file);
};

void Logger::SetLevel(LogLevel _level) noexcept
{
  GetInstance().m_level = _level;
};

// Can be used to enable or disable logging methods
void Logger::EnableLogging(bool _log_to_console, bool _log_to_file) noexcept
{
  GetInstance().m_log_to_console = _log_to_console;
  GetInstance().m_log_to_file = _log_to_file;

  // By defualt _log_to_file is false, in case class is used without calling "Init"
  // File should not be opened untill user asks specificaly to use loggin to file
  // either by calling "EnableLogging" or "Init" methods.
  if(_log_to_file)
    GetInstance().OpenLogFile();
  else
    GetInstance().CloseLogFile();
};

// Disable all logging methods
void Logger::DisableLogging() noexcept
{
  EnableLogging(false, false);
};

void Logger::Error(const char* _message) noexcept
{
  GetInstance().IError(_message);
};

void Logger::Error(const std::string& _message) noexcept
{
  GetInstance().IError(_message.c_str());
};

void Logger::Warn(const char* _message) noexcept
{
  GetInstance().IWarn(_message);
};

void Logger::Warn(const std::string& _message) noexcept
{
  GetInstance().IWarn(_message.c_str());
};

void Logger::Info(const char* _message) noexcept
{
  GetInstance().IInfo(_message);
};

void Logger::Info(const std::string& _message) noexcept
{
  GetInstance().IInfo(_message.c_str());
};

void Logger::Debug(const char* _message) noexcept
{
  GetInstance().IDebug(_message);
};

void Logger::Debug(const std::string& _message) noexcept
{
  GetInstance().IDebug(_message.c_str());
};

// Logger destructor
Logger::~Logger() noexcept
{
  CloseLogFile();
};

void Logger::IError(const char* _message) noexcept
{
  if (m_level >= LevelError)
  {
    IAddTitle(m_str_error, _message);
    ILog();
  }
};

void Logger::IWarn(const char* _message) noexcept
{
  if (m_level >= LevelWarning)
  {
    IAddTitle(m_str_warning, _message);
    ILog();
  }
};

void Logger::IInfo(const char* _message) noexcept
{
  if (m_level >= LevelInfo)
  {
    IAddTitle(m_str_info, _message);
    ILog();
  }
};

void Logger::IDebug(const char* _message) noexcept
{
  if (m_level >= LevelDebug)
  {    
    IAddTitle(m_str_debug, _message);
    ILog();
  }
};

void Logger::IAddTitle(const char* _title,const char* _message) noexcept
{
  size_t offset = 0;

  // One byte reduced to overwite the null termination character
  // Note: MNT_LOGGER_DATETIME_BUFFER_SIZE is the size with null charachter
  offset += strlen(m_str_left_decorator) +
      MNT_DATETIME_BUFFER_SIZE + strlen(m_str_right_decorator) - 1;

  // Add content in place to m_buffer by calculating the proper offset
  // and copying the content in the right place

  mnt::strcpy_mnt(m_buffer, MNT_LOGGER_BUFFER_GLOBAL,
                  _title, strlen(_title), offset);

  offset += strlen(_title);
  mnt::strcpy_mnt(m_buffer, MNT_LOGGER_BUFFER_GLOBAL,
                  _message,   strlen(_message),   offset);

  offset += strlen(_message);
  mnt::strcpy_mnt(m_buffer, MNT_LOGGER_BUFFER_GLOBAL,
                  m_str_enter, strlen(m_str_enter), offset);

  // Adding null chachter to the end, "strlen" wont work here
  // because it will not count null charachter
  offset += strlen(m_str_enter);
  mnt::strcpy_mnt(m_buffer, MNT_LOGGER_BUFFER_GLOBAL,
                  m_str_null, 1, offset);
}

void Logger::ILogToConsole(const char* _message) noexcept
{
  // Streams don't throw exceptions by default
  std::cout << _message;
};

void Logger::ILogToFile(const char* _message)
{
  if (m_log_file.is_open())
  {
    // lock constructor might throw exception
    std::lock_guard<std::mutex> lock(m_mutex);

    GetInstance().m_log_file << _message;
    if(m_log_file.fail())
       ReportOFStreamError("Failed to write to log file: ");
  }
  else
  {
    MNT_ERORR_LL("Log file is not open, can`t wite to it.");
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
  char current_time[MNT_DATETIME_BUFFER_SIZE];
  GetCurrentTime(current_time);

  // Add content in place to m_buffer by calculating the proper offset
  // and copying the content in the right place
  size_t offset = 0;
  mnt::strcpy_mnt(m_buffer, MNT_LOGGER_BUFFER_GLOBAL,
                  m_str_left_decorator, strlen(m_str_left_decorator), offset);

  offset += strlen(m_str_left_decorator);
  mnt::strcpy_mnt(m_buffer, MNT_LOGGER_BUFFER_GLOBAL,
                  current_time, strlen(current_time), offset);

  offset += strlen(current_time);
  mnt::strcpy_mnt(m_buffer, MNT_LOGGER_BUFFER_GLOBAL,
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
      MNT_ERORR_LL(ex.what());
    }
    catch (...)
    {
      MNT_ERORR_LL("Unkown exception thrown in ILogToFile(char*)");
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

void Logger::ReportOFStreamError(const char* _message, bool _include_filepath) noexcept
{
  // getting errno can thorw exception, again, if that happens, let us be doomed ...

  const char* separator = ", ";

  const size_t reason_length = 256;
  char reason[reason_length];

  size_t final_message_length = strlen(_message) + strlen(separator) + reason_length + 1;
  final_message_length += _include_filepath ? strlen(m_log_file_name) + strlen(separator) : 0;

  char* final_message = (char*)alloca(final_message_length);

  // This error is too low level to handle, if we can not allocate few bytes on stack, chances
  // are high that we can not proceed anymore, no point in handling the error, just reporting
  if (!final_message)
  {
    MNT_ERORR_LL("Could not allocate memory on stack for the message, so can not proceed");
    return;
  }

  strcpy_mnt(final_message, final_message_length, _message, strlen(_message) + 1);

  if (_include_filepath)
  {
    strcat_mnt(final_message, final_message_length, separator, strlen(separator) + 1);
    strcat_mnt(final_message, final_message_length, m_log_file_name, strlen(m_log_file_name) + 1);
  }  


// Retriveing the description of error, it is mess ...
// On window "strerror_s" is used
// On unix "strerror_r" is used, but two implementation exits with different functionalities
// The GNU version is used when __USE_GNU is defined, otherwise Posix version will be used
#ifdef _WIN32
  if ( strerror_s(reason, reason_length, errno) == 0 )
  {
    strcat_mnt(final_message, final_message_length, separator, strlen(separator) + 1);
    strcat_mnt(final_message, final_message_length, reason, strlen(reason) + 1);
  }
#elif defined __USE_XOPEN2K && !defined __USE_GNU
  if ( strerror_r( errno, reason, reason_length) == 0 )
  {
    strcat_mnt(final_message, final_message_length, separator, strlen(separator) + 1);
    strcat_mnt(final_message, final_message_length, reason, strlen(reason) + 1);
  }
#else
  // The GNU version of strerror_r returns the description as a char* and may or may not
  // use the provided buffer
  char* GNU_version_reason = strerror_r( errno, reason, reason_length);
  strcat_mnt(final_message, final_message_length, separator, strlen(separator) + 1);
  strcat_mnt(final_message, final_message_length, GNU_version_reason, strlen(GNU_version_reason) + 1);
#endif

  MNT_ERORR_LL(final_message);
}
