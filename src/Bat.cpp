#include "Bat.h"

Bat::Bat(Backend *backend, Vector2<float> &spawn) noexcept
    : Entity(backend, EntityType::Bat) {
  this->entityBox.pos = spawn;
  this->entityBox.size = {32, 32};

  this->hasGravity = false;
  this->hasCollision = false;
  this->hasEntityCollision = false;

  this->boundsReset = true;
  this->hasTarget = false;

  this->health = 1;

  this->damageable = true;

  this->directionX = -1;
  this->directionY = -1;

  this->spriteMap = this->backend->GetCachedTexture("sprites");

  // create fly sprite
  Sprite fly(0.1);

  for (int i = 0; i < 4; i++)
    fly.frames.push_back({{(36.f + i) * 16.f, 37.f * 16.f}, {16.f, 16.f}});

  // create deathFall sprite
  Sprite deathFall(0.1);

  deathFall.frames.push_back({{41.f * 16.f, 37.f * 16.f}, {16.f, 16.f}});

  // create death sprite
  Sprite death(0.1);

  for (int i = 0; i < 4; i++)
    death.frames.push_back({{(41.f + i) * 16.f, 37.f * 16.f}, {16.f, 16.f}});

  // add sprites to sprite sheet
  this->spriteSheet.sprites["fly"] = fly;
  this->spriteSheet.sprites["deathFall"] = deathFall;
  this->spriteSheet.sprites["death"] = death;

  this->spriteSheet.SetSprite("fly");
}

void Bat::Update(float deltaTime) noexcept {
  if (health > 0) {
    if (this->directionX == -1) {
      if (this->velocity.x > -this->SPEED)
        this->velocity.x -= this->ACCEL;
    } else if (this->directionX == 1) {
      if (this->velocity.x < this->SPEED)
        this->velocity.x += this->ACCEL;
    }

    if (std::abs(this->velocity.y) < this->FALL_SPEED)
      this->velocity.y += this->FALL_ACCEL * this->directionY;
    else {
      this->directionY *= -1;
      this->velocity.y += this->FALL_ACCEL * this->directionY;
    }
  } else {
    if (this->dead) {
      if (!this->waitForRemoval) {
        if (this->spriteSheet.IsLastFrame()) {
          this->deathTimer.Reset();

          this->waitForRemoval = true;

          this->spriteSheet.Lock();
        }
      } else {
        if (this->deathTimer.Elapsed() >= 3.f) {
          this->health = -1;
        }
      }
    } else {
      if (!this->deathFalling) {
        this->hasGravity = true;
        this->hasCollision = true;

        this->deathFalling = true;

        this->velocity.x = 0;
        this->velocity.y = 0;

        this->spriteSheet.SetSprite("deathFall");
      }
    }
  }

  this->spriteSheet.NextFrame();
}

void Bat::Render(const Rectangle &camera) noexcept {
  Rectangle dest = {this->entityBox.pos - camera.pos, {48.f, 48.f}};

  dest.pos.x = dest.pos.x - 24.f + (this->entityBox.size.x / 2);
  dest.pos.y = dest.pos.y - (this->entityBox.size.y / 2);

  const Vector2<float> center = {0.f, 0.f};

  // render player
  this->backend->DrawTexture(this->spriteMap, *this->spriteSheet.GetFrame(),
                             dest, 0.f, center, this->directionX == -1 ? 1 : 0);
}

bool Bat::OnCollide(int i, Tile &tile, const Vector2<float> &contact,
                    const Rectangle &tile_pos) noexcept {
  if (this->deathFalling) {
    if (contact.y == -1) {
      this->dead = true;
      this->deathFalling = false;

      this->spriteSheet.SetSprite("death");
    }
  }

  return true;
}
