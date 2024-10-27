#include <Mesh.hpp>
#include <gtest/gtest.h>
#include <unistd.h>

using namespace war;
TEST(MESH_LOADER, load_obj) {
  Mesh::Loader load;
  std::string name("../data/bunny.obj");
  EXPECT_TRUE(access(name.c_str(), F_OK) != -1);
  EXPECT_TRUE(load.OBJ(name));
  size_t count = 0;
  const auto mesh = load.getMesh();
  EXPECT_FALSE(mesh == nullptr);
  const auto grid = mesh->getGrid();
  EXPECT_FALSE(grid == nullptr);
  const auto dim = grid->dimensions;
  for (size_t i = 0; i < dim[0]; i++) {
    for (size_t j = 0; j < dim[1]; j++) {
      for (size_t k = 0; k < dim[2]; k++) {
        count += (grid->operator[](Mesh::index_t(i, j, k))).size();
      }
    }
  }
  EXPECT_TRUE(count > 0) << count;
}
