#pragma once
#include <Grid.hpp>
#include <Triangle.hpp>
#include <common.hpp>
#include <memory>
#include <string>
#include <vector>

namespace war {
class Mesh {
public:
  typedef std::shared_ptr<Triangle> triangle_ptr;
  typedef Grid<triangle_ptr> grid_t;
  typedef grid_t::aabb_t aabb_t;
  typedef grid_t::index_t index_t;
  typedef std::shared_ptr<grid_t> grid_ptr;
  typedef std::vector<triangle_ptr> triangle_set;

  void voxelize(triangle_ptr tri);
  bool aabbTriangleHit(const aabb_t &mbb, const Triangle &tri);

public:
  Mesh();
  const grid_ptr getGrid() const;

  class Loader {
    std::shared_ptr<Mesh> mesh;

  public:
    Loader();
    bool OBJ(const std::string &filename);
    const std::shared_ptr<Mesh> getMesh() const;
  };

private:
  grid_ptr grid;
  triangle_set triangles;
};
} // namespace war
