#pragma once
#include <common.h>
#include <graphics/renderer/RendererAbstractor.h>

namespace spruce {
	namespace graphics {
		namespace camera {
			class Camera {
				public:
					mat4f projection;
					mat4f view;
					mat4f combined;
					mat4f inverse;
					vec3f position;
					quaternion rotation;
					float viewportWidth;
					float viewportHeight;

					Camera(const float& viewportWidth, const float& viewportHeight);
					Camera(const Camera& camera) = default;
					Camera(Camera&&) = default;
					virtual ~Camera() = default;

					virtual void update(RendererAbstractor* renderer) = 0;
					vec3f unproject(const vec2f& screen, float depth);
					vec2f project(const vec3f& world);

					Camera& operator=(const Camera&) = default;
					Camera& operator=(Camera&&) = default;
			};
		}
	}

	using graphics::camera::Camera;
}
