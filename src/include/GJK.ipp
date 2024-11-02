#include <GJK.hpp>
#include <common.hpp>
namespace war {
typedef point_t simplex_t[4];

template <class shape1_t, class shape2_t>
bool GJKcheck(const shape1_t &s1, const shape2_t &s2) {
  vec_t dir = s2.getPoint() - s1.getPoint();
  point_t origin = vec_t(0.0);
  simplex_t simplex;
  //begin build initial simplex
  simplex[0]= s1.support(dir) - s2.support(-dir);
  dir = origin -simplex[0];
  simplex[1] = s1.support(dir) - s2.support(-dir);
  if(glm::dot(dir,simplex[1])<0)return false;
  vec_t _10 = simplex[0]-simplex[1];
  dir= glm::cross(glm::cross(_10,-simplex[1]),_10);
  simplex[2] = s1.support(dir) - s2.support(-dir);
  if(glm::dot(dir,simplex[2])<0)return false;
  vec_t _12 = simplex[2]-simplex[1];
  dir = glm::cross(_10,_12);
  simplex[3] = s1.support(dir) - s2.support(-dir);
  if(glm::dot(dir,simplex[3])<0)return false;
  //end build initial simplex
  while(true){
  vec_t _30 = simplex[0]-simplex[3];
  vec_t _31 = simplex[1]-simplex[3];
  vec_t _32 = simplex[2]-simplex[3];
	vec_t normals[3] = {
		glm::cross(_31,_30),// remove 0
		glm::cross(_32,_31),// remove 1
		glm::cross(_30,_32),//remove 2
	};
	bool inside = true;
	size_t i=0;
	for(;i<3;++i){
		dir = normals[i];
		if(glm::dot(dir,-simplex[3])>0){
			inside=false;
			break;
		}
	}
	if(inside)return true;
	point_t _4 = s1.support(dir) - s2.support(-dir);
	for(size_t j;j<4;++j){
		if(_4 == simplex[j])return false;
	}
	switch(i){
		case 0:
		simplex = {
			simplex[1],
			simplex[2],
			simplex[3],
			_4,
		};
		break;
		case 1:
		simplex = {
			simplex[0],
			simplex[2],
			simplex[3],
			_4,
		};
		break;
		case 2:
		simplex = {
			simplex[0],
			simplex[1],
			simplex[3],
			_4,
		};
		break;
	}
   
  }
}

} // namespace war
