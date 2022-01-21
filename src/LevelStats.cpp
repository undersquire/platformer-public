#include "LevelStats.h"

LevelStats::LevelStats(Backend *backend, ScoreKeeper scoreKeeper) noexcept : Scene(backend), scoreKeeper(scoreKeeper)
{
    this->fontMap = this->backend->GetCachedTexture("font");
    this->spriteMap = this->backend->GetCachedTexture("sprites");

    this->backend->GetWindowSize(this->windowSize);
}

void LevelStats::Update(float deltaTime) noexcept
{
    if (this->backend->IsKeyDown(Backend::KeyCode::RETURN))
        this->transition = new Titlescreen(this->backend);
}

void LevelStats::Render() noexcept
{
    this->backend->ClearDisplay(this->CLEAR_COLOR);

    // window width
    float width = this->windowSize.x;
    float height = this->windowSize.y;

    // render completion text
    const Rectangle title = {{width / 8, 80.f}, {width - (width / 4), 80.f}};

    this->RenderText("LEVEL`COMPLETE", title);

    // render ingame stats
    std::string gold_string = std::to_string(this->scoreKeeper.gold);
    std::string score_string = std::to_string(this->scoreKeeper.score);

    // render gold stats
    Rectangle gold_text = {{0.f, height / 2}, {24.f * ceilf(gold_string.length() + 8), 36.f}};

    gold_text.pos.x = (width / 2) - (gold_text.size.x / 2);

    this->RenderText(std::string("GOLD:`") + gold_string, gold_text);

    // render score stats
    Rectangle score_text = {{0.f, (height / 2) + 100.f}, {24.f * ceilf(score_string.length() + 8), 36.f}};

    score_text.pos.x = (width / 2) - (score_text.size.x / 2);

    this->RenderText(std::string("SCORE:`") + score_string, score_text);
}

void LevelStats::RenderText(std::string_view text, const Rectangle &destination) noexcept
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