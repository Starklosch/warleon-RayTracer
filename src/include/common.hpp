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

typedef double scalar_t;
typedef glm::tvec3<int> ivec_t;
typedef glm::tvec3<scalar_t> vec_t;
typedef glm::tvec4<scalar_t> vec4_t;
typedef vec_t point_t;
typedef glm::tvec2<scalar_t> coord2d_t;
typedef glm::mat<4, 4, scalar_t> rotationMatrix_t;
typedef glm::mat<4, 4, scalar_t> projectionMatrix_t;

const scalar_t EPSILON = std::numeric_limits<scalar_t>::epsilon();
const scalar_t MAX_SCALAR = std::numeric_limits<scalar_t>::max();
const scalar_t MIN_SCALAR = std::numeric_limits<scalar_t>::min();
const size_t MAX_INDEX = std::numeric_limits<size_t>::max();

scalar_t min(const vec_t &vec);
scalar_t max(const vec_t &vec);

}; // namespace war
