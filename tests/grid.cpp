#include <Grid.hpp>
#include <Triangle.hpp>
#include <gtest/gtest.h>

using namespace war;

const Ray ray(point_t(0.0), vec_t(1.0));
TEST(GRID, constructor) {
  Grid<Triangle *> grid(point_t(0.0), point_t(10.0), {100, 100, 100});
}
TEST(GRID_ITERATOR, begin_end) {
  Grid<Triangle *> grid(point_t(0.0), point_t(10.0), {100, 100, 100});
  grid.begin(ray);
  grid.end();
}
TEST(GRID_ITERATOR, comparison) {
  Grid<Triangle *> grid(point_t(0.0), point_t(10.0), {100, 100, 100});
  EXPECT_FALSE(grid.begin(ray) == grid.end());
  EXPECT_TRUE(grid.begin(ray) != grid.end());
}
