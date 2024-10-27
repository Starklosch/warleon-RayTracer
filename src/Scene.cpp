#include <Scene.hpp>
Scene::Scene(){}
  void Scene::addMesh(mesh_ptr mesh){
	  meshes.push_back(mesh);

  }
  void Scene::addLight(light_ptr light){
	  lights.push_back(light);
  }
  bool Scene::rayHit(const Ray&ray,vec_t& tuv,triangle_ptr&tri,mesh_ptr&mesh){
	  for(const auto m:meshes){
		  if(m->rayHit(ray,tuv,tri)){
			  mesh=m;
			  return true;
		  }
	  }
	  return false;
  }
