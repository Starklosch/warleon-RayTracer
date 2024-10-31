#pragma once
#include <Camera.hpp>
#include <Canvas.hpp>
#include <Scene.hpp>
namespace war {
class Renderer {
public:
  typedef Scene::mesh_ptr mesh_ptr;
  typedef Scene::triangle_ptr triangle_ptr;

  const Canvas canvas;
  const Camera camera;
  const Scene scene;
  Renderer(const Canvas &&canvas, const Camera &&camera, const Scene &&scene);
  void render() const;
};
} // namespace war
