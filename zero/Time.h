#pragma once

#include <chrono>

namespace zero {

class Time {
 public:
  uint64_t GetTime();

 private:
 
};


class PerformanceTimer {
 private:
  uint64_t begin_time;
  uint64_t last_update_time;

  public:
  PerformanceTimer();

  // Returns elapsed time and resets begin time
  uint64_t GetElapsedTime();
  // Returns elapsed time since this timer was created
  uint64_t GetTimeSinceConstruction();
};

}