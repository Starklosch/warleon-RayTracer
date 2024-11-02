#include <AABB.hpp>
namespace war {
point_t AABB::support(const vec_t &direction) const {
  const point_t points[8] = {
      point_t(min[0], min[1], min[2]), point_t(min[0], min[1], max[2]),
      point_t(min[0], max[1], min[2]), point_t(min[0], max[1], max[2]),
      point_t(max[0], min[1], min[2]), point_t(max[0], min[1], max[2]),
      point_t(max[0], max[1], min[2]), point_t(max[0], max[1], max[2]),
  };

  point_t result = points[0];
  scalar_t max = glm::dot(direction, points[0]);
  for (size_t i = 1; i < 8; ++i) {
    const scalar_t projection = glm::dot(direction, points[i]);
    if (projection > max) {
      max = projection;
      result = points[i];
    }
  }
  return result;
}
point_t AABB::getPoint() const { return min; }
} // namespace war
