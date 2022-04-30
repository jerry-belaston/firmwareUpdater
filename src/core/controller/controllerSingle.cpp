#include "controllerSingle.hpp"
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

class ControllerSingle::PImpl: public model::device::UploadManager::Delegate
{
public:
	PImpl(ControllerSingle& parent, model::templateInfo::Server& server,
		model::device::UploadManager& uploadManager);
	virtual ~PImpl() = default;
	void setCurrentView(workspace::SingleModeViewInterface::ViewType const currentViewType);
	void updateCommanderButtonUpload();
	void updateSelectedDevice(bool const shouldReset = false);

	// Members
	ControllerSingle& _parent;

	// Model
	model::templateInfo::Server& _server;
	model::device::UploadManager& _uploadManager;

	// Cache
	type::TemplateInfoList _templateInfoList;
	std::uint32_t _currentTemplateIndex{ 0 };
	workspace::SingleModeViewInterface::ViewType _currentViewType{ workspace::SingleModeViewInterface::ViewType::ProjectZoneSingle };
	type::Device::UserName _currentDeviceUserName;

	// View
	std::unique_ptr<workspace::SingleModeViewInterface> _dummySingleModeView;
	workspace::SingleModeViewInterface* _singleModeView{ nullptr };

public:
	// callbacks
	void onTemplateInfoListChanged(type::TemplateInfoList const& templateInfoList);

private:
	void onUploadAdded(model::device::Upload const& /*upload*/) override;
	void onUploadRemoved(model::device::UploadManager::Key const& /*key*/) override;
	void onUploadChanged(model::device::Upload const& /*upload*/) override;
};

ControllerSingle::PImpl::PImpl(ControllerSingle& parent, model::templateInfo::Server& server,
	model::device::UploadManager& uploadManager)
	: _parent{ parent }
	, _server{ server }
	, _uploadManager{ uploadManager }
	, _dummySingleModeView{ std::make_unique<workspace::SingleModeViewInterface>() }
	, _singleModeView{ _dummySingleModeView.get() }
{
}

void ControllerSingle::PImpl::setCurrentView(workspace::SingleModeViewInterface::ViewType const currentViewType)
{
	// Force reset all upload state
	if (currentViewType == workspace::SingleModeViewInterface::ViewType::ProjectZoneSingle)
	{
		for (auto const& key : _uploadManager.getKeyList())
			_uploadManager.setUploadState(key, false);
	}
	_currentViewType = currentViewType;
	_singleModeView->setCurrentView(currentViewType);
}

void ControllerSingle::PImpl::updateCommanderButtonUpload()
{
	_singleModeView->setUploadButtonState(
		_templateInfoList.size() && _uploadManager.getKeyList().size() && !_currentDeviceUserName.empty() ?
		workspace::UploadButtonState::Off :
		workspace::UploadButtonState::Disabled);
}

void ControllerSingle::PImpl::updateSelectedDevice(bool const shouldReset)
{
	if (shouldReset)
		_currentDeviceUserName = _uploadManager.getKeyList().size() ? _uploadManager.getKeyList().front() : "";
	if (_currentDeviceUserName.empty() && _uploadManager.getKeyList().size())
		_currentDeviceUserName = _uploadManager.getKeyList().front();	
	_singleModeView->selectDeviceUserName(_currentDeviceUserName);
}

void ControllerSingle::PImpl::onTemplateInfoListChanged(type::TemplateInfoList const& templateInfoList)
{
	_templateInfoList = templateInfoList;
	_singleModeView->setTemplateInfoList(templateInfoList);

	// Force template list view
	updateCommanderButtonUpload();
	setCurrentView(workspace::SingleModeViewInterface::ViewType::ProjectZoneSingle);
}

void ControllerSingle::PImpl::onUploadAdded(model::device::Upload const& upload)
{
	_singleModeView->addDeviceUserName(upload.device.userName);
	updateSelectedDevice();
	updateCommanderButtonUpload();
}

