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
class MultiModeViewInterface;
class ProjectZoneMultiViewInterface;
class TemplateInfoMultiViewInterface;
}

class ControllerMulti
{
public:
	using UniquePointer = std::unique_ptr<ControllerMulti>;

	// Construction
	static UniquePointer create(model::templateInfo::Server& server,
		model::device::UploadManager& uploadManager);
	virtual ~ControllerMulti();

	// View initialization
	void setView(workspace::MultiModeViewInterface& singleModeViewInterface);

	// View callbacks
	void onTemplateIndexChanged(type::Device::UserName const& deviceUserName, std::uint32_t const index);
	void onStartButtonClicked(type::Device::UserName const& deviceUserName);
	void onStepsProgressed(type::Device::UserName const& deviceUserName, double const ratio);
	void onStepsEnded(type::Device::UserName const& deviceUserName);
	void onStepsCancelled(type::Device::UserName const& deviceUserName);
private:
	// Constructor disabled (use create)
	ControllerMulti(model::templateInfo::Server& server,
		model::device::UploadManager& uploadManager);

	friend class Controller;
	void start();
	void stop();

private:
	class PImpl;
	std::unique_ptr<PImpl> _pImpl{ nullptr };
};

} // firmwareUpdater::core::controller
