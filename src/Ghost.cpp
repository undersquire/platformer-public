#include "Ghost.h"

Ghost::Ghost(Backend *backend, Vector2<float> &spawn) noexcept
    : Entity(backend, EntityType::Ghost) {
  this->entityBox.pos = spawn;
  this->entityBox.size = {32, 32};

  this->hasGravity = false;
  this->hasCollision = false;
  this->hasEntityCollision = false;

  this->boundsReset = true;
  this->hasTarget = true;

  this->damageable = false;

  this->health = 1;

  this->directionX = -1;

  this->spriteMap = this->backend->GetCachedTexture("sprites");

  // create fly sprite
  Sprite fly(0.1);

  for (int i = 0; i < 4; i++)
    fly.frames.push_back({{(36.f + i) * 16.f, 33.f * 16.f}, {16.f, 16.f}});

  // create hide sprite
  Sprite hide(1.0);

  hide.frames.push_back({{40.f * 16.f, 35.f * 16.f}, {16.f, 16.f}});

  // add sprites to sprite sheet
  this->spriteSheet.sprites["fly"] = fly;
  this->spriteSheet.sprites["hide"] = hide;

  this->spriteSheet.SetSprite("fly");
}

void Ghost::Update(float deltaTime) noexcept {
  if (this->target != nullptr) {
    if (this->entityBox.pos.x > this->target->entityBox.pos.x &&
        this->target->directionX == -1) {
      this->moving = true;
      this->directionX = -1;
    } else if (this->entityBox.pos.x < this->target->entityBox.pos.x &&
               this->target->directionX == 1) {
      this->moving = true;
      this->directionX = 1;
    } else
      this->moving = false;

    if (this->entityBox.pos.y > this->target->entityBox.pos.y)
      this->directionY = -1;
    else if (this->entityBox.pos.y < this->target->entityBox.pos.y)
      this->directionY = 1;
    else
      this->directionY = 0;
  }

  if (this->moving) {
    this->spriteSheet.SetSprite("fly");

    if (directionX == -1) {
      if (this->velocity.x > -this->SPEED)
        this->velocity.x -= this->ACCEL;
    } else if (directionX == 1) {
      if (this->velocity.x < this->SPEED)
        this->velocity.x += this->ACCEL;
    }

    if (directionY == -1) {
      if (this->velocity.y > -this->SPEED)
        this->velocity.y -= this->ACCEL;
    } else if (directionY == 1) {
      if (this->velocity.y < this->SPEED)
        this->velocity.y += this->ACCEL;
    }
  } else {
    this->velocity.x = 0;
    this->velocity.y = 0;

    this->spriteSheet.SetSprite("hide");
  }

  this->spriteSheet.NextFrame();
}

void Ghost::Render(const Rectangle &camera) noexcept {
  Rectangle dest = {this->entityBox.pos - camera.pos, {48.f, 48.f}};

  dest.pos.x = dest.pos.x - 24.f + (this->entityBox.size.x / 2);
  dest.pos.y = dest.pos.y - (this->entityBox.size.y / 2);

  const Vector2<float> center = {0.f, 0.f};

  // render sprite
  this->backend->DrawTexture(this->spriteMap, *this->spriteSheet.GetFrame(),
                             dest, 0.f, center, this->directionX == -1 ? 1 : 0);
}
