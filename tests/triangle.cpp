#include <Triangle.hpp>
#include <glm/gtc/random.hpp>
#include <gtest/gtest.h>
using namespace war;
using namespace glm;
TEST(TRIANGLE, Constructor) {
  Triangle tri(point_t(1.0), point_t(2.0), point_t(3.0));
}
TEST(TRIANGLE, intersection) {
  const auto A = linearRand(dvec3(-10.0), dvec3(10.0)),
             B = linearRand(dvec3(-10.0), dvec3(10.0)),
             C = linearRand(dvec3(-10.0), dvec3(10.0));
  const auto uv = linearRand(dvec2(0.0), dvec2(0.5));
  const auto uv2 = linearRand(dvec2(6.0), dvec2(2.5));
  Triangle tri(A, B, C);
  Ray hit(point_t(0.0), mat3(A, B, C) * vec_t(1 - uv.x - uv.y, uv));
  Ray noHit(point_t(0.0), mat3(A, B, C) * vec_t(1.0 - uv2.x - uv2.y, uv2));

  vec_t tuv;
  EXPECT_TRUE(tri.rayHit(hit, tuv)) << "failed to hit";
  EXPECT_FALSE(tri.rayHit(noHit, tuv)) << "failed to not hit";
}
