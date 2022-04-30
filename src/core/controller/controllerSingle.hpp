#pragma once

#include <memory>
#include "core/common/type.hpp"

namespace firmwareUpdater::core::model {
namespace templateInfo {
	class Server;
}
namespace device {
	class UploadManager;
}
}

namespace firmwareUpdater::core::controller
{

namespace workspace
{
class SingleModeViewInterface;
}

class ControllerSingle
{
public:
	using UniquePointer = std::unique_ptr<ControllerSingle>;

	// Construction
	static UniquePointer create(model::templateInfo::Server& server,
		model::device::UploadManager& uploadManager);
	virtual ~ControllerSingle();

	// View initialization
	void setView(workspace::SingleModeViewInterface& singleModeViewInterface);

	// View callbacks
	void onStartButtonClicked();
	void onDeviceListItemClicked(type::Device::UserName const& deviceUserName);
	void onStepsEnded();
	void onStepsCancelled();

private:
	// Constructor disabled (use create)
	ControllerSingle(model::templateInfo::Server& server,
		model::device::UploadManager& uploadManager);

	friend class Controller;
	void start();
	void stop();

private:
	class PImpl;
	std::unique_ptr<PImpl> _pImpl{ nullptr };
};

} // firmwareUpdater::core::controller
