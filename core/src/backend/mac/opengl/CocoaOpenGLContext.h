#pragma once
#ifdef __APPLE__
#include <common.h>
#include <graphics/renderer/api/opengl/OpenGLContext.h>
#include <backend/mac/opengl/OpenGLView.h>

namespace spruce {
	class CocoaOpenGLContext : public OpenGLContext {
		public:
			OpenGLView* view;

			CocoaOpenGLContext(OpenGLView* view);
			CocoaOpenGLContext(const CocoaOpenGLContext&) = delete;
			CocoaOpenGLContext(CocoaOpenGLContext&&) noexcept = delete;
			~CocoaOpenGLContext() = default;

			void makeContextCurrent() override;
			void swapBuffers() override;
			void setSwapInverval(int32 interval) override;

			CocoaOpenGLContext& operator=(const OpenGLContext&) = delete;
			CocoaOpenGlContext& operator=(OpenGLContext&&) noexcept = delete;
	};
}
#endif
