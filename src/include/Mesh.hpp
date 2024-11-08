#pragma once
#include <Grid.hpp>
#include <Triangle.hpp>
#include <common.hpp>
#include <memory>
#include <string>
#include <vector>

namespace war {
	class Mesh;

	using mesh_ptr = std::shared_ptr<Mesh>;
	using triangle_set = std::vector<Triangle>;
	using grid_t = Grid<triangle_ptr>;
	using grid_ptr = std::shared_ptr<grid_t>;

	class Mesh {
	public:

		void voxelize(triangle_ptr tri);
		bool aabbTriangleHit(const aabb_t& mbb, const Triangle& tri) const;

	public:
		Mesh();
		const grid_ptr getGrid() const;

		class Loader {
			mesh_ptr mesh;

		public:
			Loader();
			bool OBJ(const std::string& filename);
			const mesh_ptr getMesh() const;
		};
		bool rayHit(const Ray& ray, vec_t& tuv, triangle_ptr& tri) const;

		triangle_set triangles;
	private:
		grid_ptr grid;
	};
} // namespace war
