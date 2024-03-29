#pragma once
#ifdef __APPLE__
#include <common.h>
#include <graphics/renderer/api/metal/wrapper/MetalObj.h>
#include <graphics/renderer/api/metal/wrapper/MetalTexture.h>
#include <graphics/renderer/api/metal/wrapper/MetalLoadAction.h>
#include <graphics/renderer/api/metal/wrapper/MetalStoreAction.h>
#include <graphics/color.h>

namespace spruce {
	class MetalRenderPassDescriptor : public MetalObj {
		public:
			MetalRenderPassDescriptor();
			MetalRenderPassDescriptor(const MetalRenderPassDescriptor&) = delete;
			MetalRenderPassDescriptor(MetalRenderPassDescriptor&&) noexcept = delete;
			~MetalRenderPassDescriptor() override = default;

			void setColorAttachment(uint32 index, MetalTexture* texture, MetalLoadAction loadAction, MetalStoreAction storeAction, color clearColor) noexcept;
			void setDepthAttachment(MetalTexture* texture, MetalLoadAction loadAction, MetalStoreAction storeAction, double clearDepth) noexcept;
			void setStencilAttachment() noexcept;

			MetalRenderPassDescriptor& operator=(const MetalRenderPassDescriptor&) = delete;
			MetalRenderPassDescriptor& operator=(MetalRenderPassDescriptor&&) noexcept = delete;
	};
}
#endif
