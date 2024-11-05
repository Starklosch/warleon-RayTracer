#pragma once
#include <Color.hpp>
#include <common.hpp>
#include <string>
#include <vector>

namespace war {

class Canvas {

public:
  const size_t width, height;

  Canvas(size_t w, size_t h);
  void setPixel(size_t x, size_t y, const color_t &color) const;
  void drawLine(size_t x0, size_t y0, size_t x1, size_t y1,
                const color_t &color) const;
  void exportImage(const std::string &filename) const;
  coord2d_t getNormalizedCoordinates(size_t x, size_t y) const;

private:
  mutable std::vector<unsigned char> rgbaMatrix;
};
}; // namespace war
