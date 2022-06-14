#include "Level.h"

#include <cmath>

#include "LevelStats.h"
#include "Titlescreen.h"

Level::Level(Backend *backend) noexcept : Scene(backend)
{
    this->fontMap = this->backend->GetCachedTexture("font");
    this->spriteMap = this->backend->GetCachedTexture("sprites");
    this->background = this->backend->GetCachedTexture("background");

    this->backend->GetWindowSize(this->windowSize);

    this->map.tileSize = 32.f;

    std::vector<std::string> map;

    // this is our hard-coded level editor :)
    // please ignore its formatting, my code formatter messed it up
    // see Map.h for a list of tile types and their corresponding character id
    // value for use in this editor \/
    map.push_back("666666666666666666666666666666666666666666666666666666666666666666666666"
                  "666666666666666666666666666666666666666666666666666666666666666666666666"
                  "666666666666666666666666666");
    map.push_back("1......................................................................."
                  "........................................................................"
                  "..........................8");
    map.push_back("1..tttt................................................................."
                  "........................................................................"
                  "..........................8");
    map.push_back("1......................................................................."
                  "........................................................................"
                  "..........................8");
    map.push_back("12333333334............................................................."
                  "........................................................................"
                  "..........................8");
    map.push_back("15666666667.................................ttttt......................."
                  "........................................................................"
                  "..........................8");
    map.push_back("1..............s........................s...........s...............ttt."
                  "........................................................................"
                  "..........................8");
    map.push_back("1.............234......................233334...233334.................."
                  "..................................................ttt..................."
                  "..........................8");
    map.push_back("1.............567......................566667...566667...........s......"
                  ".b.....................................g........................tt......"
                  "..........................8");
    map.push_back("1.........................g..................ttt..............."
                  "233333333334..............................................23334........."
                  "...................................8");
    map.push_back("1...................2333333334...............ttt...........z..."
                  "566666666667....s.........................................56667........."
                  ".BC.....tt.........................8");
    map.push_back("1...................5666666667............................BC............"
                  "......BC....................................s...................FG......"
                  "..........................8");
    map.push_back("1.......................tt................................DE............"
                  "......DE.................................23334.................zDE....."
                  "BC.........................8");
    map.push_back("1......tt........BC.....tt............................BC..FG.........g.."
                  "......FG..BC........................s....56667....tttt.....2333334....."
                  "FG.........................8");
    map.push_back("1............t...DE.....tt............................DE..DE..tt........"
                  "..tt..DE..DE...s.................23334................z....5666667...."
                  "sDE......ttt................8");
    map.push_back("1......24.....s..FG..................b............BC..FG..FG............"
                  "......FG..FG..BC.................56667..........23333334.............."
                  "23334....................e..8");
    map.push_back("1p.....8A..23334.DE............................z..DE.zDE..DE............"
                  "......DE..DE..DE...z....z.......................56666667....g........."
                  "56667...23334...............8");
    map.push_back("33333339A..8999933334........23333333333333333333333333333334..........."
                  ".....23333333333333333333333334........................................."
                  "......56667.....23333333333");

    // add player
    this->entities.push_back(new Player(backend, &this->scoreKeeper));

    // convert map to a format that the game can process
    for (int x = 0; x < map[0].size(); x++)
    {
        if (x >= this->map.tiles.size())
            this->map.tiles.push_back({});

        for (int y = 0; y < map.size(); y++)
        {
            switch (map[y][x])
            {
            case '.': {
                this->map.tiles[x].push_back(TILE_TABLE[0]);
                break;
            }
            case 's': {
                this->map.tiles[x].push_back(TILE_TABLE[0]);

                Vector2<float> spawn = {x * this->map.tileSize, y * this->map.tileSize};
                this->entities.push_back(new Skeleton(this->backend, spawn));

                break;
            }
            case 'b': {
                this->map.tiles[x].push_back(TILE_TABLE[0]);

                Vector2<float> spawn = {x * this->map.tileSize, y * this->map.tileSize};
                this->entities.push_back(new Bat(this->backend, spawn));

                break;
            }
            case 'g': {
                this->map.tiles[x].push_back(TILE_TABLE[0]);

                Vector2<float> spawn = {x * this->map.tileSize, y * this->map.tileSize};
                this->entities.push_back(new Ghost(this->backend, spawn));

                break;
            }
            case 'z': {
                this->map.tiles[x].push_back(TILE_TABLE[0]);

                Vector2<float> spawn = {x * this->map.tileSize, y * this->map.tileSize};
                this->entities.push_back(new Zombie(this->backend, spawn));

                break;
            }
            case 'p': {
                this->map.tiles[x].push_back(TILE_TABLE[0]);

                Vector2<float> spawn = {x * this->map.tileSize, y * this->map.tileSize};
                this->entities[0]->entityBox.pos = spawn;

                break;
            }
            case 't': {
                this->map.tiles[x].push_back(TILE_TABLE[0]);

                Vector2<float> spawn = {x * this->map.tileSize, y * this->map.tileSize};
                this->entities.push_back(new Gold(this->backend, spawn));

                break;
            }
            case 'e': {
                this->map.tiles[x].push_back(TILE_TABLE[0]);

                Vector2<float> spawn = {x * this->map.tileSize, y * this->map.tileSize};
                this->entities.push_back(new CaveExit(this->backend, spawn));

                break;
            }
            default:
                int tile = map[y][x];

                if (tile >= 48 && tile <= 57)
                    tile -= 48;
                else if (tile >= 65 && tile <= 90)
                    tile -= 55;

                this->map.tiles[x].push_back(TILE_TABLE[tile]);

                break;
            }
        }
    }

    // setup camera view
    this->camera.pos = {0, 0};
    this->camera.size = {853, 480};
}

