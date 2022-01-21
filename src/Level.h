#pragma once

#include "Bat.h"
#include "CaveExit.h"
#include "Entity.h"
#include "Ghost.h"
#include "Gold.h"
#include "Map.h"
#include "Player.h"
#include "Scene.h"
#include "ScoreKeeper.h"
#include "Skeleton.h"
#include "Zombie.h"

class Level : public Scene
{
  public:
    Level(Backend *) noexcept;
    ~Level();

    void Update(float) noexcept override;
    void Render() noexcept override;

  private:
    void RenderText(std::string_view, const Rectangle &) noexcept;

  private:
    // clear color
    Backend::Color clearColor = {0, 0, 0};

    // paused state
    bool paused = false, pauseBlock = true;

    // sprite maps
    Backend::Texture *fontMap;
    Backend::Texture *spriteMap;
    Backend::Texture *background;

    // our world
    Map map;

    // our camera
    Rectangle camera;

    // our entity list
    std::vector<Entity *> entities;

    // world gravity
    const float GRAVITY_MAX = 500.f;
    const float GRAVITY_ACCEL = 25.f;

    // hitbox color
    const Backend::Color HITBOX_COLOR = {255, 0, 0};

    // window size
    Vector2<int> windowSize;

    // score keeper
    ScoreKeeper scoreKeeper;
};