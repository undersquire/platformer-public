#pragma once

#include "Entity.h"
#include "ScoreKeeper.h"

class Player : public Entity {
  const float WALK_SPEED = 150.f;
  const float RUN_SPEED = 250.f;

  const float ACCEL = 25.f;

  const float JUMP_ACCEL = 150.f;
  const float JUMP_VELOCITY = 600.f;

public:
  float moveSpeed = WALK_SPEED;

public:
  Player(Backend *, ScoreKeeper *) noexcept;

  void Update(float) noexcept override;
  void Render(const Rectangle &) noexcept override;

  bool OnCollide(Entity *, const Vector2<float> &) noexcept override;
  bool OnCollide(int, Tile &, const Vector2<float> &,
                 const Rectangle &) noexcept override;

private:
  bool isRunning = false, isJumping = false, isOnGround = false;
  unsigned jumpTicks;

  float friction = 1.f;

  Backend::Texture *spriteMap;
  SpriteSheet spriteSheet;

  Timer deathTimer;
  bool dead = false;

  ScoreKeeper *scoreKeeper;
};
