#include <backend/task/taskmanager.h>
#include <backend/task/WorkerThread.h>
#include <system/system.h>
#include <app.h>

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

		void init() {
			threads = std::vector<WorkerThread>(sys::getCPUThreadCount() - 1);
			if (app::debug) {
				slog(sys::getCPUThreadCount() - 1, " threads");
			}
		}

		void free() {
			for (uint16 i = 0; i < threads.size(); i++) {
				threads[i].join();
			}
			for (uint32 i = 0; i < mainTasks.size(); i++) {
				delete mainTasks[i];
			}
			for (uint32 i = 0; i < concurrentTasks.size(); i++) {
				delete concurrentTasks[i];
			}
			for (auto taskData : data) {
				delete taskData.second;
			}
		}

		void addTask(uint64 id, TaskData* taskData, TaskBackend* taskBackend) {
			std::lock_guard<std::mutex> taskGuard(task::taskMutex);
			std::lock_guard<std::mutex> dataGuard(task::dataMutex);
			data[id] = taskData;
			if (taskBackend->concurrent) {
				concurrentTasks.push_back(taskBackend);
			} else {
				mainTasks.push_back(taskBackend);
			}
		}

		bool compareTasks(TaskBackend* taskA, TaskBackend* taskB) {
			if (taskA->priority < taskB->priority) {
				return true;
			}
			return false;
		}

		TaskBackend* getConcurrentTask() {
			if (concurrentTasks.size() > 0) {
				TaskBackend* task = concurrentTasks[0];
				uint32 eraseIndex = 0;
				for (uint32 i = 1; i < mainTasks.size(); i++) {
					if (concurrentTasks[i] != nullptr) {
						if (compareTasks(task, concurrentTasks[i])) {
							task = mainTasks[i];
							eraseIndex = i;
						}
					}
				}
				concurrentTasks.erase(concurrentTasks.begin() + eraseIndex);
				return task;
			}
			return nullptr;
		}

		TaskBackend* getMainTask() {
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

		TaskBackend* getNextTask(bool main) {
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

		void incrementRef(uint64 taskId) {
			std::lock_guard<std::mutex> dataGuard(task::refMutex);
			references[taskId]++;
		}

		void deincrementRef(uint64 taskId) {
			std::lock_guard<std::mutex> dataGuard(task::refMutex);
			references[taskId]--;
			if (references[taskId] <= 0) {
				std::lock_guard<std::mutex> dataGuard(task::dataMutex);
				delete data[taskId];
				data.erase(taskId);
				references.erase(taskId);
			}
		}
	}
}