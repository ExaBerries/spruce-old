#pragma once
#include <common.h>
#include <input/input.h>

namespace spruce {
	class Window {
		public:
			uint16 width;
			uint16 height;
			bool visible;
			input::CursorMode cursorMode;

			Window();
			virtual ~Window();

			virtual void setTitle(string title) = 0;
			virtual void setVisible(bool visible) = 0;
			virtual void setFullscreen(bool fullscreen) = 0;
			virtual void close() = 0;
			virtual void setCursorMode(input::CursorMode mode) = 0;
	};
}
