#include "controller.hpp"
#include "controllerSingle.hpp"
#include "controllerMulti.hpp"
#include "viewInterface.hpp"
#include "core/model/templateInfo/server.hpp"
#include "core/model/device/uploadManager.hpp"
#include <algorithm>
#include <cassert>

namespace firmwareUpdater::core::controller
{
//////////////////////////////////////////////////////////////////////////////////////////////////
// PImpl
//////////////////////////////////////////////////////////////////////////////////////////////////

class Controller::PImpl
{
public:
	PImpl(Controller& parent, model::templateInfo::Server& server, model::device::UploadManager& uploadManager);
	virtual ~PImpl() = default;

	// Members
	Controller& _parent;
	model::templateInfo::Server& _server;
	model::device::UploadManager& _uploadManager;
	ControllerSingle::UniquePointer _controllerSingle{};
	ControllerMulti::UniquePointer _controllerMulti{};

	// View
	std::unique_ptr<MainWindowViewInterface> _dummyMainWindowView;
	MainWindowViewInterface* _mainWindowView{ nullptr };
};

Controller::PImpl::PImpl(Controller& parent, model::templateInfo::Server& server, model::device::UploadManager& uploadManager)
	: _parent{ parent }
	, _server{ server }
	, _uploadManager{ uploadManager }
	, _controllerSingle{ ControllerSingle::create(server, uploadManager) }
	, _controllerMulti{ ControllerMulti::create(server, uploadManager) }
	, _dummyMainWindowView{ std::make_unique<MainWindowViewInterface>() }
	, _mainWindowView{ _dummyMainWindowView.get() }
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Controller
//////////////////////////////////////////////////////////////////////////////////////////////////

Controller::UniquePointer Controller::create(model::templateInfo::Server& server,
	model::device::UploadManager& uploadManager)
{
	return UniquePointer(new Controller(server, uploadManager));
}

Controller::Controller(model::templateInfo::Server& server,
	model::device::UploadManager& uploadManager)
	: _pImpl(std::make_unique<PImpl>(*this, server, uploadManager))
{
	// Start in single mode
	_pImpl->_controllerSingle->start();
}

Controller::~Controller()
{
}

ControllerSingle& Controller::getControllerSingle()
{
	return *_pImpl->_controllerSingle;
}

ControllerMulti& Controller::getControllerMulti()
{
	return *_pImpl->_controllerMulti;
}

void Controller::setView(MainWindowViewInterface& mainWindowViewInterface)
{
	_pImpl->_mainWindowView = &mainWindowViewInterface;
	_pImpl->_mainWindowView->setController(*this);
}

void Controller::onModeButtonClicked(bool isModeMulti)
{
	// Switch controller
	if (isModeMulti)
	{
		_pImpl->_controllerSingle->stop();
		_pImpl->_controllerMulti->start();
	}
	else
	{
		_pImpl->_controllerMulti->stop();
		_pImpl->_controllerSingle->start();
	}

	// Reset models (will cause them to retrigger everything to registered controllers)
	_pImpl->_uploadManager.reset();
	_pImpl->_server.reset();

	// Change view mode
	auto const shouldExpand{ isModeMulti };
	auto const shouldDisplayToolbar{ isModeMulti };
	_pImpl->_mainWindowView->setPageType(
		isModeMulti ? PageType::MultiModeWorkspace : PageType::SingleModeWorkspace, 
		shouldExpand, shouldDisplayToolbar);
}

} // namespace firmwareUpdater::core::controller
