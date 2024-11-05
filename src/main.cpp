#include <Camera.hpp>
#include <Canvas.hpp>
#include <Mesh.hpp>
#include <Renderer.hpp>
#include <Scene.hpp>
#include <iostream>

using namespace war;
int main() {
  Mesh::Loader load;
  std::string name("../data/bunny.obj");
  load.OBJ(name);
  Ray cameraRay(point_t(-0.02, 0.11, -0.5), vec_t(0.0, 0.0, 1.0));
  // Renderer renderer(Canvas(1024, 1024), Camera(cameraRay, 0.4, 0.15, 0.15,
  // 0.0),Scene());
  Renderer renderer(Canvas(32, 32), Camera(cameraRay, 0.4, 0.15, 0.15, 15.0),
                    Scene());
  renderer.scene.addMesh(load.getMesh());
  renderer.render();
  renderer.canvas.exportImage("render.png");

  return 0;
}
