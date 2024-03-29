#pragma once
#include <common.h>
#include <task/Task.h>
#include <task/TaskConfig.h>

namespace spruce {
	namespace task {
		template <typename T>
		class TaskGroup;

		template <typename OUTPUT, typename ... TYPES>
		class TaskGroup<OUTPUT(TYPES...)> {
			public:
				std::vector<Task<OUTPUT(TYPES...)>> tasks;

				TaskGroup() = default;
				TaskGroup(const TaskGroup& group);
				TaskGroup(TaskGroup&&) noexcept = default;
				~TaskGroup() = default;

				void addTask(TaskConfig<OUTPUT(TYPES...)> config) noexcept;
				[[nodiscard]] bool complete() const noexcept;

				TaskGroup& operator=(const TaskGroup&) = default;
				TaskGroup& operator=(TaskGroup&&) noexcept = default;
		};
	}

	using task::TaskGroup;
}

#include <task/TaskGroupImpl.h>
