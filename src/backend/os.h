#pragma once
#include <common.h>
#include <app.h>
#include <backend/Window.h>
#include <input/Key.h>

namespace spruce {
	namespace os {
		void init();
		void free();

		Window* createWindow(app::API api);
		bool supportsAPI(app::API api);

		float timeSec();
		long timeNano();

		void updateStart();
		void updateEnd();

		uint16 codeFor(input::Key key);
	}
}
