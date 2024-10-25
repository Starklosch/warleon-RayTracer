#pragma once
#include <Grid.hpp>
#include <Triangle.hpp>
#include <common.hpp>
#include <memory>
#include <string>
#include <vector>

namespace war {
class Mesh {
  typedef std::shared_ptr<Triangle> triangle_ptr;
  typedef Grid<triangle_ptr> grid_t;
  std::unique_ptr<grid_t> grid;
  std::vector<Triangle> triangles;

  void voxelize(const Triangle &tri);

public:
  Mesh();

  class Loader {
    std::shared_ptr<Mesh> mesh;

  public:
    Loader();
    bool OBJ(const std::string &filename);
    const std::shared_ptr<Mesh> getMesh() const;
  };
};
} // namespace war
