#pragma once
#include <common.hpp>
#include <Ray.hpp>
namespace war{


class Camera{
	
	void initCameraAxes();
	vec_t up,right; 
public:
	const Ray ray;
	const scalar_t distance,width,height,tilt; //viewport related

	Camera(const Ray&r,scalar_t d,scalar_t w,scalar_t h,scalar_t t);

	Ray getRayFromViewportCoordinates(scalar_t x,scalar_t y)const;
};
};
