#pragma once

#include <functional>
#include <memory>
#include <chrono>

namespace firmwareUpdater::core::toolkit
{

using namespace std::chrono_literals;

class TimerTask
{
public:
	using UniquePointer = std::unique_ptr<TimerTask>;
	using TaskHandler = std::function<void()>;

	// Construction
	static UniquePointer create(TaskHandler const& onTask, std::chrono::milliseconds const& timer = 2s);
	virtual ~TimerTask();

private:
	// Constructor disabled (use create)
	TimerTask(TaskHandler const& onTask, std::chrono::milliseconds const& timer);

private:
	class PImpl;
	std::unique_ptr<PImpl> _pImpl{ nullptr };
};

} // firmwareUpdater::core::toolkit
