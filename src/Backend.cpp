#include "Backend.h"

#include <stdexcept>

#include "SDL.h"

Backend::Texture::Texture(void *data) : data(data)
{
}

Backend::Texture::~Texture()
{
    if (this->data != nullptr)
        SDL_DestroyTexture((SDL_Texture *)this->data);
}

Backend::Backend(const char *title, unsigned width, unsigned height) : width(width), height(height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        throw std::runtime_error(SDL_GetError());

    this->window =
        SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);

    if (this->window == nullptr)
        throw std::runtime_error(SDL_GetError());

    this->renderer =
        SDL_CreateRenderer((SDL_Window *)this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (this->renderer == nullptr)
        throw std::runtime_error(SDL_GetError());

    SDL_RenderSetLogicalSize((SDL_Renderer *)this->renderer, width, height);
}

Backend::~Backend()
{
    // free all textures in texture cache
    for (auto t : this->textureCache)
        delete t.second;

    // destroy renderer
    SDL_DestroyRenderer((SDL_Renderer *)this->renderer);

    // destroy window
    SDL_DestroyWindow((SDL_Window *)this->window);

    // shutdown SDL library
    SDL_Quit();
}

bool Backend::IsRunning() const noexcept
{
    return this->running;
}

void Backend::UpdateEverything() noexcept
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            this->running = false;
            break;
        }
    }
}

AssetManager &Backend::GetAssetManager()
{
    return this->assetManager;
}

void Backend::GetWindowSize(Vector2<int> &size) const noexcept
{
    size.x = this->width;
    size.y = this->height;
}

void Backend::ClearDisplay(const Color &color) noexcept
{
    SDL_SetRenderDrawColor((SDL_Renderer *)this->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear((SDL_Renderer *)this->renderer);
}

void Backend::DrawLine(const Vector2<float> &p1, const Vector2<float> &p2, const Color &color) noexcept
{
    SDL_SetRenderDrawColor((SDL_Renderer *)this->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLineF((SDL_Renderer *)this->renderer, p1.x, p1.y, p2.x, p2.y);
}

void Backend::DrawRectangle(const Rectangle &rect, const Color &color) noexcept
{
    SDL_SetRenderDrawColor((SDL_Renderer *)this->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRectF((SDL_Renderer *)this->renderer, (SDL_FRect *)&rect);
}

void Backend::FillRectangle(const Rectangle &rect, const Color &color) noexcept
{
    SDL_SetRenderDrawColor((SDL_Renderer *)this->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRectF((SDL_Renderer *)this->renderer, (SDL_FRect *)&rect);
}

Backend::Texture *Backend::InitTexture(void *textureData) noexcept
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface((SDL_Renderer *)this->renderer, (SDL_Surface *)textureData);

    if (texture == nullptr)
        return nullptr;

    return new Texture((void *)texture);
}

void Backend::DrawTexture(Backend::Texture *texture, const Rectangle &source, const Rectangle &destination) noexcept
{
    if (texture == nullptr)
        return;

    SDL_Rect src = {static_cast<int>(source.pos.x), static_cast<int>(source.pos.y), static_cast<int>(source.size.x),
                    static_cast<int>(source.size.y)};

    SDL_RenderCopyF((SDL_Renderer *)this->renderer, (SDL_Texture *)texture->data, &src,
                    (const SDL_FRect *)&destination);
}

void Backend::DrawTexture(Backend::Texture *texture, const Rectangle &source, const Rectangle &destination,
                          const double angle, const Vector2<float> &center, const unsigned flip) noexcept
{
    if (texture == nullptr)
        return;

    SDL_Rect src = {static_cast<int>(source.pos.x), static_cast<int>(source.pos.y), static_cast<int>(source.size.x),
                    static_cast<int>(source.size.y)};

    SDL_RenderCopyExF((SDL_Renderer *)this->renderer, (SDL_Texture *)texture->data, &src,
                      (const SDL_FRect *)&destination, angle, (const SDL_FPoint *)&center,
                      (const SDL_RendererFlip)flip);
}

Backend::Texture *Backend::GetCachedTexture(std::string_view name) noexcept
{
    if (this->textureCache.find(name) != this->textureCache.end())
        return this->textureCache[name];
    else
        return nullptr;
}

void Backend::AddCachedTexture(Backend::Texture *texture, std::string_view name) noexcept
{
    this->textureCache[name] = texture;
}

void Backend::GetMouseState(MouseState &state) noexcept
{
    int rawMouseX, rawMouseY;
    int buttons = SDL_GetMouseState(&rawMouseX, &rawMouseY);

    float mouseX, mouseY;

    SDL_RenderWindowToLogical((SDL_Renderer *)this->renderer, rawMouseX, rawMouseY, &mouseX, &mouseY);

    state.posX = mouseX;
    state.posY = mouseY;

    switch (buttons)
    {
    case SDL_BUTTON(1):
        state.button = MouseButton::Left;
        break;
    case SDL_BUTTON(2):
        state.button = MouseButton::Middle;
        break;
    case SDL_BUTTON(3):
        state.button = MouseButton::Right;
        break;
    default:
        state.button = MouseButton::None;
        break;
    }
}

bool Backend::IsKeyDown(KeyCode keyCode) noexcept
{
    uint16_t key = static_cast<uint16_t>(keyCode);

    int size = 0;
    auto keys = SDL_GetKeyboardState(&size);

    if (key < size)
        return keys[key];
    else
        return false;
}

std::string Backend::GetBasePath() noexcept
{
    return std::string(SDL_GetBasePath());
}

std::string Backend::GetPrefPath(const char *org, const char *app) noexcept
{
    return std::string(SDL_GetPrefPath(org, app));
}

void Backend::Present() noexcept
{
    SDL_RenderPresent((SDL_Renderer *)this->renderer);
}