Level::~Level()
{
    // free the memory used by all of the entities
    for (auto entity : this->entities)
        delete entity;

    // clear the vector (not necessary but just in case)
    this->entities.clear();
}

bool ResolveCollision(Rectangle &r_dynamic, Vector2<float> &velocity, const float deltaTime,
                      Rectangle &r_static) noexcept
{
    Vector2<float> contact_point, contact_normal;
    float contact_time = 0.f;

    if (r_static.Intersects(r_dynamic, velocity, deltaTime, contact_point, contact_normal, contact_time))
    {
        velocity += contact_normal * Vector2<float>(std::abs(velocity.x), std::abs(velocity.y)) * (1 - contact_time);
        return true;
    }

    return false;
}

bool ResolveDynamicCollision(Rectangle &r_dynamic, Vector2<float> &velocity, const float deltaTime, Rectangle &r_static,
                             Vector2<float> &dyn_velocity) noexcept
{
    Vector2<float> contact_point, contact_normal;
    float contact_time = 0.f;

    if (r_static.Intersects(dyn_velocity, r_dynamic, velocity, deltaTime, contact_point, contact_normal, contact_time))
    {
        velocity += contact_normal * Vector2<float>(std::abs(velocity.x), std::abs(velocity.y)) * (1 - contact_time);
        return true;
    }

    return false;
}

