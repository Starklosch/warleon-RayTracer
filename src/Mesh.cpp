#include <Mesh.hpp>
#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif
#include <tiny_obj_loader.h>

namespace war {
Mesh::Mesh() : grid(nullptr), triangles() {}
void Mesh::voxelize(const Triangle &tri) {
  // TODO implement a triangle volxelization algorithm
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
        //
        //      // Check if  is zero or positive. negative = no normal data
        //      if (idx.normal_index >= 0) {
        //        tinyobj::real_t nx =
        //        attrib.normals[3*size_t(idx.normal_index)+0]; tinyobj::real_t
        //        ny = attrib.normals[3*size_t(idx.normal_index)+1];
        //        tinyobj::real_t nz =
        //        attrib.normals[3*size_t(idx.normal_index)+2];
        //      }
        //
        //      // Check if  is zero or positive. negative = no texcoord data
        //      if (idx.texcoord_index >= 0) {
        //        tinyobj::real_t tx =
        //        attrib.texcoords[2*size_t(idx.texcoord_index)+0];
        //        tinyobj::real_t ty =
        //        attrib.texcoords[2*size_t(idx.texcoord_index)+1];
        //      }

        // Optional: vertex colors
        // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
        // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
        // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
      }
      // CREATE THE TRIANGLE
      mesh->triangles.emplace_back(vertices[0], vertices[1], vertices[2]);
      minDiff = glm::min(glm::min(glm::abs(vertices[0] - vertices[1]),
                                  glm::abs(vertices[1] - vertices[2])),
                         glm::abs(vertices[0] - vertices[2]));
      index_offset += fv;

      // per-face material
      // shapes[s].mesh.material_ids[f];
    }
  }
  // create grid
  mesh->grid = std::make_unique<grid_t>(min, max,
                                        grid_t::index_t((max - min) / minDiff));

  return true;
}

const std::shared_ptr<Mesh> Mesh::Loader::getMesh() const { return mesh; }
} // namespace war
