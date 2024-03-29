#include <backend/task/taskmanager.h>
#include <backend/task/WorkerThread.h>
#include <system/system.h>

namespace spruce {
	namespace task {
		std::vector<TaskBackend*> mainTasks;
		std::vector<TaskBackend*> concurrentTasks;
		std::map<uint64, TaskData*> data;
		std::map<uint64, uint16> references;
		std::vector<WorkerThread> threads;
		uint64 taskId;
		std::mutex taskMutex;
		std::mutex dataMutex;
		std::mutex refMutex;

		void init() noexcept {
			threads = std::vector<WorkerThread>(sys::getCPUThreadCount() - 1);
			slog(threads.size(), " threads");
		}

		void free() noexcept {
			for (WorkerThread& thread : threads) {
				thread.join();
			}
			for (TaskBackend*& mainTask : mainTasks) {
				delete mainTask;
			}
			for (TaskBackend*& concurrentTask : concurrentTasks) {
				delete concurrentTask;
			}
			for (auto& [id, taskData] : data) {
				delete taskData;
			}
		}

		void addTask(uint64 id, TaskData* taskData, TaskBackend* taskBackend) noexcept {
			std::lock_guard<std::mutex> taskGuard(taskMutex);
			std::lock_guard<std::mutex> dataGuard(dataMutex);
			data[id] = taskData;
			if (taskBackend->concurrent) {
				concurrentTasks.push_back(taskBackend);
			} else {
				mainTasks.push_back(taskBackend);
			}
		}

		bool compareTasks(TaskBackend* taskA, TaskBackend* taskB) noexcept {
			return (taskA->priority < taskB->priority);
		}

		TaskBackend* getConcurrentTask() noexcept {
			if (concurrentTasks.size() > 0) {
				TaskBackend* task = concurrentTasks[0];
				uint32 eraseIndex = 0;
				for (uint32 i = 1; i < concurrentTasks.size(); i++) {
					if (concurrentTasks[i] != nullptr) {
						if (compareTasks(task, concurrentTasks[i])) {
							task = concurrentTasks[i];
							eraseIndex = i;
						}
					}
				}
				concurrentTasks.erase(concurrentTasks.begin() + eraseIndex);
				return task;
			}
			return nullptr;
		}

		TaskBackend* getMainTask() noexcept {
			if (mainTasks.size() > 0) {
				TaskBackend* task = mainTasks[0];
				uint32 eraseIndex = 0;
				for (uint32 i = 1; i < mainTasks.size(); i++) {
					if (mainTasks[i] != nullptr) {
						if (compareTasks(task, mainTasks[i])) {
							task = mainTasks[i];
							eraseIndex = i;
						}
					}
				}
				mainTasks.erase(mainTasks.begin() + eraseIndex);
				return task;
			}
			return nullptr;
		}

		TaskBackend* getNextTask(bool main) noexcept {
			std::lock_guard<std::mutex> taskGuard(taskMutex);
			TaskBackend* nextTask = nullptr;
			if (main) {
				nextTask = getMainTask();
				if (nextTask == nullptr) {
					nextTask = getConcurrentTask();
				}
			} else {
				nextTask = getConcurrentTask();
			}
			return nextTask;
		}

		TaskBackend* getGraphicsTask(bool concurrent) {
			std::lock_guard<std::mutex> taskGuard(taskMutex);
			if (!concurrent) {
				if (mainTasks.size() > 0) {
					TaskBackend* task = mainTasks[0];
					uint32 eraseIndex = 0;
					for (uint32 i = 1; i < mainTasks.size(); i++) {
						if (mainTasks[i] != nullptr) {
							if (mainTasks[i]->priority == GRAPHICS) {
								task = mainTasks[i];
								eraseIndex = i;
								break;
							}
						}
					}
					mainTasks.erase(mainTasks.begin() + eraseIndex);
					return task;
				}
			}
			if (concurrentTasks.size() > 0) {
				TaskBackend* task = concurrentTasks[0];
				uint32 eraseIndex = 0;
				for (uint32 i = 1; i < mainTasks.size(); i++) {
					if (concurrentTasks[i] != nullptr) {
						if (concurrentTasks[i]->priority == GRAPHICS) {
							task = mainTasks[i];
							eraseIndex = i;
							break;
						}
					}
				}
				concurrentTasks.erase(concurrentTasks.begin() + eraseIndex);
				return task;
			}
			return nullptr;
		}

		bool executeTask(task::TaskBackend* taskBackend) noexcept {
			if (taskBackend != nullptr) {
				if (taskBackend->functionData != nullptr) {
					taskBackend->functionData->execute();
					taskBackend->complete = true;
					delete taskBackend;
					return true;
				} else {
					serr("invalid task, functionData == nullptr");
				}
			}
			return false;
		}

		bool executeMainTask() noexcept {
			return executeTask(task::getNextTask(true));
		}

		bool executeGraphicsTask(bool concurrent) noexcept {
			return executeTask(task::getGraphicsTask(concurrent));
		}

		void incrementRef(uint64 taskId) noexcept {
			std::lock_guard<std::mutex> dataGuard(refMutex);
			references[taskId]++;
		}

		void deincrementRef(uint64 taskId) noexcept {
			std::lock_guard<std::mutex> dataGuard(refMutex);
			references[taskId]--;
			if (references[taskId] <= 0) {
				delete data[taskId];
				data.erase(taskId);
				references.erase(taskId);
			}
		}
	}
}
