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
const ID IDgenerator;
}; // namespace war
