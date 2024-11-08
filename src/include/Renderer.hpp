#pragma once
#include <Camera.hpp>
#include <Canvas.hpp>
#include <Scene.hpp>
#include "Reference.h"

namespace war {
	class Renderer {
	public:
		const Canvas canvas;
		const Camera camera;
		const Reference<Scene> scene;

		Renderer(const Canvas& canvas, const Camera& camera, const Reference<Scene> scene);
		void render() const;
	};
} // namespace war
