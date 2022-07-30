#include "Timer.h"

Timer::Timer() { this->Reset(); }

void Timer::Reset() { start = std::chrono::system_clock::now(); }

float Timer::Elapsed() {
  std::chrono::time_point<std::chrono::system_clock> time =
      std::chrono::system_clock::now();

  std::chrono::duration<float> elapsedTime = time - this->start;

  return elapsedTime.count();
}
