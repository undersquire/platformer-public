#pragma once

#include "Entity.h"

class CaveExit : public Entity
{
  public:
    CaveExit(Backend *, Vector2<float> &) noexcept;

    void Update(float) noexcept override;
    void Render(const Rectangle &) noexcept override;

  private:
    Backend::Texture *spriteMap;
    SpriteSheet spriteSheet;
};