#include "controllerMulti.hpp"
#include "viewInterface.hpp"
#include "core/model/templateInfo/server.hpp"
#include "core/model/device/uploadManager.hpp"
#include <algorithm>
#include <cassert>
#include <map>

namespace firmwareUpdater::core::controller
{

//////////////////////////////////////////////////////////////////////////////////////////////////
// PImpl
//////////////////////////////////////////////////////////////////////////////////////////////////

class ControllerMulti::PImpl: public model::device::UploadManager::Delegate
{
public:
	PImpl(ControllerMulti& parent, model::templateInfo::Server& server,
		model::device::UploadManager& uploadManager);
	virtual ~PImpl() = default;

	// Members
	ControllerMulti& _parent;

	// Model
	model::templateInfo::Server& _server;
	model::device::UploadManager& _uploadManager;

	// Cache
	type::TemplateInfoList _templateInfoList;
	std::map<model::device::UploadManager::Key, model::device::Upload> _uploads;

	// View
	std::unique_ptr<workspace::MultiModeViewInterface> _dummyMultiModeView;
	workspace::MultiModeViewInterface* _multiModeView{ nullptr };

public:
	// callbacks
	void onTemplateInfoListChanged(type::TemplateInfoList const& templateInfoList);

private:
	void onUploadAdded(model::device::Upload const& /*upload*/) override;
	void onUploadRemoved(model::device::UploadManager::Key const& /*key*/) override;
	void onUploadChanged(model::device::Upload const& /*upload*/) override;
};

ControllerMulti::PImpl::PImpl(ControllerMulti& parent, model::templateInfo::Server& server,
	model::device::UploadManager& uploadManager)
	: _parent{ parent }
	, _server{ server }
	, _uploadManager{ uploadManager }
	, _dummyMultiModeView{ std::make_unique<workspace::MultiModeViewInterface>() }
	, _multiModeView{ _dummyMultiModeView.get() }
{
}

void ControllerMulti::PImpl::onTemplateInfoListChanged(type::TemplateInfoList const& templateInfoList)
{
	_templateInfoList = templateInfoList;
	_multiModeView->setTemplateInfoList(_templateInfoList);
}

void ControllerMulti::PImpl::onUploadAdded(model::device::Upload const& upload)
{
	_uploads[upload.device.userName] = upload;
	_multiModeView->addDeviceUserName(upload.device.userName);
}

void ControllerMulti::PImpl::onUploadRemoved(model::device::UploadManager::Key const& key)
{
	_multiModeView->removeDeviceUserName(key);
	_uploads.erase(key);
}

void ControllerMulti::PImpl::onUploadChanged(model::device::Upload const& upload)
{
	_multiModeView->stopProcessing(upload.device.userName);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ControllerMulti
//////////////////////////////////////////////////////////////////////////////////////////////////

ControllerMulti::UniquePointer ControllerMulti::create(model::templateInfo::Server& server,
	model::device::UploadManager& uploadManager)
{
	return UniquePointer(new ControllerMulti(server, uploadManager));
}

ControllerMulti::ControllerMulti(model::templateInfo::Server& server,
	model::device::UploadManager& uploadManager)
	: _pImpl(std::make_unique<PImpl>(*this, server, uploadManager))
{
}

ControllerMulti::~ControllerMulti()
{
}

void ControllerMulti::setView(workspace::MultiModeViewInterface& singleModeViewInterface)
{
	_pImpl->_multiModeView = &singleModeViewInterface;
	_pImpl->_multiModeView->setController(*this);
}

void ControllerMulti::onTemplateIndexChanged(type::Device::UserName const& deviceUserName, std::uint32_t const /*index*/)
{
	// Only stop processing (the update of the sub view is done by MultiModeView)
	_pImpl->_multiModeView->stopProcessing(deviceUserName);
}

void ControllerMulti::onStartButtonClicked(type::Device::UserName const& deviceUserName)
{
	_pImpl->_uploadManager.setUploadState(deviceUserName, true);
	_pImpl->_multiModeView->setProgress(deviceUserName, 0.0);
	_pImpl->_multiModeView->startProcessing(deviceUserName);
}

void ControllerMulti::onStepsProgressed(type::Device::UserName const& deviceUserName, double const ratio)
{
	_pImpl->_multiModeView->setProgress(deviceUserName, ratio);
}

void ControllerMulti::onStepsEnded(type::Device::UserName const& deviceUserName)
{
	_pImpl->_uploadManager.setUploadState(deviceUserName, false);
	_pImpl->_multiModeView->setProgress(deviceUserName, 1.0);
	_pImpl->_multiModeView->setUploadButtonState(deviceUserName, workspace::UploadButtonState::Off);
}

void ControllerMulti::onStepsCancelled(type::Device::UserName const& deviceUserName)
{
	_pImpl->_uploadManager.setUploadState(deviceUserName, false);
	_pImpl->_multiModeView->setProgress(deviceUserName, 0.0);
	_pImpl->_multiModeView->setUploadButtonState(deviceUserName, workspace::UploadButtonState::Off);
}

void ControllerMulti::start()
{
	// Register to models
	_pImpl->_server.setTemplateInfoListChangeHandler([this](type::TemplateInfoList const& templateInfoList)
	{
		_pImpl->onTemplateInfoListChanged(templateInfoList);
	});
	_pImpl->_uploadManager.setDelegate(_pImpl.get());
}

void ControllerMulti::stop()
{
	// Reset members

	// Unregister from models notifications
	_pImpl->_server.setTemplateInfoListChangeHandler(nullptr);
	_pImpl->_uploadManager.setDelegate(nullptr);

	// Clean UI
	_pImpl->_multiModeView->resetView();
}

} // namespace firmwareUpdater::core::controller
