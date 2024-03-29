#include <graphics/renderer/api/metal/wrapper/MetalCommandBuffer.h>
#ifdef __APPLE__
#include <graphics/renderer/api/metal/mtlplatform.h>

namespace spruce {
	MetalCommandBuffer::MetalCommandBuffer(void* ptr) : MetalObj(ptr) {
	}

	constexpr id<MTLCommandBuffer> castCmdBuffer(void* ptr) noexcept {
		return (__bridge id<MTLCommandBuffer>) ptr;
	}

	constexpr id<MTLDrawable> castDrawable(void* ptr) noexcept {
		return (__bridge id<MTLDrawable>) ptr;
	}

	constexpr MTLRenderPassDescriptor* castRPD(void* ptr) noexcept {
		return (__bridge MTLRenderPassDescriptor*) ptr;
	}

	void MetalCommandBuffer::commit() noexcept {
		[castCmdBuffer(ptr) commit];
	}

	void MetalCommandBuffer::enqueue() noexcept {
		[castCmdBuffer(ptr) enqueue];
	}

	void MetalCommandBuffer::presentDrawable(MetalDrawable* drawable) noexcept {
		[castCmdBuffer(ptr) presentDrawable:castDrawable(drawable->ptr)];
	}

	[[nodiscard]] owner<MetalRenderCommandEncoder> MetalCommandBuffer::createRenderCommandEncoder(MetalRenderPassDescriptor& descriptor) noexcept {
		return new MetalRenderCommandEncoder([castCmdBuffer(ptr) renderCommandEncoderWithDescriptor:castRPD(descriptor.ptr)]);
	}
}
#endif
