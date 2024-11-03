#include <Triangle.hpp>
namespace war {

Triangle::Triangle(point_t a, point_t b, point_t c)
    : A(a), AB(b - a), AC(c - a) {}

bool Triangle::rayHit(Ray ray, vec_t &tuv) const {
  //  print("------ in Triangle::rayHit ------\n");
  //  print("A =  [{}, {}, {}]\n", A[0], A[1], A[2]);
  //  print("AB =  [{}, {}, {}]\n", AB[0], AB[1], AB[2]);
  //  print("AC =  [{}, {}, {}]\n", AC[0], AC[1], AC[2]);
  scalar_t &t = tuv.x;
  scalar_t &u = tuv.y;
  scalar_t &v = tuv.z;

  vec_t P = glm::cross(ray.D, AC);
  scalar_t det = glm::dot(AB, P);

  // print("P = [{}, {}, {}]\n", P[0], P[1], P[2]);
  // print("det = {}\n", det);
  if (glm::abs(det) < EPSILON) {
    // print("return false\n");
    // print("------ out Triangle::rayHit ------\n");
    return false;
  }

  scalar_t idet = 1.0 / det;

  vec_t T = ray.O - A;
  u = glm::dot(T, P) * idet;
  // print("T = [{}, {}, {}]\n", T[0], T[1], T[2]);
  // print("u = {}\n", u);
  if (u < 0.0 || u > 1.0) {
    // print("return false\n");
    // print("------ out Triangle::rayHit ------\n");
    return false;
  }

  vec_t Q = glm::cross(T, AB);
  v = glm::dot(ray.D, Q) * idet;
  // print("Q = [{}, {}, {}]\n", Q[0], Q[1], Q[2]);
  // print("v = {}\n", v);
  // print("u+v = {}\n", u + v);
  if (v < 0.0 || (u + v) > 1.0) {
    // print("return false\n");
    // print("------ out Triangle::rayHit ------\n");
    return false;
  }

  t = glm::dot(AC, Q) * idet;
  // print("t = {}\n", t);
  if (t > EPSILON) {
    // print("return true\n");
    // print("------ out Triangle::rayHit ------\n");
    return true;
  }

  // print("return false\n");
  // print("------ out Triangle::rayHit ------\n");
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
