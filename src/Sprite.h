#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Math.h"
#include "Timer.h"

class Sprite {
public:
  float frameSpeed;
  std::vector<Rectangle> frames;

  bool reverseReset = false;

public:
  Sprite() noexcept;
  Sprite(float) noexcept;

  bool Lock() noexcept;
  void Reset() noexcept;
  bool IsFirstFrame() noexcept;
  bool IsLastFrame() noexcept;
  void NextFrame() noexcept;
  Rectangle &GetFrame() noexcept;

private:
  int next = 0, dir = 1;
  Timer frameTimer;

  bool locked = false;
};

struct SpriteSheet {
  Sprite *currentSprite = nullptr;
  std::unordered_map<std::string_view, Sprite> sprites;

  bool Lock() noexcept;
  void Reset() noexcept;
  bool IsFirstFrame() noexcept;
  bool IsLastFrame() noexcept;
  void NextFrame() noexcept;
  Rectangle *GetFrame() noexcept;
  void SetSprite(std::string_view) noexcept;
};
