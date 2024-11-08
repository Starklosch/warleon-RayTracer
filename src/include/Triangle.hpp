#pragma once
#include <Ray.hpp>
#include <common.hpp>

namespace war {
    class Triangle {
    public:
        const point_t A;
        const vec_t AB, AC;

        Triangle(point_t a, point_t b, point_t c);

        // Fast, Minimum Storage Ray-Triangle Intersection
        bool rayHit(Ray ray, vec_t& tuv) const;
        point_t support(const vec_t& direction) const;
        point_t getPoint() const;
    };

    using triangle_ptr = Triangle*;
} // namespace war
