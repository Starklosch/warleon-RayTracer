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
  const aabb_t tbox = {mbb.min - center, mbb.max - center};
  const point_t A = tri.A, B = tri.A + tri.AB, C = tri.A + tri.AC;

  // const Triangle ttri(A - center, B - center, C - center);
  const vec_t BC = C - B;

  const vec_t X(1.0, 0.0, 0.0);
  const vec_t Y(0.0, 1.0, 0.0);
  const vec_t Z(0.0, 0.0, 1.0);

  const vec_t axis[13] = {
      X,
      Y,
      Z,
      glm::cross(X, A),
      glm::cross(X, B),
      glm::cross(X, C),
      glm::cross(Y, A),
      glm::cross(Y, B),
      glm::cross(Y, C),
      glm::cross(Z, A),
      glm::cross(Z, B),
      glm::cross(Z, C),
      glm::cross(tri.AB, tri.AC),
  };

  for (const auto &ax : axis) {
    scalar_t pt[3] = {glm::dot(A, ax), glm::dot(B, ax), glm::dot(C, ax)};
    scalar_t pb[3] = {glm::dot(X, ax), glm::dot(Y, ax), glm::dot(Z, ax)};
    scalar_t r =
        glm::dot(e, vec_t(glm::abs(pb[0]), glm::abs(pb[1]), glm::abs(pb[2])));
    if (glm::max(-glm::max(glm::max(pt[0], pt[1]), pt[2]),
                 glm::min(glm::max(pt[0], pt[1]), pt[2])) > r) {
      return false;
    }
  }
  return true;
}
void Mesh::voxelize(triangle_ptr tri) {
  point_t vertices[3] = {tri->A, tri->A + tri->AB, tri->A + tri->AC};
  const aabb_t mbb = {
      glm::min(glm::min(vertices[0], vertices[1]), vertices[2]),
      glm::max(glm::max(vertices[0], vertices[1]), vertices[2])};

  index_t min = grid->worldToGrid(mbb.min);
  index_t max = grid->worldToGrid(mbb.max);

  for (size_t i = min.x; i < max.x; i++) {
    for (size_t j = min.y; j < max.y; j++) {
      for (size_t k = min.z; k < max.z; k++) {
        const index_t index(i, j, k);
        const aabb_t cell = grid->getAABB(index);
        if (aabbTriangleHit(cell, *tri)) {
          grid->operator[](index).push_back(tri);
        }
      }
    }
  }
}

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

  vec_t min = vec_t(MAX_SCALAR), max = vec_t(MIN_SCALAR);
  vec_t minDiff = vec_t(MAX_SCALAR);

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
        min = glm::max(vertices[v], max);
      }
      // CREATE THE TRIANGLE
      mesh->triangles.emplace_back(vertices[0], vertices[1], vertices[2]);
      minDiff = glm::min(glm::min(glm::abs(vertices[0] - vertices[1]),
                                  glm::abs(vertices[1] - vertices[2])),
                         glm::abs(vertices[0] - vertices[2]));
      index_offset += fv;
    }
  }
  // init grid
  mesh->grid = std::make_unique<grid_t>(min, max,
                                        grid_t::index_t((max - min) / minDiff));

  // index the triangles
  for (size_t i = 0; i < mesh->triangles.size(); i++) {
    triangle_ptr tp(&mesh->triangles[i]);
    mesh->voxelize(tp);
  }

  return true;
}
const std::shared_ptr<Mesh> Mesh::Loader::getMesh() const { return mesh; }
} // namespace war
