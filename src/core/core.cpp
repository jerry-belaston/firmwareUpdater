#include "core.hpp"
#include "model/model.hpp"
#include "controller/controller.hpp"

namespace firmwareUpdater::core
{

//////////////////////////////////////////////////////////////////////////////////////////////////
// PImpl
//////////////////////////////////////////////////////////////////////////////////////////////////

class Core::PImpl
{
public:
	PImpl(Core& parent);

	// Members
	Core& _parent;
	model::Model::UniquePointer _model{ model::Model::create() };
	controller::Controller::UniquePointer _controller{ controller::Controller::create(_model->getServer(), _model->getUploadManager()) };
};

Core::PImpl::PImpl(Core& parent)
	: _parent{ parent }
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Core
//////////////////////////////////////////////////////////////////////////////////////////////////

Core::UniquePointer Core::create()
{
	return UniquePointer(new Core());
}

Core::Core()
	: _pImpl(std::make_unique<PImpl>(*this))
{
}

Core::~Core()
{
}

void Core::start()
{
	// Start only the model (controller has already registered to needed models during construction)
	_pImpl->_model->start();
}

model::Model& Core::getModel()
{
	return *_pImpl->_model;
}

controller::Controller& Core::getController()
{
	return *_pImpl->_controller;
}

} // namespace firmwareUpdater::core
