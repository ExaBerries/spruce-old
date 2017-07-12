#pragma once
#include <common.h>
#include <GLFW/glfw3.h>
#include <graphics/Texture.h>

namespace spruce {
	class OpenGLTexture: public Texture {
		public:
			GLuint texture;

			OpenGLTexture(uint8* data, uint16& width, uint16& height, uint16& bitsPerPixel);
			virtual ~OpenGLTexture();

			void bind(uint16 unit);
			void toVRAM();
	};
}
