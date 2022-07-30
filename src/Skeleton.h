#pragma once

#include "Entity.h"

class Skeleton : public Entity {
  const float SPEED = 50.f;
  const float ACCEL = 10.f;

public:
  Skeleton(Backend *, Vector2<float> &) noexcept;

  void Update(float) noexcept override;
  void Render(const Rectangle &) noexcept override;

  bool OnCollide(int, Tile &, const Vector2<float> &,
                 const Rectangle &) noexcept override;

private:
  Backend::Texture *spriteMap;
  SpriteSheet spriteSheet;

  int tileTicks = 0;

  Timer deathTimer;
  bool dead = false, waitingLife = false, reviving = false;
};
