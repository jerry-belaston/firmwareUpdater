#include "taskManager.hpp"

#include <vector>

#include "core/toolkit/timerTask.hpp"

namespace firmwareUpdater::core::model::priv
{

//////////////////////////////////////////////////////////////////////////////////////////////////
// PImpl
//////////////////////////////////////////////////////////////////////////////////////////////////

class TaskManager::PImpl
{
public:
	PImpl(TaskManager& parent);

	// Members
	TaskManager& _parent;
	toolkit::TimerTask::UniquePointer _timerTask{};
	std::vector<Task> _tasks;
};

TaskManager::PImpl::PImpl(TaskManager& parent)
	: _parent{ parent }
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// TaskManager
//////////////////////////////////////////////////////////////////////////////////////////////////

TaskManager::UniquePointer TaskManager::create()
{
	return UniquePointer(new TaskManager());
}

TaskManager::TaskManager()
	: _pImpl(std::make_unique<PImpl>(*this))
{
}

TaskManager::~TaskManager()
{
}

void TaskManager::start()
{
	_pImpl->_timerTask = core::toolkit::TimerTask::create([this]
	{
		// Launch each task sequentially
		for (auto task: _pImpl->_tasks)
			task();
	});
}

void TaskManager::addTask(Task const& task)
{
	_pImpl->_tasks.push_back(task);
}

} // namespace firmwareUpdater::core::model::priv
