#include "Sprite.h"

Sprite::Sprite() noexcept : frameSpeed(1.0) {}

Sprite::Sprite(float frameSpeed) noexcept : frameSpeed(frameSpeed) {}

bool Sprite::Lock() noexcept {
  this->locked = !this->locked;

  return this->locked;
}

void Sprite::Reset() noexcept { this->next = 0; }

bool Sprite::IsFirstFrame() noexcept { return this->next == 0; }

bool Sprite::IsLastFrame() noexcept {
  if (this->next + 1 >= this->frames.size())
    return true;

  return false;
}

void Sprite::NextFrame() noexcept {
  if (!this->locked && this->frameTimer.Elapsed() >= this->frameSpeed) {
    this->next += this->dir;

    if (next < 0 || next >= this->frames.size()) {
      if (this->reverseReset) {
        this->dir *= -1;
        this->next += this->dir;
      } else
        this->next = 0;
    }

    this->frameTimer.Reset();
  }
}

Rectangle &Sprite::GetFrame() noexcept { return this->frames[next]; }

void SpriteSheet::NextFrame() noexcept {
  if (this->currentSprite != nullptr)
    this->currentSprite->NextFrame();
}

Rectangle *SpriteSheet::GetFrame() noexcept {
  if (this->currentSprite != nullptr)
    return &this->currentSprite->GetFrame();
  else
    return nullptr;
}

void SpriteSheet::SetSprite(std::string_view name) noexcept {
  if (this->sprites.find(name) != this->sprites.end())
    this->currentSprite = &this->sprites[name];
  else
    this->currentSprite = nullptr;
}

bool SpriteSheet::Lock() noexcept {
  if (this->currentSprite == nullptr)
    return false;

  return this->currentSprite->Lock();
}

void SpriteSheet::Reset() noexcept { this->currentSprite->Reset(); }

bool SpriteSheet::IsFirstFrame() noexcept {
  if (this->currentSprite == nullptr)
    return false;

  return this->currentSprite->IsFirstFrame();
}

bool SpriteSheet::IsLastFrame() noexcept {
  if (this->currentSprite == nullptr)
    return false;

  return this->currentSprite->IsLastFrame();
}
