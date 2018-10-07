#pragma once
#include <common.h>
#include <API.h>
#include <backend/Window.h>
#include <backend/api/RenderAPI.h>
#include <graphics/Screen.h>
#include <pipeline/Pipeline.h>
#include <pipeline/SimplePipeline.h>
#include <pipeline/EncodeExecutePipeline.h>

namespace spruce {
	namespace app {
		extern std::vector<std::function<void()>> freeCallbacks;
		extern Window* window;
		extern API apiType;
		extern RenderAPI* api;
		extern graphics::Screen* screen;
		extern bool debug;
		extern Pipeline* pipeline;

		void init();
		void run();
		void free();

		void setPipeline(Pipeline* pipeline);
		bool supportsAPI(API api);
		void setRenderAPI(API api);
		void setScreen(graphics::Screen* newScreen);
		void clearCommands();
		void addFreeCallback(std::function<void()> function);
	}
}
