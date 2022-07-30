#pragma once

#include "Entity.h"

class Ghost : public Entity {
  const float SPEED = 75.f;
  const float ACCEL = 1.f;

public:
  Ghost(Backend *, Vector2<float> &) noexcept;

  void Update(float) noexcept override;
  void Render(const Rectangle &) noexcept override;

private:
  Backend::Texture *spriteMap;
  SpriteSheet spriteSheet;

  bool moving = false;
};
