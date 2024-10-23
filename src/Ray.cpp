#include <Ray.hpp>

namespace war {
Ray::Ray(const point_t &&o, const vec_t &&d)
    : O(o), D(glm::normalize(d)), id(IDgenerator.generate()) {}
Ray::Ray(const point_t &o, const vec_t &d)
    : O(o), D(glm::normalize(d)), id(IDgenerator.generate()) {}

vec_t Ray::at(scalar_t t) const { return O + t * D; }

}; // namespace war
