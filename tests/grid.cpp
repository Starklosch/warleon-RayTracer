#include <gtest/gtest.h>
#include <Triangle.hpp>
#include <Grid.hpp>

using namespace war;
TEST(GRID,constructor){
	Grid<Triangle*> grid(point_t(0.0),point_t(10.0),{100,100,100});
}