void Level::Update(float deltaTime) noexcept
{
    // update paused state

    // if the ESCAPE key is pressed, inverse the paused state
    if (this->backend->IsKeyDown(Backend::KeyCode::RETURN))
    {
        if (!this->pauseBlock)
        {
            this->paused = !this->paused;
            this->pauseBlock = true;
        }
    }
    else
        this->pauseBlock = false;

    // dont update if we are paused
    if (this->paused)
        return;

    // update all entities
    std::vector<Entity *> dead;

    for (int i = 0; i < entities.size(); i++)
    {
        auto &entity = entities[i];

        if (entity->health < 0)
        {
            if (i == 0)
            {
                if (entity->health == -9)
                    this->transition = new LevelStats(this->backend, this->scoreKeeper);
                else
                    this->transition = new Titlescreen(this->backend);
                break;
            }

            dead.push_back(entity);
            continue;
        }

        if (entity->entityBox.pos.x < -entity->entityBox.size.x ||
            entity->entityBox.pos.x > this->map.tiles.size() * this->map.tileSize ||
            entity->entityBox.pos.y > this->map.tiles[0].size() * this->map.tileSize)
        {
            if (i == 0)
                entity->health = 0;
            else
            {
                dead.push_back(entity);
                continue;
            }
        }

        if (entity->boundsReset)
        {
            if (entity->entityBox.pos.x < this->camera.pos.x - (this->camera.size.x / 3) ||
                entity->entityBox.pos.x > this->camera.pos.x + this->camera.size.x + (this->camera.size.x / 3) ||
                entity->entityBox.pos.y < this->camera.pos.y - (this->camera.size.y / 3) ||
                entity->entityBox.pos.y > this->camera.pos.y + this->camera.size.y + (this->camera.size.y / 3))
                continue;
        }

        entity->Update(deltaTime);

        // apply gravity
        if (entity->hasGravity && entity->velocity.y < this->GRAVITY_MAX)
            entity->velocity.y += this->GRAVITY_ACCEL;

        if (entity->hasCollision)
        {
            const Rectangle next = {entity->entityBox.pos + (entity->velocity * deltaTime), entity->entityBox.size};

            int startX, startY, endX, endY;

            if (next.pos.x >= entity->entityBox.pos.x)
            {
                startX = static_cast<int>(entity->entityBox.pos.x) / this->map.tileSize;
                endX = static_cast<int>(next.pos.x + next.size.x) / this->map.tileSize;
            }
            else
            {
                endX = static_cast<int>(entity->entityBox.pos.x + entity->entityBox.size.x) / this->map.tileSize;
                startX = static_cast<int>(next.pos.x) / this->map.tileSize;
            }

            if (next.pos.y >= entity->entityBox.pos.y)
            {
                startY = static_cast<int>(entity->entityBox.pos.y) / this->map.tileSize;
                endY = static_cast<int>(next.pos.y + next.size.y) / this->map.tileSize;
            }
            else
            {
                endY = static_cast<int>(entity->entityBox.pos.y + entity->entityBox.size.y) / this->map.tileSize;
                startY = static_cast<int>(next.pos.y) / this->map.tileSize;
            }

            startX = std::max(startX, 0);
            startX = std::min(startX, static_cast<int>(this->map.tiles.size() - 1));

            endX = std::max(endX, 0);
            endX = std::min(endX, static_cast<int>(this->map.tiles.size() - 1));

            startY = std::max(startY, 0);
            startY = std::min(startY, static_cast<int>(this->map.tiles[0].size() - 1));

            endY = std::max(endY, 0);
            endY = std::min(endY, static_cast<int>(this->map.tiles[0].size() - 1));

            Vector2<float> cp, cn;
            float ct = 0.f;

            std::vector<std::tuple<float, Rectangle, Tile, Vector2<float>>> collisions;

            for (int x = startX; x <= endX; x++)
            {
                for (int y = startY; y <= endY; y++)
                {
                    const Rectangle rect = {Vector2<float>(x * this->map.tileSize, y * this->map.tileSize),
                                            Vector2<float>(this->map.tileSize, this->map.tileSize)};

                    if (this->map.tiles[x][y].type != Tile::Type::Passable &&
                        rect.Intersects(entity->entityBox, entity->velocity, deltaTime, cp, cn, ct))
                        collisions.push_back({ct, rect, this->map.tiles[x][y], cn});
                }
            }

            // sort collisions
            std::sort(collisions.begin(), collisions.end(),
                      [](const std::tuple<float, Rectangle, Tile, Vector2<float>> &a,
                         const std::tuple<float, Rectangle, Tile, Vector2<float>> &b) {
                          return std::get<0>(a) < std::get<0>(b);
                      });

            // resolve each collision
            for (int i = 0; i < collisions.size(); i++)
            {
                auto &c = collisions[i];

                // resolve collision
                if (entity->OnCollide(i, std::get<2>(c), std::get<3>(c), std::get<1>(c)))
                    ResolveCollision(entity->entityBox, entity->velocity, deltaTime, std::get<1>(c));
            }
        }

        if (entity->hasEntityCollision)
        {
            Vector2<float> cp, cn;
            float ct = 0.f;

            std::vector<std::tuple<float, int, Vector2<float>>> collisions;

            for (int j = 0; j < this->entities.size(); j++)
            {
                auto &e = this->entities[j];

                if (e != entity)
                {
                    if (e->entityBox.Intersects(e->velocity, entity->entityBox, entity->velocity, deltaTime, cp, cn,
                                                ct))
                        collisions.push_back({ct, j, cn});
                }
            }

            // sort collisions
            std::sort(collisions.begin(), collisions.end(),
                      [](const std::tuple<float, int, Vector2<float>> &a,
                         const std::tuple<float, int, Vector2<float>> &b) { return std::get<0>(a) < std::get<0>(b); });

            for (auto &c : collisions)
            {
                // resolve collision
                if (entity->OnCollide(entities[std::get<1>(c)], std::get<2>(c)))
                    ResolveDynamicCollision(entity->entityBox, entity->velocity, deltaTime,
                                            entities[std::get<1>(c)]->entityBox, entities[std::get<1>(c)]->velocity);
            }
        }

        // apply velocity
        entity->entityBox.pos += entity->velocity * deltaTime;

        if (entity->hasTarget)
            entity->target = this->entities[0];

        if (i == 0 && entity->health == 0)
            break;
    }

    for (auto entity : dead)
    {
        auto pos = std::find(this->entities.begin(), this->entities.end(), entity);

        if (pos != this->entities.end())
            this->entities.erase(pos);

        delete entity;
    }

    // update camera
    if (this->entities.size() > 0)
    {
        this->camera.pos =
            this->entities[0]->entityBox.pos + (this->entities[0]->entityBox.size / 2) - (this->camera.size / 2);

        if (this->camera.pos.x < 0)
            this->camera.pos.x = 0;

        if (this->camera.pos.x + this->camera.size.x > this->map.tiles.size() * this->map.tileSize)
            this->camera.pos.x = this->map.tiles.size() * this->map.tileSize - this->camera.size.x;

        if (this->camera.pos.y < 0)
            this->camera.pos.y = 0;

        if (this->camera.pos.y + this->camera.size.y > this->map.tiles[0].size() * this->map.tileSize)
            this->camera.pos.y = this->map.tiles[0].size() * this->map.tileSize - this->camera.size.y;

        this->camera.pos.x = static_cast<int>(this->camera.pos.x);
        this->camera.pos.y = static_cast<int>(this->camera.pos.y);
    }
}

