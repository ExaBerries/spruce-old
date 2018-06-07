#pragma once
#include <common.h>
#include <graphics/Texture.h>
#include <Metal/Metal.h>

namespace spruce {
	class MetalTexture : public Texture {
		public:
			id<MTLTexture> mtlTexture;

			MetalTexture(PixelFormat format, buffer<uint8> data, uint16& width, uint16& height);
			MetalTexture(const MetalTexture& texture) = delete;
			~MetalTexture();

			void toVRAM();
			void freeVRAM();
	};
}
