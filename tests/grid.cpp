#include <Grid.hpp>
#include <Triangle.hpp>
#include <gtest/gtest.h>

using namespace war;

typedef Grid<Triangle *> grid_t;
const grid_t grid(point_t(1.0), point_t(10.0), {100, 100, 100});
const Ray ray(point_t(0.0), vec_t(1.0));
const Ray ray2(point_t(0.0), vec_t(1.0, 1.0, 10.0));
const Ray ray3(point_t(0.0), vec_t(1.0, 10.0, 1.0));
// const Ray ray4(point_t(0.0), vec_t(10.0, 1.0, 1.0));
// const Ray ray5(point_t(0.0), vec_t(10.0, 10.0, 1.0));
// const Ray ray6(point_t(0.0), vec_t(1.0, 10.0, 10.0));
// const Ray ray7(point_t(0.0), vec_t(10.0, 1.0, 10.0));
// const Ray ray8(point_t(0.0), vec_t(10.0, 10.0, 10.0));
TEST(GRID_ITERATOR, begin_end) {
  grid.begin(ray);
  grid.end();
}
TEST(GRID_ITERATOR, begin_cell) {
  // testing::internal::CaptureStdout();
  const auto it = grid.begin(ray);
  const auto it2 = grid.begin(ray2);

  ASSERT_NE(it, grid.end());
  ASSERT_NE(it2, grid.end());
  const auto it3 = grid.begin(ray3);
  //	const auto it4 = grid.begin(ray4);
  //	const auto it5 = grid.begin(ray5);
  //	const auto it6 = grid.begin(ray6);
  //	const auto it7 = grid.begin(ray7);
  //	const auto it8 = grid.begin(ray8);
  const grid_t::index_t i = {0, 0, 0};
  const grid_t::index_t i2 = {0, 0, 99};
  const grid_t::index_t i3 = {0, 100, 0};
  //	const grid_t::index_t i4 = {100,0,0};
  //	const grid_t::index_t i5 = {100,100,0};
  //	const grid_t::index_t i6 = {1,100,100};
  //	const grid_t::index_t i7 = {100,0,100};
  //	const grid_t::index_t i8 = {100,100,100};
  EXPECT_EQ(it.getIndex(), i);
  EXPECT_EQ(it2.getIndex(), i2);
  // SUCCEED() << testing::internal::GetCapturedStdout();
  EXPECT_EQ(&*it3, &grid[i3]);
  //	ASSERT_EQ(&*it4, &grid[i4]);
  //	ASSERT_EQ(&*it5, &grid[i5]);
  //	ASSERT_EQ(&*it6, &grid[i6]);
  //	ASSERT_EQ(&*it7, &grid[i7]);
  //	ASSERT_NE(&*it8, &grid[i8]);
  //	ASSERT_EQ(&*it8, &grid[i]);
}
TEST(GRID_ITERATOR, comparison) {
  Grid<Triangle *> grid(point_t(0.0), point_t(10.0), {100, 100, 100});
  EXPECT_FALSE(grid.begin(ray) == grid.end());
  EXPECT_TRUE(grid.begin(ray) != grid.end());
}
