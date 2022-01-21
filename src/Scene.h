#pragma once

#include "Backend.h"

class Scene
{
  public:
    Scene *transition = nullptr;

  public:
    Scene(Backend *) noexcept;

    virtual void Update(float) noexcept
    {
    }
    virtual void Render() noexcept
    {
    }

  protected:
    Backend *backend;
};