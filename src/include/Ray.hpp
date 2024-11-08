#pragma once
#include <common.hpp>

namespace war {

class Ray {
public:
  const point_t O; // origin
  const vec_t D;   // direction

  Ray(point_t &&o, vec_t &&d);
  Ray(const point_t &o, const vec_t &d);
  vec_t at(scalar_t t) const;
};
}; // namespace war
