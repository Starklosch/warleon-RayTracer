#include <Renderer.hpp>
namespace war {
Renderer::Renderer(const Canvas &&canvas, const Camera &&camera,
                   const Scene &&scene)
    : canvas(canvas), camera(camera), scene(scene) {}
void Renderer::render() const {
  for (size_t i = 0; i < canvas.width; ++i) {
    for (size_t j = 0; j < canvas.height; ++j) {
      const coord2d_t coord = canvas.getNormalizedCoordinates(i, j);
      const Ray ray = camera.getRayFromViewportCoordinates(coord.x, coord.y);
      vec_t tuv;
      triangle_ptr tri;
      mesh_ptr mesh;
      if (scene.rayHit(ray, tuv, tri, mesh)) {
        canvas.setPixel(i, j, color_t(255, 255, 255));
      }
    }
  }
}
} // namespace war
