#include "Skeleton.h"

Skeleton::Skeleton(Backend *backend, Vector2<float> &spawn) noexcept
    : Entity(backend, EntityType::Skeleton) {
  this->entityBox.pos = spawn;
  this->entityBox.size = {31, 31};

  this->hasGravity = true;
  this->hasCollision = true;
  this->hasEntityCollision = true;

  this->boundsReset = true;
  this->hasTarget = false;

  this->health = 1;

  this->damageable = true;

  this->directionX = -1;

  this->spriteMap = this->backend->GetCachedTexture("sprites");

  // create walk sprite
  Sprite walk(0.2);

  for (int i = 0; i < 3; i++)
    walk.frames.push_back({{(36.f + i) * 16.f, 31.f * 16.f}, {16.f, 16.f}});

  // death sprite
  Sprite death(0.1);

  death.reverseReset = true;

  for (int i = 0; i < 8; i++)
    death.frames.push_back({{(42.f + i) * 16.f, 31.f * 16.f}, {16.f, 16.f}});

  // add sprites to sprite sheet
  this->spriteSheet.sprites["walk"] = walk;
  this->spriteSheet.sprites["death"] = death;

  this->spriteSheet.SetSprite("walk");
}

void Skeleton::Update(float deltaTime) noexcept {
  if (this->health > 0) {
    if (this->tileTicks > 45) {
      this->directionX *= -1;
      this->tileTicks = 0;
    } else
      this->tileTicks++;

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

      this->spriteSheet.SetSprite("death");
    } else {
      if (!this->waitingLife) {
        if (this->spriteSheet.IsLastFrame()) {
          this->waitingLife = true;

          this->deathTimer.Reset();
          this->spriteSheet.Lock();
        }
      } else {
        if (!this->reviving) {
          if (this->deathTimer.Elapsed() >= 5.f) {
            this->reviving = true;
            this->spriteSheet.Lock();
          }
        } else {
          if (this->spriteSheet.IsFirstFrame()) {
            this->health = 1;
            this->dead = false;
            this->waitingLife = false;
            this->reviving = false;

            this->spriteSheet.SetSprite("walk");
          }
        }
      }
    }
  }

  this->spriteSheet.NextFrame();
}

void Skeleton::Render(const Rectangle &camera) noexcept {
  Rectangle dest = {this->entityBox.pos - camera.pos, {48.f, 48.f}};

  dest.pos.x = dest.pos.x - 24.f + (this->entityBox.size.x / 2);
  dest.pos.y = dest.pos.y - (this->entityBox.size.y / 2);

  const Vector2<float> center = {0.f, 0.f};

  // render sprite
  this->backend->DrawTexture(this->spriteMap, *this->spriteSheet.GetFrame(),
                             dest, 0.f, center, this->directionX == -1 ? 1 : 0);
}

bool Skeleton::OnCollide(int i, Tile &tile, const Vector2<float> &contact,
                         const Rectangle &tile_pos) noexcept {
  int myY = static_cast<int>(this->entityBox.pos.y) / tile_pos.size.y;
  int tileY = static_cast<int>(tile_pos.pos.y) / tile_pos.size.y;

  if ((i == 0 || i == 1) && contact.x != 0 && myY == tileY)
    this->directionX = contact.x;

  if (contact.x != 0 && myY < tileY)
    this->tileTicks = 0;

  return true;
}
