#pragma once

#include <unordered_map>

#include "Asset.h"

class AssetManager {
public:
  ~AssetManager();

  Asset *GetAsset(const char *);

  bool LoadAsset(const char *, const char *, AssetType);

private:
  std::unordered_map<std::string_view, Asset *> assets;
};
