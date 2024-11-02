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
  typedef grid_t::index_t index_t;
  typedef std::shared_ptr<grid_t> grid_ptr;
  typedef std::vector<triangle_ptr> triangle_set;
  typedef std::shared_ptr<Mesh> mesh_ptr;

  void voxelize(triangle_ptr tri);
  bool aabbTriangleHit(const aabb_t &mbb, const Triangle &tri) const;

public:
  Mesh();
  const grid_ptr getGrid() const;

  class Loader {
    mesh_ptr mesh;

  public:
    Loader();
    bool OBJ(const std::string &filename);
    const mesh_ptr getMesh() const;
  };
  bool rayHit(const Ray &ray, vec_t &tuv, triangle_ptr &tri) const;

private:
  grid_ptr grid;
  triangle_set triangles;
};
} // namespace war
