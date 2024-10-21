#pragma once
#include <common.hpp>
#include <Ray.hpp>

namespace war{
class Triangle{
public:
	const point_t A;
	const vec_t AB,AC;

	Triangle(point_t a,point_t b, point_t c);

	//Fast, Minimum Storage Ray-Triangle Intersection
	bool rayHit(Ray ray,vec_t& t);
};
}
