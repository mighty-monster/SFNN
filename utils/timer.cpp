///////////////////////////////////////////////////////////////////////////////
// @File Name:     timer.hpp                                                 //
// @Author:        Arash Fatehi                                              //
// @L.M.D:         12th Feb 2021                                             //
// @Description:   Measeres execution time                                   //
//                                                                           //
// Detail Description:                                                       //
// By creating an instance of object at begining of scope the execution time //
// of targeted scope will be measured automatically                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <sstream>

#include "timer.hpp"
#include "logger.hpp"

using namespace nnc;

// Pass false as parameter to hinder automatic logging
Timer::Timer(bool auto_print)
{
  m_auto_print = auto_print;
  Start();
};

Timer::~Timer()
{
  Stop();
  if (m_auto_print)
    PrintDuration();
};

void Timer::Start()
{
  m_start = std::chrono::steady_clock::now();
};

void Timer::Stop()
{
  m_end = std::chrono::steady_clock::now();
  m_duration = m_end - m_start;
};

void Timer::PrintDuration()
{
  std::ostringstream  string_stream;
  string_stream << "Duration: " << m_duration.count()
                   * std::chrono::steady_clock::period::num
                   / std::chrono::steady_clock::period::den << " Seconds";

  Logger::Debug(string_stream.str());
};

// Returns duration with period of steady_clock,
// Manual check of steady_clock::period::den is needed to interpret the result correctly
std::chrono::duration<double, std::chrono::steady_clock::period> Timer::GetDuration()
{
  return m_duration;
};
