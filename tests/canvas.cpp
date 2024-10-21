#include <Canvas.hpp>
#include <gtest/gtest.h>

using namespace war;
TEST(CANVAS, exportImage) {
  Canvas canvas(255, 255);
  canvas.exportImage("render.png");
}
