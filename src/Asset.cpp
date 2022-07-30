#include "Asset.h"

#include "SDL.h"
#include "stb_image.h"

Asset::Asset(AssetType type, const char *name, void *data) noexcept
    : type(type), name(name), data(data) {}

AssetType Asset::GetType() const noexcept { return this->type; }

std::string_view Asset::GetName() const noexcept { return this->name; }

void *Asset::GetData() const noexcept { return this->data; }

Texture::Texture(const char *name, void *data, int width, int height,
                 int pixelSize) noexcept
    : Asset(AssetType::Texture, name, data), width(width), height(height),
      pixelSize(pixelSize) {
  SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormatFrom(
      data, width, height, 32, 4 * width, SDL_PIXELFORMAT_RGBA32);

  SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 135, 175, 96));

  this->textureData = (void *)surface;
}

Texture::~Texture() noexcept {
  SDL_FreeSurface((SDL_Surface *)this->textureData);
  stbi_image_free(this->GetData());
}

int Texture::GetWidth() const noexcept { return this->width; }

int Texture::GetHeight() const noexcept { return this->height; }

int Texture::GetPixelSize() const noexcept { return this->pixelSize; }

void *Texture::GetTextureData() const noexcept { return this->textureData; }
