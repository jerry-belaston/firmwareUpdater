#include "model.hpp"
#include "private/taskManager.hpp"
#include "templateInfo/server.hpp"
#include "device/discovery.hpp"
#include "device/uploadManager.hpp"

namespace firmwareUpdater::core::model
{

//////////////////////////////////////////////////////////////////////////////////////////////////
// PImpl
//////////////////////////////////////////////////////////////////////////////////////////////////

class Model::PImpl
{
public:
	PImpl(Model& parent);

	// Members
	Model& _parent;
	priv::TaskManager::UniquePointer _taskManager{ priv::TaskManager::create() };
	templateInfo::Server::UniquePointer _server{ templateInfo::Server::create(*_taskManager) };
	device::Discovery::UniquePointer _discovery{ device::Discovery::create(*_taskManager) };
	device::UploadManager::UniquePointer _uploadManager{ device::UploadManager::create(*_discovery) };
};

Model::PImpl::PImpl(Model& parent)
	: _parent{ parent }
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Model
//////////////////////////////////////////////////////////////////////////////////////////////////

Model::UniquePointer Model::create()
{
	return UniquePointer(new Model());
}

Model::Model()
	: _pImpl(std::make_unique<PImpl>(*this))
{
}

Model::~Model()
{
}

void Model::start()
{
	_pImpl->_server->start();
	_pImpl->_discovery->start();
	_pImpl->_taskManager->start();
}

templateInfo::Server& Model::getServer()
{
	return *_pImpl->_server;
}

device::Discovery& Model::getDiscovery()
{
	return *_pImpl->_discovery;
}

device::UploadManager& Model::getUploadManager()
{
	return *_pImpl->_uploadManager;
}

} // namespace firmwareUpdater::core::model
