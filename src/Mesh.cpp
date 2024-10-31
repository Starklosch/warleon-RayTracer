#include <Mesh.hpp>
#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif
#include <tiny_obj_loader.h>

namespace war {
Mesh::Mesh() : grid(nullptr), triangles() {}

bool Mesh::aabbTriangleHit(const aabb_t &mbb, const Triangle &tri) {
  const vec_t &e = grid->boxSize;
  const point_t center = mbb.min + e / 2.0;
  const point_t A = tri.A, B = tri.A + tri.AB, C = tri.A + tri.AC;

  const Triangle ttri(A - center, B - center, C - center);

  const vec_t X(1.0, 0.0, 0.0);
  const vec_t Y(0.0, 1.0, 0.0);
  const vec_t Z(0.0, 0.0, 1.0);
  const point_t tA = ttri.A, tB = ttri.A + ttri.AB, tC = ttri.A + ttri.AC;

  const vec_t axis[13] = {
      glm::cross(ttri.AB, ttri.AC),
      X,
      Y,
      Z,
      glm::cross(X, tA),
      glm::cross(X, tB),
      glm::cross(X, tC),
      glm::cross(Y, tA),
      glm::cross(Y, tB),
      glm::cross(Y, tC),
      glm::cross(Z, tA),
      glm::cross(Z, tB),
      glm::cross(Z, tC),
  };

  for (const auto &ax : axis) {
    scalar_t pt[3] = {glm::dot(tA, ax), glm::dot(tB, ax), glm::dot(tC, ax)};
    scalar_t pb[3] = {glm::dot(X, ax), glm::dot(Y, ax), glm::dot(Z, ax)};
    scalar_t r = glm::dot(e, glm::abs(vec_t(pb[0], pb[1], pb[2])));
    if (glm::max(-glm::max(glm::max(pt[0], pt[1]), pt[2]),
                 glm::min(glm::min(pt[0], pt[1]), pt[2])) > r) {
      return false;
    }
  }
  return true;
}

bool Mesh::rayHit(const Ray &ray, vec_t &tuv, triangle_ptr &triangle) const {
  for (auto it = grid->begin(ray); it != grid->end(); ++it) {
    if (it->empty()) {
      continue;
    }
    vec_t curr;
    bool hit = it->at(0)->rayHit(ray, tuv);
    for (size_t i = 1; i < it->size(); ++i) {
      const auto &tri = it->at(i);
      if (tri->rayHit(ray, curr)) {
        auto collisionIndex = grid->worldToGrid(ray.at(curr[0]));
        if (glm::all(glm::equal(collisionIndex, it.current))) {
          hit = true;
          if (curr[0] <= tuv[0]) {
            tuv = curr;
            triangle = tri;
          }
        }
      }
    }
    if (hit) {
      return true;
    }
  }
  return false;
}

void Mesh::voxelize(triangle_ptr tri) {
  point_t vertices[3] = {tri->A, tri->A + tri->AB, tri->A + tri->AC};
  const aabb_t mbb = {
      glm::min(glm::min(vertices[0], vertices[1]), vertices[2]),
      glm::max(glm::max(vertices[0], vertices[1]), vertices[2])};

  index_t min = grid->worldToGrid(mbb.min);
  index_t max = grid->worldToGrid(mbb.max);
  auto diff = max - min;
  for (size_t i = min.x; i < max.x + 1; i++) {
    for (size_t j = min.y; j < max.y + 1; j++) {
      for (size_t k = min.z; k < max.z + 1; k++) {
        const index_t index(i, j, k);
        const aabb_t cell = grid->getAABB(index);
        bool intersect = aabbTriangleHit(cell, *tri);
        if (intersect) {
          grid->operator[](index).push_back(tri);
        }
      }
    }
  }
}
const typename Mesh::grid_ptr Mesh::getGrid() const { return grid; }

Mesh::Loader::Loader() : mesh(std::make_unique<Mesh>()) {}
bool Mesh::Loader::OBJ(const std::string &filename) {
  tinyobj::ObjReaderConfig reader_config;
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(filename, reader_config)) {
    return false;
  }

  auto &attrib = reader.GetAttrib();
  auto &shapes = reader.GetShapes();
  auto &materials = reader.GetMaterials();

  vec_t min(MAX_SCALAR), max(MIN_SCALAR);
  // vec_t minDiff = vec_t(MAX_SCALAR);

  // Loop over shapes
  for (size_t s = 0; s < shapes.size(); s++) {
    // Loop over faces(polygon)
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
      size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
      vec_t vertices[fv];

      // Loop over vertices in the face.
      for (size_t v = 0; v < fv; v++) {
        // access to vertex
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
        tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
        tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
        tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

        vertices[v] = vec_t(vx, vy, vz);
        min = glm::min(vertices[v], min);
        max = glm::max(vertices[v], max);
      }
      // CREATE THE TRIANGLE
      mesh->triangles.emplace_back(
          std::make_shared<Triangle>(vertices[0], vertices[1], vertices[2]));
      //      minDiff = glm::min(glm::min(glm::abs(vertices[0] - vertices[1]),
      //                                 glm::abs(vertices[1] - vertices[2])),
      //                        glm::abs(vertices[0] - vertices[2]));
      index_offset += fv;
    }
  }
  // init grid
  mesh->grid = std::make_shared<grid_t>(min, max, grid_t::index_t(4));
  // mesh->grid = std::make_shared<grid_t>(min, max, grid_t::index_t(32));

  // index the triangles
  for (size_t i = 0; i < mesh->triangles.size(); i++) {
    mesh->voxelize(mesh->triangles[i]);
  }

  return true;
}
const typename Mesh::mesh_ptr Mesh::Loader::getMesh() const { return mesh; }
} // namespace war
