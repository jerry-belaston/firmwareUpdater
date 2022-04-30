#pragma once

#include <memory>
#include <functional>

#include "core/common/type.hpp"

namespace firmwareUpdater::core::model::priv
{
	class TaskManager;
}

namespace firmwareUpdater::core::model::device
{

class Discovery
{
public:
	using UniquePointer = std::unique_ptr<Discovery>;
	using DeviceListChangeHandler = std::function<void(type::DeviceList const&)>;

	// Construction
	static UniquePointer create(firmwareUpdater::core::model::priv::TaskManager& taskManager);
	virtual ~Discovery();

	void start();
	void setDeviceListChangeHandler(DeviceListChangeHandler const& onDeviceListChanged);

private:
	// Constructor disabled (use create)
	Discovery(firmwareUpdater::core::model::priv::TaskManager& taskManager);

private:
	class PImpl;
	std::unique_ptr<PImpl> _pImpl{ nullptr };
};

} // firmwareUpdater::core::model::device
