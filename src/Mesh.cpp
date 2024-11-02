#include <Mesh.hpp>
#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif
#include <GJK.hpp>
#include <tiny_obj_loader.h>

namespace war {
Mesh::Mesh() : grid(nullptr), triangles() {}

bool Mesh::aabbTriangleHit(const aabb_t &mbb, const Triangle &tri) const {
  const vec_t &e = grid->boxSize;
  const point_t center = mbb.min + e / 2.0;
  const point_t A = tri.A - center;
  const point_t B = tri.A + tri.AB - center;
  const point_t C = tri.A + tri.AC - center;
  const vec_t AB = B - A;
  const vec_t AC = C - A;

  const vec_t X(1.0, 0.0, 0.0);
  const vec_t Y(0.0, 1.0, 0.0);
  const vec_t Z(0.0, 0.0, 1.0);

  const vec_t axis[13] = {
      X,
      Y,
      Z,
      glm::normalize(glm::cross(AB, AC)),
      glm::normalize(glm::cross(X, A)),
      glm::normalize(glm::cross(X, B)),
      glm::normalize(glm::cross(X, C)),
      glm::normalize(glm::cross(Y, A)),
      glm::normalize(glm::cross(Y, B)),
      glm::normalize(glm::cross(Y, C)),
      glm::normalize(glm::cross(Z, A)),
      glm::normalize(glm::cross(Z, B)),
      glm::normalize(glm::cross(Z, C)),
  };

  for (const auto &ax : axis) {
    if (glm::length(ax) < EPSILON)
      continue;

    vec_t pt(glm::dot(A, ax), glm::dot(B, ax), glm::dot(C, ax));
    vec_t pb(glm::dot(X, ax), glm::dot(Y, ax), glm::dot(Z, ax));
    scalar_t r = glm::dot(e, glm::abs(pb));

    scalar_t minpt = glm::min(glm::min(pt[0], pt[1]), pt[2]);
    scalar_t maxpt = glm::max(glm::max(pt[0], pt[1]), pt[2]);

    if (minpt > r || maxpt < -r) {
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
    for (size_t i = 0; i < it->size(); ++i) {
      const auto &tri = it->at(i);
      if (tri->rayHit(ray, curr)) {
        // auto collisionIndex = grid->worldToGrid(ray.at(curr[0]));
        // if (glm::all(glm::equal(collisionIndex, it.current))) {
        hit = true;
        if (curr[0] <= tuv[0]) {
          tuv = curr;
          triangle = tri;
          //}
          //}
        }
      }
      if (hit) {
        return true;
      }
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
  for (size_t i = min.x; i <= max.x; i++) {
    for (size_t j = min.y; j <= max.y; j++) {
      for (size_t k = min.z; k <= max.z; k++) {
        const index_t index(i, j, k);
        const aabb_t cell = grid->getAABB(index);
         bool intersect = aabbTriangleHit(cell, *tri);
        //bool intersect = GJKcheck(cell, *tri);
        if (intersect) {
          grid->operator[](index).push_back(tri);
        }
      }
    }
  }
}

const typename Mesh::grid_ptr Mesh::getGrid() const { return grid; }

Mesh::Loader::Loader() : mesh(std::make_shared<Mesh>()) {}
bool Mesh::Loader::OBJ(const std::string &filename) {
  tinyobj::ObjReaderConfig reader_config;
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(filename, reader_config)) {
    return false;
  }

  auto &attrib = reader.GetAttrib();
  auto &shapes = reader.GetShapes();
  // auto &materials = reader.GetMaterials();

  vec_t min(MAX_SCALAR), max(MIN_SCALAR);
  // vec_t minDiff = vec_t(MAX_SCALAR);

  print("meshes read = {}\n", shapes.size());
  // Loop over shapes
  for (size_t s = 0; s < shapes.size(); s++) {
    // Loop over faces(polygon)
    size_t index_offset = 0;
    print("triangles in shape[{}] = {}\n", s,
          shapes[s].mesh.num_face_vertices.size());
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
  size_t counter = 0;
  for (size_t i = 0; i < mesh->grid->dimensions[0]; ++i) {
    for (size_t j = 0; j < mesh->grid->dimensions[1]; ++j) {
      for (size_t k = 0; k < mesh->grid->dimensions[2]; ++k) {
        counter += mesh->grid->operator[](index_t(i, j, k)).size();
      }
    }
  }
  print("triangles in grid = {}\n", counter);

  return true;
}
const typename Mesh::mesh_ptr Mesh::Loader::getMesh() const { return mesh; }
} // namespace war
