#pragma once
#include <Camera.hpp>
#include <Canvas.hpp>
#include <Scene.hpp>
namespace war {
class Renderer {
public:
  const Canvas canvas;
  const Camera camera;
  const Scene scene;
  Renderer(const Canvas &canvas, const Camera &camera);
  void render();
};
} // namespace war
