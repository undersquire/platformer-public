#pragma once

#include "Entity.h"

class Gold : public Entity {
public:
  Gold(Backend *, Vector2<float> &) noexcept;

  void Update(float) noexcept override;
  void Render(const Rectangle &) noexcept override;

private:
  Backend::Texture *spriteMap;
  SpriteSheet spriteSheet;
};
