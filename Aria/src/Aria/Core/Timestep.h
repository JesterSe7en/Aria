#pragma once

namespace ARIA {
class Timestep {
 public:
  Timestep(float time = 0.0f) : mTime(time) {}

  operator float() const { return mTime; }

  float get_seconds() { return mTime; };
  float get_milliseconds() { return mTime * 1000.0f; }

 private:
  float mTime = 0.0f;
};

}  // namespace ARIA