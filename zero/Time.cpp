#include "Time.h"


namespace zero {

uint64_t Time::GetTime() {
  return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now())
      .time_since_epoch()
      .count();
}

PerformanceTimer::PerformanceTimer() {
  begin_time = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now())
                   .time_since_epoch()
                   .count();
  last_update_time = begin_time;
}

uint64_t PerformanceTimer::GetTimeSinceConstruction() {
  uint64_t now = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now())
                     .time_since_epoch()
                     .count();

  uint64_t elapsed = now - begin_time;

  return elapsed;
}

uint64_t PerformanceTimer::GetElapsedTime() {
  uint64_t now = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now())
                     .time_since_epoch()
                     .count();

  uint64_t elapsed = now - last_update_time;

  last_update_time = now;

  return elapsed;
}
}  // namespace zero