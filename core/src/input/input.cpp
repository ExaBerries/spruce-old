#include <input/input.h>
#include <backend/Window.h>
#include <backend/os.h>
#include <task/async.h>

namespace spruce {
	namespace input {
		std::vector<InputProcessor*> processors;
		std::mutex processorsMutex;

		void addProcessor(InputProcessor* processor) noexcept {
			std::lock_guard<std::mutex> processorsLock(processorsMutex);
			processors.push_back(processor);
		}

		void removeProcessor(InputProcessor* processor) noexcept {
			std::lock_guard<std::mutex> processorsLock(processorsMutex);
			for (uint32 i = 0; i < processors.size(); i++) {
				if (processors[i] == processor) {
					processors.erase(processors.begin() + i);
				}
			}
		}

		void setCursorMode(CursorMode mode) noexcept {
			//app::window->setCursorMode(mode);
		}

		uint16 codeFor(Key key) noexcept {
			return os::codeFor(key);
		}
	}
}