void ControllerSingle::PImpl::onUploadRemoved(model::device::UploadManager::Key const& key)
{
	auto const shouldResetSelected{ _currentDeviceUserName == key };
	_singleModeView->removeDeviceUserName(key);
	updateSelectedDevice(shouldResetSelected);
	updateCommanderButtonUpload();
	setCurrentView(workspace::SingleModeViewInterface::ViewType::ProjectZoneSingle);
}

void ControllerSingle::PImpl::onUploadChanged(model::device::Upload const& /*key*/)
{
	setCurrentView(workspace::SingleModeViewInterface::ViewType::ProjectZoneSingle);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ControllerSingle
//////////////////////////////////////////////////////////////////////////////////////////////////

ControllerSingle::UniquePointer ControllerSingle::create(model::templateInfo::Server& server,
	model::device::UploadManager& uploadManager)
{
	return UniquePointer(new ControllerSingle(server, uploadManager));
}

ControllerSingle::ControllerSingle(model::templateInfo::Server& server,
	model::device::UploadManager& uploadManager)
	: _pImpl(std::make_unique<PImpl>(*this, server, uploadManager))
{
}

ControllerSingle::~ControllerSingle()
{
}

void ControllerSingle::setView(workspace::SingleModeViewInterface& singleModeViewInterface)
{
	_pImpl->_singleModeView = &singleModeViewInterface;
	_pImpl->_singleModeView->setController(*this);
	_pImpl->_singleModeView->setTemplateInfoList(_pImpl->_templateInfoList);

	// Display default view
	_pImpl->setCurrentView(workspace::SingleModeViewInterface::ViewType::ProjectZoneSingle);

	_pImpl->updateCommanderButtonUpload();
}

void ControllerSingle::onStartButtonClicked()
{
	_pImpl->_uploadManager.setUploadTemplate(_pImpl->_currentDeviceUserName, _pImpl->_currentTemplateIndex);
	_pImpl->_uploadManager.setUploadState(_pImpl->_currentDeviceUserName, true);
	_pImpl->_singleModeView->startProcessing();
	_pImpl->setCurrentView(workspace::SingleModeViewInterface::ViewType::TemplateInfo);
}

void ControllerSingle::onDeviceListItemClicked(type::Device::UserName const& deviceUserName)
{
	_pImpl->_currentDeviceUserName = deviceUserName;
}

void ControllerSingle::onStepsEnded()
{
	_pImpl->_uploadManager.setUploadState(_pImpl->_currentDeviceUserName, false);
	_pImpl->setCurrentView(workspace::SingleModeViewInterface::ViewType::ProjectZoneSingle);
	_pImpl->updateCommanderButtonUpload();
}

void ControllerSingle::onStepsCancelled()
{
	_pImpl->_uploadManager.setUploadState(_pImpl->_currentDeviceUserName, false);
	_pImpl->setCurrentView(workspace::SingleModeViewInterface::ViewType::ProjectZoneSingle);
	_pImpl->updateCommanderButtonUpload();
}

void ControllerSingle::start()
{
	// Register to models
	_pImpl->_server.setTemplateInfoListChangeHandler([this](type::TemplateInfoList const& templateInfoList)
	{
		_pImpl->onTemplateInfoListChanged(templateInfoList);
	});
	_pImpl->_uploadManager.setDelegate(_pImpl.get());
}

void ControllerSingle::stop()
{
	// Reset cache members
	_pImpl->_templateInfoList = {};
	_pImpl->_currentTemplateIndex = 0;
	_pImpl->_currentViewType = workspace::SingleModeViewInterface::ViewType::ProjectZoneSingle;
	_pImpl->_currentDeviceUserName = {};

	// Unregister from models notifications
	_pImpl->_server.setTemplateInfoListChangeHandler(nullptr);
	_pImpl->_uploadManager.setDelegate(nullptr);

	// Clean UI
	_pImpl->_singleModeView->resetView();
}

} // namespace firmwareUpdater::core::controller
