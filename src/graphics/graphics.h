#pragma once
#include <common.h>
#include <graphics/Screen.h>
#include <graphics/Mesh.h>
#include <graphics/Shader.h>
#include <graphics/MeshRenderer.h>
#include <graphics/Camera.h>
#include <graphics/Texture.h>

namespace spruce {
	namespace graphics {
		Mesh* createMesh(uint16 vertexCount, float* vertices, uint16 indexCount, uint16* indices);
		Shader* createShader(string& vertSource, string& fragSource, uint16 attributesCount, VertexAttribute* attributes);
		MeshRenderer* createMeshRenderer();
		Camera* createPerspectiveCamera(float viewportWidth, float viewportHeight, float fieldOfView, float near, float far, vec3f& up);
		Camera* createOrthographicCamera(float viewportWidth, float viewportHeight, float near, float far, vec3f& up);
		Texture* createTexture(string& path);

		int getWidth();
		int getHeight();
	}
}
