#include "CaveExit.h"

CaveExit::CaveExit(Backend *backend, Vector2<float> &spawn) noexcept
    : Entity(backend, EntityType::CaveExit) {
  this->entityBox.pos = spawn;
  this->entityBox.size = {64, 64};

  this->hasGravity = false;
  this->hasCollision = false;
  this->hasEntityCollision = false;

  this->boundsReset = true;
  this->hasTarget = false;

  this->damageable = false;

  this->health = 0;

  this->spriteMap = this->backend->GetCachedTexture("sprites");

  // create sprite
  Sprite exit(1.0);

  exit.frames.push_back({{22.5f * 32.f, 4.f * 32.f}, {32.f, 32.f}});

  // add sprites to sprite sheet
  this->spriteSheet.sprites["exit"] = exit;

  this->spriteSheet.SetSprite("exit");
}

void CaveExit::Update(float deltaTime) noexcept {
  this->spriteSheet.NextFrame();
}

void CaveExit::Render(const Rectangle &camera) noexcept {
  Rectangle dest = {this->entityBox.pos - camera.pos, this->entityBox.size};

  // render sprite
  this->backend->DrawTexture(this->spriteMap, *this->spriteSheet.GetFrame(),
                             dest);
}
