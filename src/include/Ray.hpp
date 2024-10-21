#pragma once
#include <common.hpp>

namespace war{

class Ray{
public:
	const point_t O; //origin
	const vec_t D; //direction

	Ray(const point_t&& o,const vec_t&& d);
	Ray(const point_t& o,const vec_t& d);
};
};
