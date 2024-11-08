#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <Triangle.hpp>
namespace war {

Triangle::Triangle(point_t a, point_t b, point_t c)
    : A(a), AB(b - a), AC(c - a) {}

bool Triangle::rayHit(Ray ray, vec_t &tuv) const {
  scalar_t &t = tuv.x;
  scalar_t &u = tuv.y;
  scalar_t &v = tuv.z;

  vec_t P = glm::cross(ray.D, AC);
  scalar_t det = glm::dot(AB, P);

  if (glm::abs(det) < EPSILON) {
    return false;
  }

  scalar_t idet = 1.0 / det;

  vec_t T = ray.O - A;
  u = glm::dot(T, P) * idet;
  if (u < 0.0 || u > 1.0) {
    return false;
  }

  vec_t Q = glm::cross(T, AB);
  v = glm::dot(ray.D, Q) * idet;
  if (v < 0.0 || (u + v) > 1.0) {
    return false;
  }

  t = glm::dot(AC, Q) * idet;
  if (t > EPSILON) {
	  //glm::vec3 center = (A + AB + AC) / 3.f;
   //   std::cout << "ray hit " << glm::to_string(ray.D) << " with triangle " << glm::to_string(center);
    return true;
  }

  return false;
}

point_t Triangle::support(const vec_t &direction) const {
  const point_t points[3] = {
      A,
      A + AB,
      A + AC,
  };
  point_t result = points[0];
  scalar_t max = glm::dot(direction, points[0]);
  for (size_t i = 1; i < 3; ++i) {
    const scalar_t projection = glm::dot(direction, points[i]);
    if (projection > max) {
      max = projection;
      result = points[i];
    }
  }
  return result;
}
point_t Triangle::getPoint() const { return A; }
}; // namespace war
