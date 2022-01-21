#pragma once

#include <chrono>
#include <cstdint>

class Timer
{
  public:
    Timer();

    void Reset();
    float Elapsed();

  private:
    std::chrono::time_point<std::chrono::system_clock> start;
};