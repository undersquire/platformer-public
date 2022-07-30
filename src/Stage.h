#pragma once

#include <vector>

#include "Backend.h"
#include "Scene.h"

class Stage {
public:
  ~Stage();

  Scene *GetScene();
  void SetScene(Scene *);

  bool UpdateScene(float);
  void RenderScene();

private:
  Scene *currentScene = nullptr;
};
