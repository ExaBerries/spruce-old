#pragma once
#include <common.h>
#include <graphics/renderer/RendererAbstractor.h>
#include <app/API.h>
#include <app/Application.h>
#include <graphics/MeshFwd.h>
#include <graphics/MeshAPIData.h>
#include <graphics/TextureFwd.h>
#include <graphics/TextureAPIData.h>

namespace spruce {
	namespace graphics {
		template <typename EncodeT, typename ExecuteT, enum app::API api>
		class Renderer : public RendererAbstractor {
			public:
				Renderer() = default;
				Renderer(const Renderer&) = default;
				Renderer(Renderer&&) noexcept = default;
				~Renderer() override = default;

				[[nodiscard]] std::any encodeBackend(void* encodeData) noexcept override;
				void executeBackend(std::any executeData) noexcept override;
				
				[[nodiscard]] virtual ExecuteT encode(EncodeT&) noexcept = 0;
				virtual void execute(ExecuteT&) noexcept = 0;

				Renderer& operator=(const Renderer&) = default;
				Renderer& operator=(Renderer&&) noexcept = default;
		};

		template <typename EncodeT, typename ExecuteT, enum app::API api>
		[[nodiscard]] std::any Renderer<EncodeT, ExecuteT, api>::encodeBackend(void* encodeData) noexcept {
			static_assert(std::is_copy_assignable_v<ExecuteT>);
			return encode(*static_cast<EncodeT*>(encodeData));
		}

		template <typename EncodeT, typename ExecuteT, enum app::API api>
		void Renderer<EncodeT, ExecuteT, api>::executeBackend(std::any executeData) noexcept {
			ExecuteT executeDataCasted = std::any_cast<ExecuteT>(executeData);
			execute(executeDataCasted);
		}
	}

	using graphics::Renderer;
}
