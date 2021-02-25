// File Name:     timer.cpp
// Author:        Arash Fatehi
// Date:          12th Feb 2021
// Description:   Measeres execution time from it`s creation till end of it`s scope

#include <sstream>

#include "timer.hpp"
#include "logger.hpp"

using namespace nne;

// Pass false as parameter to hinder automatic logging
Timer::Timer(bool p_auto_print)
{
  m_auto_print = p_auto_print;
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
