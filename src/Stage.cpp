#include "Stage.h"

Stage::~Stage() {
  if (this->currentScene != nullptr)
    delete this->currentScene;
}

Scene *Stage::GetScene() { return this->currentScene; }

void Stage::SetScene(Scene *scene) {
  if (this->currentScene != nullptr)
    delete this->currentScene;

  this->currentScene = scene;
}

bool Stage::UpdateScene(float deltaTime) {
  if (this->currentScene != nullptr) {
    this->currentScene->Update(deltaTime);

    if (this->currentScene->transition != nullptr) {
      this->SetScene(this->currentScene->transition);
      return false;
    } else
      return true;
  }

  return false;
}

void Stage::RenderScene() {
  if (this->currentScene != nullptr)
    this->currentScene->Render();
}
