#pragma once

#include <cstdint>
#include <vector>

#include "Math.h"
#include "Sprite.h"

struct Tile {
  enum class Type { Solid, Passable, Platform };

  Type type;
  float friction;
  Rectangle sprite;

  std::vector<uint32_t> residents;
};

static const Tile TILE_TABLE[] = {
    /* 0 - air                   */
    {Tile::Type::Passable, .97f, {{-1.f, -1.f}, {-1.f, -1.f}}},
    /* 1 - solid air             */
    {Tile::Type::Solid, .97f, {{-1.f, -1.f}, {-1.f, -1.f}}},
    /* 2 - left stone top        */
    {Tile::Type::Solid, .89f, {{2.f * 16.f, 7.f * 16.f}, {16.f, 16.f}}},
    /* 3 - middle stone top      */
    {Tile::Type::Solid, .89f, {{3.f * 16.f, 7.f * 16.f}, {16.f, 16.f}}},
    /* 4 - right stone top       */
    {Tile::Type::Solid, .89f, {{7.f * 16.f, 7.f * 16.f}, {16.f, 16.f}}},
    /* 5 - left stone bottom     */
    {Tile::Type::Solid, .89f, {{2.f * 16.f, 8.f * 16.f}, {16.f, 16.f}}},
    /* 6 - middle stone bottom   */
    {Tile::Type::Solid, .89f, {{3.f * 16.f, 8.f * 16.f}, {16.f, 16.f}}},
    /* 7 - right stone bottom    */
    {Tile::Type::Solid, .89f, {{7.f * 16.f, 8.f * 16.f}, {16.f, 16.f}}},
    /* 8 - left stone fill       */
    {Tile::Type::Solid, .89f, {{4.f * 16.f, 6.f * 16.f}, {16.f, 16.f}}},
    /* 9 - middle stone fill     */
    {Tile::Type::Solid, .89f, {{1.f * 16.f, 4.f * 16.f}, {16.f, 16.f}}},
    /* A - right stone fill      */
    {Tile::Type::Solid, .89f, {{5.f * 16.f, 6.f * 16.f}, {16.f, 16.f}}},
    /* B - left catwalk top      */
    {Tile::Type::Platform, .89f, {{34.f * 16.f, 6.f * 16.f}, {16.f, 16.f}}},
    /* C - right catwalk top     */
    {Tile::Type::Platform, .89f, {{35.f * 16.f, 6.f * 16.f}, {16.f, 16.f}}},
    /* D - left scaffold bottom  */
    {Tile::Type::Passable, .89f, {{34.f * 16.f, 7.f * 16.f}, {16.f, 16.f}}},
    /* E - right scaffold bottom */
    {Tile::Type::Passable, .89f, {{35.f * 16.f, 7.f * 16.f}, {16.f, 16.f}}},
    /* F - left scaffold top     */
    {Tile::Type::Passable, .89f, {{34.f * 16.f, 8.f * 16.f}, {16.f, 16.f}}},
    /* G - right scaffold top    */
    {Tile::Type::Passable, .89f, {{35.f * 16.f, 8.f * 16.f}, {16.f, 16.f}}}};

struct Map {
  float tileSize;
  std::vector<std::vector<Tile>> tiles;
};
