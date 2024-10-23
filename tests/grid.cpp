#include <Grid.hpp>
#include <Triangle.hpp>
#include <gtest/gtest.h>

using namespace war;

typedef Grid<Triangle *> grid_t;
const Ray ray(point_t(0.0), vec_t(1.0));
const grid_t grid(point_t(1.0), point_t(10.0), {100, 100, 100});
TEST(GRID_ITERATOR, begin_end) {
  grid.begin(ray);
  grid.end();
}
TEST(GRID_ITERATOR, begin_cell) {
	const auto it = grid.begin(ray);
	const grid_t::index_t i = {0,0,0};
	ASSERT_EQ(*it, grid[i]);
}
TEST(GRID_ITERATOR, comparison) {
  Grid<Triangle *> grid(point_t(0.0), point_t(10.0), {100, 100, 100});
  EXPECT_FALSE(grid.begin(ray) == grid.end());
  EXPECT_TRUE(grid.begin(ray) != grid.end());
}
