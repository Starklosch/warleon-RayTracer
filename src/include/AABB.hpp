#include <common.hpp>
namespace war {
typedef struct AABB {
  point_t min;
  point_t max;

  point_t support(const vec_t &direction) const;
  point_t getPoint() const;

} aabb_t;
} // namespace war
