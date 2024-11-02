#pragma once

#include <fmt/core.h>
#include <glm/glm.hpp>
#include <id.hpp>
#include <limits>

namespace war {

const size_t DIM = 3;

typedef double scalar_t;
typedef glm::tvec3<int> ivec_t;
typedef glm::tvec3<scalar_t> vec_t;
typedef vec_t point_t;
typedef glm::tvec2<scalar_t> coord2d_t;
typedef glm::mat<4, 4, scalar_t> rotationMatrix_t;
typedef ID::type id_t;

const scalar_t EPSILON = std::numeric_limits<scalar_t>::epsilon();
const scalar_t MAX_SCALAR = std::numeric_limits<scalar_t>::max();
const scalar_t MIN_SCALAR = std::numeric_limits<scalar_t>::min();
const size_t MAX_INDEX = std::numeric_limits<size_t>::max();
const ID IDgenerator;

scalar_t min(const vec_t &vec);
scalar_t max(const vec_t &vec);

using fmt::print;

}; // namespace war
