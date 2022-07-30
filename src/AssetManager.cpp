#include "AssetManager.h"

#include "stb_image.h"

AssetManager::~AssetManager() {
  for (auto &key : this->assets)
    delete key.second;

  this->assets.clear();
}

Asset *AssetManager::GetAsset(const char *name) {
  if (this->assets.find(name) != this->assets.end())
    return this->assets[name];
  else
    return nullptr;
}

bool AssetManager::LoadAsset(const char *file, const char *name,
                             AssetType type) {
  switch (type) {
  case AssetType::Texture: {
    int x, y, n;
    void *data = stbi_load(file, &x, &y, &n, STBI_rgb_alpha);

    if (data == nullptr)
      return false;

    this->assets[name] = new Texture(name, data, x, y, n);

    break;
  }
  default:
    return false;
    break;
  }

  return true;
}
