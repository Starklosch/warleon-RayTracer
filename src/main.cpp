#include <Grid.hpp>
#include <Triangle.hpp>
#include <iostream>

using namespace war;
typedef Grid<Triangle *> grid_t;
const grid_t grid(point_t(1.0), point_t(11.0), {100, 100, 100});
const Ray ray(point_t(0.0, 0.0, 0.0), vec_t(1.0, 1.0, 1.0));
int main() {
  auto it = grid.begin(ray);
  auto i = grid.end().getIndex();
  std::cout << "iterator = " << &it << " - grid = " << grid.end().grid << " - {"
            << i[0] << ", " << i[1] << ", " << i[2] << "}" << std::endl;
  size_t j = 0;
  for (; it != grid.end(); ++it) {

    i = it.getIndex();
    std::cout << "iterator = " << &it << " - grid = " << it.grid << " - {"
              << i[0] << ", " << i[1] << ", " << i[2] << "}" << std::endl;
    j++;
  }
  std::cout << j;

  return 0;
}
