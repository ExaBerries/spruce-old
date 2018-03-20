#include <backend/api/opengl/OpenGL.h>
#include <backend/api/opengl/platform.h>
#include <backend/api/opengl/OpenGLMesh.h>
#include <backend/api/opengl/OpenGLShader.h>
#include <backend/api/opengl/OpenGLShapeRenderer.h>
#include <backend/api/opengl/OpenGLTexture.h>
#include <backend/api/opengl/OpenGLRenderTarget.h>
#include <io/image.h>

namespace spruce {
	OpenGL::OpenGL(Window* window) : RenderAPI(window, vec3f(2, 2, 2)) {
		fontVert =
				#include "font.vert"
		;
		fontFrag =
				#include "font.frag"
		;
	}

	OpenGL::~OpenGL() {
	}

	void OpenGL::init() {
		makeContextCurrent(window);
		swapInterval(window, 1);
		window->setVisible(true);
	}

	void OpenGL::updateStart() {
		glViewport(0, 0, window->width, window->height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGL::updateEnd() {
		swapBuffers(window);
	}

	string OpenGL::getError() {
		return std::to_string(glGetError());
	}

	Mesh* OpenGL::createMesh(uint16 vertexCount, float* vertices, uint16 indexCount, uint16* indices) {
		return new OpenGLMesh(vertexCount, vertices, indexCount, indices);
	}

	Shader* OpenGL::createShader(uint8* vertData, uint16 vertDataSize, uint8* fragData, uint16 fragDataSize, uint16 attributesCount, VertexAttribute* attributes) {
		return new OpenGLShader(vertData, vertDataSize, fragData, fragDataSize, attributesCount, attributes);
	}

	Shader* OpenGL::createShader(string& vertSource, string& fragSource, uint16 attributesCount, VertexAttribute* attributes) {
		return new OpenGLShader(vertSource, fragSource, attributesCount, attributes);
	}

	ShapeRenderer* OpenGL::createShapeRenderer() {
		return new OpenGLShapeRenderer();
	}

	Texture* OpenGL::createTexture(const FileHandle& path) {
		uint16 width = 0;
		uint16 height = 0;
		uint16 bitsPerPixel = 0;
		uint8* data = io::loadImage(path, width, height, bitsPerPixel);
		return new OpenGLTexture(Texture::RGBA, data, width, height);
	}

	Texture* OpenGL::createTexture(Texture::PixelFormat format, uint8* data, uint16 width, uint16 height) {
		return new OpenGLTexture(format, data, width, height);
	}

	RenderTarget* OpenGL::createRenderTarget(Texture::PixelFormat format, uint16 width, uint16 height) {
		return new OpenGLRenderTarget(format, width, height);
	}

	void OpenGL::render(Mesh* mesh, Shader* shader) {
		setDepth(true);
		mesh->bind();
		for (int i = 0; i < shader->attributeCount; i++) {
			glEnableVertexAttribArray(shader->getAttributeLocation(shader->attributes[i].name));
		}
		if (mesh->indexCount > 0) {
			glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_SHORT, 0);
		} else {
			glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount);
		}
		for (int i = 0; i < shader->attributeCount; i++) {
			glDisableVertexAttribArray(shader->getAttributeLocation(shader->attributes[i].name));
		}
		mesh->unbind();
	}

	void OpenGL::renderStart(graphics::RenderPass* renderPass) {
		if (renderPass->target != nullptr) {
			renderPass->target->bind();
			glViewport(0, 0, renderPass->target->width, renderPass->target->height);
		} else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, window->width, window->height);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGL::setBlend(bool value) {
		if (value) {
			glEnable(GL_BLEND);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
		} else {
			glDisable(GL_BLEND);
		}
	}

	void OpenGL::setDepth(bool value) {
		if (value) {
			glEnable(GL_DEPTH_TEST);
		} else {
			glDisable(GL_DEPTH_TEST);
		}
	}

	string OpenGL::getGPUVendor() {
		return string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	}

	uint16 OpenGL::getAPIVersionMajor() {
		int32 major = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		return major;
	}

	uint16 OpenGL::getAPIVersionMinor() {
		int32 minor = 0;
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		return minor;
	}

	string OpenGL::getAPIRendererName() {
		return string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	}

	void OpenGL::setPerspective(mat4f& matrix, float& near, float& far, float& fov, float& aspectRatio) {
		float size = 1.0 / tan(fov / 2.0);
		matrix.values[0] = size / aspectRatio;
		matrix.values[1] = 0.0;
		matrix.values[2] = 0.0;
		matrix.values[3] = 0.0;
		matrix.values[4] = 0.0;
		matrix.values[5] = size;
		matrix.values[6] = 0.0;
		matrix.values[7] = 0.0;
		matrix.values[8] = 0.0;
		matrix.values[9] = 0.0;
		matrix.values[10] = (far + near) / (near - far);
		matrix.values[11] = (2.0 * far * near) / (near - far);
		matrix.values[12] = 0.0;
		matrix.values[13] = 0.0;
		matrix.values[14] = -1.0;
		matrix.values[15] = 0.0;
	}

	void OpenGL::setOrthographic(mat4f& matrix, float left, float right, float top, float bottom, float near, float far) {
		matrix.values[0] = 2 / (right - left);
		matrix.values[1] = 0;
		matrix.values[2] = 0;
		matrix.values[3] = (left + right) / (left - right);
		matrix.values[4] = 0;
		matrix.values[5] = 2 / (top - bottom);
		matrix.values[6] = 0;
		matrix.values[7] = (top + bottom) / (bottom - top);
		matrix.values[8] = 0;
		matrix.values[9] = 0;
		matrix.values[10] = 2 / (near - far);
		matrix.values[11] = near / (near - far);
		matrix.values[12] = 0;
		matrix.values[13] = 0;
		matrix.values[14] = 0;
		matrix.values[15] = 1;
	}
}