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
  typedef grid_t::aabb_t aabb_t;
  typedef grid_t::index_t index_t;
  std::unique_ptr<grid_t> grid;
  std::vector<Triangle> triangles;

  void voxelize(triangle_ptr tri);
  bool aabbTriangleHit(const aabb_t &mbb, const Triangle &tri);

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
