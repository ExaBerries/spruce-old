#include <Frame.h>
#include <backend/task/taskmanager.h>
#ifdef DEBUG
#ifdef PROFILE
#include <util/profile/profile.h>
#include <util/profile/FrameProfileData.h>
#endif
#endif

namespace spruce {
	Frame::Frame() {
		#ifdef DEBUG
		#ifdef PROFILE
		encodeStartTime = 0;
		encodeEndTime = 0;
		executeStartTime = 0;
		executeEndTime = 0;
		delta = 0;
		#endif
		#endif
	}

	Frame::Frame(const Frame& frame) {
		this->mainCommandBuffer = frame.mainCommandBuffer;
		this->commandBuffers = frame.commandBuffers;
		#ifdef DEBUG
		#ifdef PROFILE
		this->encodeStartTime = frame.encodeStartTime;
		this->encodeEndTime = frame.encodeEndTime;
		this->executeStartTime = frame.executeStartTime;
		this->executeEndTime = frame.executeEndTime;
		this->delta = frame.delta;
		#endif
		#endif
	}

	Frame::~Frame() {
		mainCommandBuffer.reset();
		for (auto& cmdBuffer : commandBuffers) {
			cmdBuffer.second.reset();
		}
		#ifdef DEBUG
		#ifdef PROFILE
		util::profile::dataMutex.lock();
		util::profile::FrameProfileData frameData;
		frameData.encodeStartTime = encodeStartTime;
		frameData.encodeEndTime = encodeEndTime;
		frameData.executeStartTime = executeStartTime;
		frameData.executeEndTime = executeEndTime;
		frameData.delta = delta;
		util::profile::data.frameProfiles.push_back(frameData);
		util::profile::dataMutex.unlock();
		#endif
		#endif
	}

	CommandBuffer& Frame::getCommandBuffer() {
		for (auto& cmdBuffer : commandBuffers) {
			if (cmdBuffer.first == std::this_thread::get_id()) {
				return cmdBuffer.second;
			}
		}
		return mainCommandBuffer;
	}

	buffer<CommandBuffer*> Frame::getCommandBuffers() {
		buffer<CommandBuffer*> cmdBuffers(commandBuffers.size() + 1);
		uint16 i = 0;
		cmdBuffers[i++] = &mainCommandBuffer;
		for (auto& cmdBuffer : commandBuffers) {
			cmdBuffers[i++] = &cmdBuffer.second;
		}
		return cmdBuffers;
	}

	Frame& Frame::operator=(Frame frame) {
		this->mainCommandBuffer = frame.mainCommandBuffer;
		this->commandBuffers = frame.commandBuffers;
		#ifdef DEBUG
		#ifdef PROFILE
		this->encodeStartTime = frame.encodeStartTime;
		this->encodeEndTime = frame.encodeEndTime;
		this->executeStartTime = frame.executeStartTime;
		this->executeEndTime = frame.executeEndTime;
		this->delta = frame.delta;
		#endif
		#endif
		return *this;
	}
}