void Level::Render() noexcept
{
    // clear screen
    this->backend->ClearDisplay(this->clearColor);

    // render background
    const Rectangle dest = {{0.f, 0.f},
                            {static_cast<float>(this->windowSize.x), static_cast<float>(this->windowSize.y)}};

    this->backend->DrawTexture(this->background, dest, dest);

    // render tiles
    int startX = this->camera.pos.x / this->map.tileSize;
    int startY = this->camera.pos.y / this->map.tileSize;

    int endX = (this->camera.pos.x + this->camera.size.x) / this->map.tileSize;
    int endY = (this->camera.pos.y + this->camera.size.y) / this->map.tileSize;

    if (startX < 0)
        startX = 0;

    if (startY < 0)
        startY = 0;

    if (endX >= map.tiles.size())
        endX = map.tiles.size() - 1;

    if (endY >= map.tiles[endX].size())
        endY = map.tiles[endX].size() - 1;

    for (int x = startX - 1; x <= endX; x++)
    {
        if (x < 0)
            continue;

        for (int y = startY - 1; y <= endY; y++)
        {
            if (y < 0)
                continue;

            Rectangle rect = {{x * this->map.tileSize, y * this->map.tileSize},
                              {this->map.tileSize, this->map.tileSize}};

            rect.pos -= this->camera.pos;

            this->backend->DrawTexture(this->spriteMap, this->map.tiles[x][y].sprite, rect);
        }
    }

    // render all entities
    for (auto entity : this->entities)
    {
        if (entity->entityBox.pos.x >= this->camera.pos.x - entity->entityBox.size.x &&
            entity->entityBox.pos.x <= this->camera.pos.x + this->camera.size.x + entity->entityBox.size.x &&
            entity->entityBox.pos.y >= this->camera.pos.y - entity->entityBox.size.y &&
            entity->entityBox.pos.y <= this->camera.pos.y + this->camera.size.y + entity->entityBox.size.y)
            entity->Render(this->camera);

        // uncomment the line below if you want to show entity hitboxes
        // this->backend->DrawRectangle({entity->entityBox.pos - this->camera.pos,
        // entity->entityBox.size}, this->HITBOX_COLOR);
    }

    // window width
    float width = this->windowSize.x;

    if (this->paused)
    {
        const Rectangle title = {{width / 8, 80.f}, {width - (width / 4), 80.f}};

        this->RenderText("PAUSED", title);
    }

    // render ingame stats
    std::string gold_string = std::to_string(this->scoreKeeper.gold);
    std::string score_string = std::to_string(this->scoreKeeper.score);

    // render gold stats
    const Rectangle gold_icon = {{10.f, 8.f}, {42.f, 42.f}};
    const Rectangle gold_text = {{70.f, 20.f}, {24.f * ceilf(gold_string.length() + 2), 24.f}};

    this->RenderText(std::string("X`") + gold_string, gold_text);

    const Rectangle source = {{18.f * 32.f, 20.5f * 32.f}, {32.f, 32.f}};

    this->backend->DrawTexture(this->spriteMap, source, gold_icon);

    // render score stats
    const Rectangle score_text = {{width - 200.f, 10.f}, {12.f * ceilf(score_string.length() + 8), 24.f}};

    this->RenderText(std::string("SCORE:`") + score_string, score_text);
}

void Level::RenderText(std::string_view text, const Rectangle &destination) noexcept
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
