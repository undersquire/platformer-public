#pragma once

#include "Scene.h"

class ControlsScreen : public Scene
{
  public:
    ControlsScreen(Backend *) noexcept;

    void Update(float) noexcept override;
    void Render() noexcept override;

  private:
    void RenderText(std::string_view, const Rectangle &) noexcept;

  private:
    // clear color
    Backend::Color clearColor = {0, 0, 0};

    // keyboard state
    bool locked = false;

    // font
    Backend::Texture *fontMap;

    // window size
    Vector2<int> windowSize;
};