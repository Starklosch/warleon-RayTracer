#pragma once
#include <Mesh.hpp>

namespace war {
class Scene {
public:
  typedef Mesh::mesh_ptr mesh_ptr;
  typedef Mesh::triangle_ptr triangle_ptr;
  typedef void *light_ptr;
  typedef std::vector<mesh_ptr> mesh_set;
  typedef std::vector<light_ptr> light_set;

  Scene();
  void addMesh(mesh_ptr mesh) const;
  void addLight(light_ptr light) const;
  bool rayHit(const Ray &ray, vec_t &tuv, triangle_ptr &tri,
              mesh_ptr &mesh) const;

private:
  mutable mesh_set meshes;
  mutable light_set lights;
};
} // namespace war
