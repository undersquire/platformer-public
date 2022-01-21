#pragma once

#include "Entity.h"

class Bat : public Entity
{
    const float SPEED = 100.f;
    const float ACCEL = 50.f;

    const float FALL_SPEED = 150.f;
    const float FALL_ACCEL = 5.f;

    const Backend::Color COLOR = {55, 55, 0};

  public:
    Bat(Backend *, Vector2<float> &) noexcept;

    void Update(float) noexcept override;
    void Render(const Rectangle &) noexcept override;

    bool OnCollide(int, Tile &, const Vector2<float> &, const Rectangle &) noexcept override;

  private:
    Backend::Texture *spriteMap;
    SpriteSheet spriteSheet;

    bool deathFalling = false, dead = false, waitForRemoval = false;
    Timer deathTimer;
};