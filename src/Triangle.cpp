#include <Triangle.hpp>
namespace war {

Triangle::Triangle(point_t a, point_t b, point_t c)
    : A(a), AB(b - a), AC(c - a) {}

bool Triangle::rayHit(Ray ray, vec_t &tuv) {
  scalar_t &t = tuv.x;
  scalar_t &u = tuv.y;
  scalar_t &v = tuv.z;

  vec_t P = glm::cross(ray.D, AC);
  scalar_t det = glm::dot(AB, P);

  if (det > -EPSILON && det < EPSILON)
    return false;

  scalar_t idet = 1.0 / det;

  vec_t T = ray.O - A;

  u = glm::dot(T, P) * idet;
  if (u < 0.0 || u > 1.0)
    return false;

  vec_t Q = glm::cross(T, AB);
  v = glm::dot(ray.D, Q) * idet;
  if (v < 0.0 || v > 1.0)
    return false;

  t = glm::dot(AC, Q) * idet;
  return true;
}
}; // namespace war
