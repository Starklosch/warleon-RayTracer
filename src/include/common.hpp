#pragma once

#include <fmt/core.h>

#include <glm/glm.hpp>
#include <limits>

#ifdef DEVELOPMENT
#define print(...) fmt::print(__VA_ARGS__)
#else
#define print(...)
#endif

namespace war {
	const size_t DIM = 3;

	using scalar_t = float;
	using ivec_t = glm::ivec3;
	using vec_t = glm::vec3;
	using vec4_t = glm::vec4;
	using point_t = vec_t;
	using coord2d_t = glm::vec2;
	using coord2d_t = glm::vec2;
	using rotationMatrix_t = glm::mat4;
	using projectionMatrix_t = glm::mat4;
	using index_t = glm::tvec3<size_t>;

	const scalar_t EPSILON = std::numeric_limits<scalar_t>::epsilon();
	const scalar_t MAX_SCALAR = std::numeric_limits<scalar_t>::max();
	const scalar_t MIN_SCALAR = std::numeric_limits<scalar_t>::min();
	const size_t MAX_INDEX = std::numeric_limits<size_t>::max();

	scalar_t min(const vec_t &vec);
	scalar_t max(const vec_t &vec);
}; // namespace war
