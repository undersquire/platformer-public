#pragma once

#include <string>

enum class AssetType { Texture, Sound, Music, Font };

class Asset {
public:
  Asset(AssetType, const char *, void *) noexcept;

  AssetType GetType() const noexcept;

  std::string_view GetName() const noexcept;

  void *GetData() const noexcept;

private:
  AssetType type;
  std::string_view name;

  void *data;
};

class Texture : public Asset {
public:
  Texture(const char *, void *, int, int, int) noexcept;
  ~Texture() noexcept;

  int GetWidth() const noexcept;

  int GetHeight() const noexcept;

  int GetPixelSize() const noexcept;

  void *GetTextureData() const noexcept;

private:
  int width, height, pixelSize;
  void *textureData;
};

class Font : public Asset {
public:
  Font(const char *, void *) noexcept;
};
