#include "Gold.h"

Gold::Gold(Backend *backend, Vector2<float> &spawn) noexcept
    : Entity(backend, EntityType::GoldItem) {
  this->entityBox.pos = spawn;
  this->entityBox.size = {32, 32};

  this->hasGravity = false;
  this->hasCollision = false;
  this->hasEntityCollision = false;

  this->boundsReset = true;
  this->hasTarget = false;

  this->damageable = false;

  this->health = 0;

  this->spriteMap = this->backend->GetCachedTexture("sprites");

  // create shine sprite
  Sprite shine(0.1);

  for (int i = 0; i < 4; i++)
    shine.frames.push_back({{(18.f + i) * 32.f, 20.5f * 32.f}, {32.f, 32.f}});

  // add sprites to sprite sheet
  this->spriteSheet.sprites["shine"] = shine;

  this->spriteSheet.SetSprite("shine");
}

void Gold::Update(float) noexcept { this->spriteSheet.NextFrame(); }

void Gold::Render(const Rectangle &camera) noexcept {
  Rectangle dest = {this->entityBox.pos - camera.pos, this->entityBox.size};

  const Vector2<float> center = {0.f, 0.f};

  // render sprite
  this->backend->DrawTexture(this->spriteMap, *this->spriteSheet.GetFrame(),
                             dest, 0.f, center, 0);
}
