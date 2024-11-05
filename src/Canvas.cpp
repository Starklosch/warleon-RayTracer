#include <Canvas.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace war {

Canvas::Canvas(size_t w, size_t h)
    : width(w), height(h), rgbaMatrix(w * h * 4, 0) {}

void Canvas::setPixel(size_t x, size_t y, const color_t &color) const {
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
  return coord2d_t((x - hw) / hw, (hh - y) / hh);
}

// Bresenham's line algorithm for drawing a line between 2D points
void Canvas::drawLine(size_t x0, size_t y0, size_t x1, size_t y1,
                      const color_t &color) const {
  int dx = glm::abs(x1 - x0), dy = glm::abs(y1 - y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;

  while (true) {
    setPixel(x0, y0, color);
    if (x0 == x1 && y0 == y1)
      break;
    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}
}; // namespace war
