#include <gtest/gtest.h>
#include <Canvas.hpp>

using namespace war;
TEST(CANVAS,exportImage){
	Canvas canvas(255,255);
	canvas.exportImage("render.png");
}
