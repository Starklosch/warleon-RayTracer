#include <gtest/gtest.h>
#include <Camera.hpp>

using namespace war;

TEST(CAMERA, Constructor) {
	Camera cam(
			Ray(point_t(0.0),vec_t(1.0)),
			1.0,
			1.0,
			1.0,
			15.0
			);
}
