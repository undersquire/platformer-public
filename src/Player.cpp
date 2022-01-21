#include "Player.h"

#include <algorithm>
#include <iostream>

Player::Player(Backend *backend, ScoreKeeper *scoreKeeper) noexcept
    : Entity(backend, EntityType::Player), scoreKeeper(scoreKeeper)
{
    this->entityBox.pos = {0.f, 0.f};
    this->entityBox.size = {28.f, 31.f};

    this->hasGravity = true;
    this->hasCollision = true;
    this->hasEntityCollision = true;

    this->boundsReset = false;
    this->hasTarget = false;

    this->damageable = false;

    this->health = 1;
    this->directionX = 1;

    this->spriteMap = this->backend->GetCachedTexture("sprites");

    // create idle sprite
    Sprite idle(0.5);

    for (int i = 0; i < 3; i++)
        idle.frames.push_back({{(43.f + i) * 16.f, 29.f * 16.f}, {16.f, 16.f}});

    // create walk sprite
    Sprite walk(0.1);

    for (int i = 0; i < 6; i++)
        walk.frames.push_back({{(36.f + i) * 16.f, 29.f * 16.f}, {16.f, 16.f}});

    // create run sprite
    Sprite run(0.05);

    for (int i = 0; i < 6; i++)
        run.frames.push_back({{(36.f + i) * 16.f, 29.f * 16.f}, {16.f, 16.f}});

    // create death sprite
    Sprite death(0.1);

    death.frames.push_back({{47.f * 16.f, 29.f * 16.f}, {16.f, 16.f}});

    // add sprites to sprite sheet
    this->spriteSheet.sprites["idle"] = idle;
    this->spriteSheet.sprites["walk"] = walk;
    this->spriteSheet.sprites["run"] = run;
    this->spriteSheet.sprites["death"] = death;

    this->spriteSheet.SetSprite("idle");
}

void Player::Update(float deltaTime) noexcept
{
    if (this->health > 0)
    {
        // check if sprinting
        if (backend->IsKeyDown(Backend::KeyCode::LSHIFT))
        {
            this->isRunning = true;
            this->moveSpeed = this->RUN_SPEED;
        }
        else
        {
            this->isRunning = false;
            this->moveSpeed = this->WALK_SPEED;
        }

        if (this->velocity.y != 0)
            this->isOnGround = false;

        // jump
        if (this->backend->IsKeyDown(Backend::KeyCode::SPACE))
        {
            if (this->isJumping)
            {
                if (this->jumpTicks < 4)
                {
                    this->jumpTicks++;

                    if (this->velocity.y < this->JUMP_VELOCITY)
                        this->velocity.y -= this->JUMP_ACCEL;
                }
                else
                {
                    this->isJumping = false;
                }
            }
            else
            {
                if (this->isOnGround)
                {
                    this->jumpTicks = 0;

                    this->isJumping = true;
                    this->isOnGround = false;
                }
            }
        }

        this->spriteSheet.SetSprite("idle");

        // process movement from input
        if (backend->IsKeyDown(Backend::KeyCode::LEFT) || backend->IsKeyDown(Backend::KeyCode::A))
        {
            this->directionX = -1;

            if (this->velocity.x > -this->moveSpeed)
                this->velocity.x -= this->ACCEL;

            if (this->isRunning)
                this->spriteSheet.SetSprite("run");
            else
                this->spriteSheet.SetSprite("walk");
        }

        if (backend->IsKeyDown(Backend::KeyCode::RIGHT) || backend->IsKeyDown(Backend::KeyCode::D))
        {
            this->directionX = 1;

            if (this->velocity.x < this->moveSpeed)
                this->velocity.x += this->ACCEL;

            if (this->isRunning)
                this->spriteSheet.SetSprite("run");
            else
                this->spriteSheet.SetSprite("walk");
        }

        this->velocity.x *= this->friction;

        if (std::abs(this->velocity.x) < 10)
            this->velocity.x = 0;

        this->friction = 1.f;
    }
    else
    {
        if (!this->dead)
        {
            this->dead = true;

            this->velocity.x = 0;
            this->velocity.y = -this->JUMP_VELOCITY;

            this->hasCollision = false;

            this->deathTimer.Reset();

            this->spriteSheet.SetSprite("death");
            this->spriteSheet.Lock();
        }
        else
        {
            if (this->deathTimer.Elapsed() > 3.f)
                this->health = -1;
        }
    }

    this->spriteSheet.NextFrame();
}

void Player::Render(const Rectangle &camera) noexcept
{
    Rectangle dest = {this->entityBox.pos - camera.pos, {48.f, 48.f}};

    dest.pos.x = dest.pos.x - 24.f + (this->entityBox.size.x / 2);
    dest.pos.y = dest.pos.y - (this->entityBox.size.y / 2);

    const Vector2<float> center = {0.f, 0.f};

    // render player
    this->backend->DrawTexture(this->spriteMap, *this->spriteSheet.GetFrame(), dest, 0.f, center,
                               this->directionX == -1 ? 1 : 0);
}

bool Player::OnCollide(Entity *entity, const Vector2<float> &contact) noexcept
{
    if (this->health > 0)
    {
        if (entity->health > 0)
        {
            // check y contact
            if (entity->damageable && contact.y == -1)
            {
                entity->health--;
                this->velocity.y = -this->JUMP_VELOCITY / 2;

                this->scoreKeeper->score += 100;
            }
            else
                this->health--;
        }
        else
        {
            switch (entity->type)
            {
            case EntityType::GoldItem: // pick up gold for score
            {
                this->scoreKeeper->gold++;
                this->scoreKeeper->score += 500;

                entity->health = -1;

                break;
            }
            case EntityType::CaveExit: {
                this->health = -9;
                break;
            }
            default:
                break;
            }
        }
    }

    return false;
}

bool Player::OnCollide(int, Tile &tile, const Vector2<float> &contact, const Rectangle &tile_pos) noexcept
{
    if (this->health > 0)
    {
        // check x contact
        if (contact.x != 0 && tile.type == Tile::Type::Platform)
            return false;

        // check y contact
        if (contact.y == 1) // vertical collision
        {
            if (tile.type == Tile::Type::Platform) // platform
                return false;
        }
        else if (contact.y == -1)
        {
            this->friction = tile.friction;

            if (this->isOnGround && tile.type == Tile::Type::Platform)
            {
                if (this->backend->IsKeyDown(Backend::KeyCode::DOWN) || this->backend->IsKeyDown(Backend::KeyCode::S))
                    return false;
            }

            this->isOnGround = true;
        }
    }

    return true;
}