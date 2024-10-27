#include <Camera.hpp>
#include <Canvas.hpp>
#include <Mesh.hpp>
#include <iostream>

using namespace war;
int main() {
  Mesh::Loader load;
  std::string name("../data/bunny.obj");
  load.OBJ(name);
  const auto mesh = load.getMesh();
  Canvas canvas(512, 512);
  Ray cameraRay(point_t(-0.02, 0.11, -0.5), vec_t(0.0, 0.0, 1.0));
  Camera camera(cameraRay, 0.4, 0.15, 0.15, 0.0);

  return 0;
}
