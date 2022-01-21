#pragma once

#include <algorithm>

#include "Backend.h"
#include "Map.h"
#include "Math.h"
#include "Sprite.h"

enum class EntityType
{
    Player,
    Bat,
    Ghost,
    Zombie,
    Skeleton,
    GoldItem,
    PickaxeItem,
    CaveExit
};

class Entity
{
  public:
    EntityType type;

    Rectangle entityBox;
    Vector2<float> velocity = {0, 0};

    bool hasGravity, hasCollision, hasEntityCollision, boundsReset, hasTarget, damageable;

    int health = 0;
    int directionX = 0, directionY = 0;

    Entity *target = nullptr;

  public:
    Entity(Backend *, EntityType) noexcept;

    virtual void Update(float) noexcept
    {
    }

    virtual void Render(const Rectangle &) noexcept
    {
    }

    virtual bool OnCollide(Entity *, const Vector2<float> &) noexcept
    {
        return false;
    }

    virtual bool OnCollide(int, Tile &, const Vector2<float> &, const Rectangle &) noexcept
    {
        return true;
    }

  protected:
    Backend *backend;
};