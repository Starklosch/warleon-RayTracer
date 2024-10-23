#include <Grid.hpp>
#include <Triangle.hpp>
#include <iostream>

using namespace war;
typedef Grid<Triangle *> grid_t;
const grid_t grid(point_t(1.0), point_t(11.0), {100, 100, 100});
const Ray ray(point_t(0.0, 0.0, 10.0),
              vec_t(1.0, 1.0, 11.0) - point_t(0.0, 0.0, 10.0));
const Ray ray2(point_t(10.0), vec_t(1.0, 1.0, 10.0));
const Ray ray3(point_t(20.0), vec_t(11.0, 11.0, 11.0) - point_t(20.0));
int main() {
  const auto it = grid.begin(ray);
  const auto it2 = grid.begin(ray2);
  const auto it3 = grid.begin(ray3);

  const grid_t::index_t i = it.getIndex();
  const grid_t::index_t i2 = it2.getIndex();
  const grid_t::index_t i3 = it3.getIndex();

  std::cout << "{" << i[0] << ", " << i[1] << ", " << i[2] << "}" << std::endl;
  std::cout << "{" << i2[0] << ", " << i2[1] << ", " << i2[2] << "}"
            << std::endl;
  std::cout << "{" << i3[0] << ", " << i3[1] << ", " << i3[2] << "}"
            << std::endl;

  return 0;
}
