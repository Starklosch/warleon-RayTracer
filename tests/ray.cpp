#include <Ray.hpp>
#include <gtest/gtest.h>

using namespace war;
TEST(RAY, Constructor) { Ray ray(point_t(0.0), vec_t(1.0)); }
