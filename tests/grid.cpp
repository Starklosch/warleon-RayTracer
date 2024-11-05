#include <Grid.hpp>
#include <Triangle.hpp>
#include <gtest/gtest.h>

using namespace war;

typedef Grid<Triangle *> grid_t;
const grid_t grid(point_t(1.0), point_t(10.0), {100, 100, 100});
const Ray ray(point_t(0.0), vec_t(1.0));
const Ray ray2(point_t(0.0), vec_t(1.0, 1.0, 10.0));
const Ray ray3(point_t(0.0), vec_t(1.0, 10.0, 1.0));

TEST(GRID_ITERATOR, begin_end) {
  grid.begin(ray);
  grid.end();
}

TEST(GRID_ITERATOR, begin_cell) {
  const auto it = grid.begin(ray);
  const auto it2 = grid.begin(ray2);

  ASSERT_NE(it, grid.end());
  ASSERT_NE(it2, grid.end());
  const auto it3 = grid.begin(ray3);
  const grid_t::index_t i = {0, 0, 0};
  const grid_t::index_t i2 = {0, 0, 99};
  const grid_t::index_t i3 = {0, 99, 0};
  EXPECT_EQ(it.getIndex(), i);
  EXPECT_EQ(it2.getIndex(), i2);
  EXPECT_EQ(&*it3, &grid[i3]);
}

TEST(GRID_ITERATOR, comparison) {
  EXPECT_FALSE(grid.begin(ray) == grid.end());
  EXPECT_TRUE(grid.begin(ray) != grid.end());
}

TEST(GRID_ITERATOR, operator_plus_plus) {
  auto it = grid.begin(ray);
  const auto i = it.getIndex();
  for (; it != grid.end(); ++it) {
    const auto i = it.getIndex();
  }
}
