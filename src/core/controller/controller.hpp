#pragma once

#include <memory>

namespace firmwareUpdater::core::model::templateInfo
{
class Server;
}
namespace firmwareUpdater::core::model::device
{
	class UploadManager;
}

namespace firmwareUpdater::core::controller
{

class MainWindowViewInterface;
class ControllerSingle;
class ControllerMulti;
class Controller
{
public:
	using UniquePointer = std::unique_ptr<Controller>;

	// Construction
	static UniquePointer create(model::templateInfo::Server& server,
		model::device::UploadManager& uploadManager);
	virtual ~Controller();

	// Sub Controllers
	ControllerSingle& getControllerSingle();
	ControllerMulti& getControllerMulti();

	// View initialization
	void setView(MainWindowViewInterface& mainWindowViewInterface);
	
	void onModeButtonClicked(bool isSingleMode);

private:
	// Constructor disabled (use create)
	Controller(model::templateInfo::Server& server,
		model::device::UploadManager& uploadManager);

private:
	class PImpl;
	std::unique_ptr<PImpl> _pImpl{ nullptr };
};

} // firmwareUpdater::core::controller
