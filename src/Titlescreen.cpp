#include "Titlescreen.h"

#include "ControlsScreen.h"

Titlescreen::Titlescreen(Backend *backend) noexcept : Scene(backend)
{
    this->fontMap = this->backend->GetCachedTexture("font");

    this->backend->GetWindowSize(this->windowSize);

    if (this->backend->IsKeyDown(Backend::KeyCode::RETURN))
        this->locked = true;
}

void Titlescreen::RenderText(std::string_view text, const Rectangle &destination) noexcept
{
    for (int i = 0; i < text.size(); i++)
    {
        auto c = text[i];

        if (c < 64)
            c -= 32;

        int y = c / 8;
        int x = c - (8 * y);

        const Rectangle source = {{x * 16.f, y * 16.f}, {16, 16}};
        const Vector2<float> center = {0.f, 0.f};

        float width = destination.size.x / text.size();
        const Rectangle dest = {{destination.pos.x + (i * width), destination.pos.y}, {width, destination.size.y}};

        this->backend->DrawTexture(this->fontMap, source, dest, 0.f, center, 0);
    }
}

void Titlescreen::Update(float deltaTime) noexcept
{
    if (this->backend->IsKeyDown(Backend::KeyCode::RETURN))
    {
        if (!locked)
        {
            this->locked = true;
            this->transition = new ControlsScreen(this->backend);
        }
    }
    else
        this->locked = false;
}

void Titlescreen::Render() noexcept
{
    // clear screen
    this->backend->ClearDisplay(this->clearColor);

    // window width
    float width = this->windowSize.x;
    float height = this->windowSize.y;

    // render title text

    const Rectangle title = {{width / 8, 80.f}, {width - (width / 4), 80.f}};

    this->RenderText("GOLD`RUSH", title);

    // render play text
    const Rectangle text = {{width / 4, 220.f}, {width - (width / 2), 30.f}};

    this->RenderText("PRESS`ENTER`TO`START", text);

    // render author text
    const Rectangle author = {{10, height - 30}, {width / 3, 20}};

    this->RenderText("BY`UNDERSQUIRE`-`2021", author);
}