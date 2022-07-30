#include "Zombie.h"

Zombie::Zombie(Backend *backend, Vector2<float> &spawn) noexcept
    : Entity(backend, EntityType::Zombie) {
  this->entityBox.pos = spawn;
  this->entityBox.size = {31, 31};

  this->hasGravity = true;
  this->hasCollision = true;
  this->hasEntityCollision = true;

  this->boundsReset = true;
  this->hasTarget = true;

  this->health = 1;

  this->damageable = true;

  this->directionX = -1;

  this->spriteMap = this->backend->GetCachedTexture("sprites");

  // create walk sprite
  Sprite walk(0.1);

  for (int i = 0; i < 3; i++)
    walk.frames.push_back({{(36.f + i) * 16.f, 39.f * 16.f}, {16.f, 16.f}});

  // death sprite
  Sprite death(0.1);

  for (int i = 0; i < 8; i++)
    death.frames.push_back({{(41.f + i) * 16.f, 39.f * 16.f}, {16.f, 16.f}});

  // add sprites to sprite sheet
  this->spriteSheet.sprites["walk"] = walk;
  this->spriteSheet.sprites["death"] = death;

  this->spriteSheet.SetSprite("walk");
}

void Zombie::Update(float deltaTime) noexcept {
  if (this->health > 0) {
    if (this->target != nullptr) {
      if (this->entityBox.pos.x < this->target->entityBox.pos.x)
        this->directionX = 1;
      else
        this->directionX = -1;
    }

    if (this->directionX == -1) {
      if (this->velocity.x > -this->SPEED)
        this->velocity.x -= this->ACCEL;
    } else if (this->directionX == 1) {
      if (this->velocity.x < this->SPEED)
        this->velocity.x += this->ACCEL;
    }
  } else {
    if (!this->dead) {
      this->dead = true;
      this->velocity.x = 0;

      this->spriteSheet.Reset();
      this->spriteSheet.SetSprite("death");
    } else {
      if (!this->waitForRemoval) {
        if (this->spriteSheet.IsLastFrame()) {
          this->waitForRemoval = true;
          this->deathTimer.Reset();

          this->spriteSheet.Lock();
        }
      } else {
        if (this->deathTimer.Elapsed() > 3.f)
          this->health = -1;
      }
    }
  }

  this->spriteSheet.NextFrame();
}

void Zombie::Render(const Rectangle &camera) noexcept {
  Rectangle dest = {this->entityBox.pos - camera.pos, {48.f, 48.f}};

  dest.pos.x = dest.pos.x - 24.f + (this->entityBox.size.x / 2);
  dest.pos.y = dest.pos.y - (this->entityBox.size.y / 2);

  const Vector2<float> center = {0.f, 0.f};

  // render sprite
  this->backend->DrawTexture(this->spriteMap, *this->spriteSheet.GetFrame(),
                             dest, 0.f, center, this->directionX == -1 ? 1 : 0);
}
