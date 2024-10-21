#pragma once
#include <Color.hpp>
#include <common.hpp>
#include <string>
#include <vector>

namespace war {

class Canvas {
  std::vector<unsigned char> rgbaMatrix;

public:
  const size_t width, height;

  Canvas(size_t w, size_t h);
  void setPixel(size_t x, size_t y, const color_t &color);
  void exportImage(const std::string &filename) const;
  coord2d_t getNormalizedCoordinates(size_t x, size_t y) const;
};
}; // namespace war
