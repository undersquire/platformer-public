#include "ControlsScreen.h"

#include "Level.h"

ControlsScreen::ControlsScreen(Backend *backend) noexcept : Scene(backend) {
  this->fontMap = this->backend->GetCachedTexture("font");

  this->backend->GetWindowSize(this->windowSize);

  if (this->backend->IsKeyDown(Backend::KeyCode::RETURN))
    this->locked = true;
}

void ControlsScreen::RenderText(std::string_view text,
                                const Rectangle &destination) noexcept {
  for (int i = 0; i < text.size(); i++) {
    auto c = text[i];

    if (c < 64)
      c -= 32;

    int y = c / 8;
    int x = c - (8 * y);

    const Rectangle source = {{x * 16.f, y * 16.f}, {16, 16}};
    const Vector2<float> center = {0.f, 0.f};

    float width = destination.size.x / text.size();
    const Rectangle dest = {
        {destination.pos.x + (i * width), destination.pos.y},
        {width, destination.size.y}};

    this->backend->DrawTexture(this->fontMap, source, dest, 0.f, center, 0);
  }
}

void ControlsScreen::Update(float deltaTime) noexcept {
  if (this->backend->IsKeyDown(Backend::KeyCode::RETURN)) {
    if (!locked) {
      this->locked = true;
      this->transition = new Level(this->backend);
    }
  } else
    this->locked = false;
}

void ControlsScreen::Render() noexcept {
  // clear screen
  this->backend->ClearDisplay(this->clearColor);

  // window width
  float width = this->windowSize.x;

  // render title text

  const Rectangle title = {{width / 8, 80.f}, {width - (width / 4), 80.f}};

  this->RenderText("HOW`TO`PLAY", title);

  // render instructions text
  const Rectangle moveLeft = {{width / 4, 200.f}, {width - (width / 2), 25.f}};

  this->RenderText("MOVE`LEFT`-`A`/`LEFT`ARROW", moveLeft);

  const Rectangle moveRight = {{width / 4, 250.f}, {width - (width / 2), 25.f}};

  this->RenderText("MOVE`RIGHT`-`D`/`RIGHT`ARROW", moveRight);

  const Rectangle run = {{width / 4, 300.f}, {width - (width / 2), 25.f}};

  this->RenderText("```RUN`-`SHIFT```", run);

  const Rectangle jump = {{width / 4, 350.f}, {width - (width / 2), 25.f}};

  this->RenderText("```JUMP`-`SPACE`BAR```", jump);

  // render continue text
  const Rectangle text = {{width / 6, 420.f}, {width - (width / 3), 30.f}};

  this->RenderText("PRESS`ENTER`TO`CONTINUE", text);
}
