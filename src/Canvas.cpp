#include <Canvas.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace war {

Canvas::Canvas(size_t w, size_t h)
    : width(w), height(h), rgbaMatrix(w * h * 4, 0) {}

void Canvas::setPixel(size_t x, size_t y, const color_t &color) {
  const size_t i = 4 * (y * width + x);
  rgbaMatrix[i] = color.r;
  rgbaMatrix[i + 1] = color.g;
  rgbaMatrix[i + 2] = color.b;
  rgbaMatrix[i + 3] = color.a;
}
void Canvas::exportImage(const std::string &filename) const {
  stbi_write_png(filename.c_str(), width, height, 4, rgbaMatrix.data(),
                 width * 4);
}

coord2d_t Canvas::getNormalizedCoordinates(size_t x, size_t y) const {
  scalar_t hw = width / 2, hh = height / 2;
  return coord2d_t((x - hw) / hw, (y - hh) / hh);
}

}; // namespace war
