#ifndef PUBGAME_H
#define PUBGAME_H

#include <vector>
#include <zero/Math.h>

namespace zero {

struct DevaSafes {
  bool Empty() { return f0.empty() || f1.empty(); }
  void Clear() {
    f0.clear();
    f1.clear();
  }

  std::vector<Vector2f> f0;
  std::vector<Vector2f> f1;
};

struct HSAnchorPoints {

    HSAnchorPoints(Vector2f flagroom, Vector2f gate) : flagroom(flagroom), gate(gate) {}

  Vector2f flagroom;
  Vector2f gate;
};



}

#endif
