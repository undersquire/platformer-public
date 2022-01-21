#pragma once

#include "Entity.h"

class Zombie : public Entity
{
    const float SPEED = 75.f;
    const float ACCEL = 25.f;

  public:
    Zombie(Backend *, Vector2<float> &) noexcept;

    void Update(float) noexcept override;
    void Render(const Rectangle &) noexcept override;

  private:
    Backend::Texture *spriteMap;
    SpriteSheet spriteSheet;

    Timer deathTimer;
    bool dead = false, waitForRemoval = false;
};