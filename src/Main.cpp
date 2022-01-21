/**
 * @author undersquire
 * @date 11/20/2021
 *
 * A simple and fun 2D platformer game.
 */
#include "Level.h"
#include "SDL.h"
#include "Stage.h"
#include "Timer.h"
#include "Titlescreen.h"

/**
 * Pre-loads all assets for the game into memory
 *
 * @param backend A reference to the backend object
 */
bool loadAssets(Backend &backend);

/**
 * Entry point + Game Loop
 */
int main(int, char **)
{
    // Backend for Graphics, Input, Audio, etc
    Backend backend("Gold Rush", 853, 480);

    // try loading assets
    if (!loadAssets(backend))
        return -1;

    // Stage for handling scenes
    Stage stage;

    // activate our main level scene
    stage.SetScene(new Titlescreen(&backend));

    // Timer for frame time counting
    Timer timer;

    while (backend.IsRunning())
    {
        // acquire delta time from timer
        float deltaTime = timer.Elapsed();

        if (deltaTime > 0.017)
            deltaTime = 0.017;

        // poll operating system events
        backend.UpdateEverything();

        // update the current scene
        if (!stage.UpdateScene(deltaTime))
            continue;

        // reset timer
        timer.Reset();

        // render the current scene
        stage.RenderScene();

        // present all graphics to screen
        backend.Present();
    }

    return 0;
}

bool loadAssets(Backend &backend)
{
    std::string path = backend.GetBasePath();

    // sprites & tileset
    if (!backend.GetAssetManager().LoadAsset((path + "res/sprites.png").c_str(), "sprites", AssetType::Texture))
        return false;

    // obtain the raw texture
    Texture *raw_sprites = (Texture *)backend.GetAssetManager().GetAsset("sprites");

    // create the backend texture
    auto sprites = backend.InitTexture(raw_sprites->GetTextureData());

    // cache the texture for future use
    backend.AddCachedTexture(sprites, "sprites");

    // font sheet
    if (!backend.GetAssetManager().LoadAsset((path + "res/font.png").c_str(), "font", AssetType::Texture))
        return false;

    // obtain the raw texture
    Texture *raw_font = (Texture *)backend.GetAssetManager().GetAsset("font");

    // create the backend texture
    auto font = backend.InitTexture(raw_font->GetTextureData());

    // cache the texture for future use
    backend.AddCachedTexture(font, "font");

    // background
    if (!backend.GetAssetManager().LoadAsset((path + "res/background.png").c_str(), "background", AssetType::Texture))
        return false;

    // obtain the raw texture
    Texture *raw_background = (Texture *)backend.GetAssetManager().GetAsset("background");

    // create the backend texture
    auto background = backend.InitTexture(raw_background->GetTextureData());

    // cache the texture for future use
    backend.AddCachedTexture(background, "background");

    return true;
}