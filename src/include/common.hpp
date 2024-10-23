#pragma once

#include <glm/glm.hpp>
#include <id.hpp>
#include <limits>

namespace war {

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
const ID IDgenerator;

scalar_t min(const vec_t &vec);
scalar_t max(const vec_t &vec);

}; // namespace war
