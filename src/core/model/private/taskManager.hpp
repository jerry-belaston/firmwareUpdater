#pragma once

#include <memory>
#include <functional>

namespace firmwareUpdater::core::model::priv
{

// Allow align all task on a single sequential thread
// Based on a TimerTask
class TaskManager
{
public:
	using UniquePointer = std::unique_ptr<TaskManager>;
	using Task = std::function<void()>;

	// Construction
	static UniquePointer create();
	virtual ~TaskManager();

	void start();
	void addTask(Task const& task);

private:
	// Constructor disabled (use create)
	TaskManager();

private:
	class PImpl;
	std::unique_ptr<PImpl> _pImpl{ nullptr };
};

} // firmwareUpdater::core::model::priv
