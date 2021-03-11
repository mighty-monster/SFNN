// File Name:     timer.hpp
// Author:        Arash Fatehi
// Date:          12th Feb 2021
// Description:   Measeres execution time from it`s creation till end of it`s scope

// ---------------------
// Detail Description:
// By creating an instance of object at begining of scope the execution time
// of targeted scope will be measured automatically
// --------------------

#pragma once

#include <iostream>
#include <chrono>

namespace mnt {
  class Timer
  {
  public:
    // Pass false as parameter to hinder automatic logging
    Timer(bool p_auto_print = true) noexcept;
    ~Timer() noexcept;

    void Start() noexcept;
    void Stop() noexcept;
    void PrintDuration() noexcept;

    // Returns duration with period of steady_clock,
    // Manual check of steady_clock::period::den is needed to interpret the result correctly
    std::chrono::duration<double, std::chrono::steady_clock::period> GetDuration();

  private:
    std::chrono::time_point<std::chrono::steady_clock> m_start, m_end;
    std::chrono::duration<double, std::chrono::steady_clock::period> m_duration;

    // If true, prints the duration in seconds automatically
    bool m_auto_print = true;
  };
}

