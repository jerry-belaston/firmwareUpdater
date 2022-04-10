#include "timerTask.hpp"

#include <condition_variable>
#include <mutex>
#include <thread>

namespace firmwareUpdater::core::toolkit
{

//////////////////////////////////////////////////////////////////////////////////////////////////
// PImpl
//////////////////////////////////////////////////////////////////////////////////////////////////

class TimerTask::PImpl
{
public:
	PImpl(TimerTask& parent, TaskHandler const& onTask, std::chrono::milliseconds const& timer);
	virtual ~PImpl();
	void run();

	// Members
	TimerTask& _parent;
	TaskHandler _onTask;
	std::chrono::milliseconds _timer{ 0ms };
	std::thread _thread;
	std::condition_variable _condition{};
	std::mutex _mutex{};
	std::atomic<bool> _shouldTerminate{ false };
};

TimerTask::PImpl::PImpl(TimerTask& parent, TaskHandler const& onTask, std::chrono::milliseconds const& timer)
	: _parent{ parent }
	, _onTask{ onTask }
	, _timer{ timer }
{
}

TimerTask::PImpl::~PImpl()
{
	// "Notify thread we want to terminate"
	{
		auto const lock = std::lock_guard<std::mutex>(_mutex);
		_shouldTerminate = true;
	}
	_condition.notify_all();

	// Wait for thread to complete
	if (_thread.joinable())
		_thread.join();
}

void TimerTask::PImpl::run()
{
	_thread = std::thread([this]
	{
		// Process once before sleeping
		_onTask();

		for (;;)
		{
			{
				// Init previousTime to now
				auto previousTime = std::chrono::steady_clock::now();

				// Wait until elapsed time
				auto lock = std::unique_lock<std::mutex>(_mutex);
				_condition.wait_until(lock, previousTime + _timer, [this, &previousTime]
				{
					auto const delta = previousTime - std::chrono::steady_clock::now();
					return _shouldTerminate || (delta > _timer);
				});

				// Termination if requested
				if (_shouldTerminate)
					return;
			}

			// Process task
			_onTask();
		}
	});
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// TimerTask
//////////////////////////////////////////////////////////////////////////////////////////////////

TimerTask::UniquePointer TimerTask::create(TaskHandler const& onTask, std::chrono::milliseconds const& timer)
{
	return UniquePointer(new TimerTask(onTask, timer));
}

TimerTask::TimerTask(TaskHandler const& onTask, std::chrono::milliseconds const& timer)
	: _pImpl(std::make_unique<PImpl>(*this, onTask, timer))
{
	_pImpl->run();
}

TimerTask::~TimerTask()
{
}

} // namespace firmwareUpdater::core::toolkit
