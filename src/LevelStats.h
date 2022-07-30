#pragma once

#include "Scene.h"
#include "ScoreKeeper.h"
#include "Titlescreen.h"

class LevelStats : public Scene {
public:
  LevelStats(Backend *, ScoreKeeper) noexcept;
  ~LevelStats();

  void Update(float) noexcept override;
  void Render() noexcept override;

private:
  void RenderText(std::string_view text, const Rectangle &destination) noexcept;

private:
  // clear color
  const Backend::Color CLEAR_COLOR = {0, 0, 0};

  // sprite maps
  Backend::Texture *fontMap;
  Backend::Texture *spriteMap;

  // window size
  Vector2<int> windowSize;

  // score keeper
  ScoreKeeper scoreKeeper;
};
