#include "controller.hpp"
#include "viewInterface.hpp"
#include "core/model/server.hpp"
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
	PImpl(Controller& parent, model::Server& server);
	virtual ~PImpl() = default;
	void setCurrentView(MainViewInterface::ViewType const viewType);
	void updateTemplateInfoListCache(type::TemplateInfoList const& templateInfoList);

	// Members
	Controller& _parent;

	// Model
	model::Server& _server;
	type::TemplateInfoList _templateInfoList;
	type::TemplateNameList _templateNameList;
	MainViewInterface::ViewType _currentViewType{ MainViewInterface::ViewType::Welcome };

	// View
	std::unique_ptr<MainViewInterface> _dummyMainView;
	MainViewInterface* _mainView{ nullptr };
	std::unique_ptr<WelcomeViewInterface> _dummyWelcomeView;
	WelcomeViewInterface* _welcomeView{ nullptr };
	std::unique_ptr<TemplateBrowserViewInterface> _dummyTemplateBrowserView;
	TemplateBrowserViewInterface* _templateBrowserView{ nullptr };
	std::unique_ptr<TemplateInfoViewInterface> _dummyTemplateInfoView;
	TemplateInfoViewInterface* _templateInfoView{ nullptr };

public:
	// server callback
	void onTemplateInfoListChanged(type::TemplateInfoList const& templateInfoList);
};

Controller::PImpl::PImpl(Controller& parent, model::Server& server)
	: _parent{ parent }
	, _server{ server }
	, _dummyMainView{ std::make_unique<MainViewInterface>() }
	, _mainView{ _dummyMainView.get() }
	, _dummyWelcomeView{ std::make_unique<WelcomeViewInterface>() }
	, _welcomeView{ _dummyWelcomeView.get() }
	, _dummyTemplateBrowserView{ std::make_unique<TemplateBrowserViewInterface>() }
	, _templateBrowserView{ _dummyTemplateBrowserView.get() }
	, _dummyTemplateInfoView{ std::make_unique<TemplateInfoViewInterface>() }
	, _templateInfoView{ _dummyTemplateInfoView.get() }

{
}

void Controller::PImpl::setCurrentView(MainViewInterface::ViewType const currentViewType)
{
	_currentViewType = currentViewType;
	_mainView->setCurrentView(currentViewType);
}

void Controller::PImpl::updateTemplateInfoListCache(type::TemplateInfoList const& templateInfoList)
{
	_templateInfoList = templateInfoList;
	_templateNameList.clear();
	for (auto const& templateInfo : templateInfoList)
	{
		_templateNameList.push_back(templateInfo.name);
	}
}

void Controller::PImpl::onTemplateInfoListChanged(type::TemplateInfoList const& templateInfoList)
{
	updateTemplateInfoListCache(templateInfoList);
	_templateBrowserView->setTemplateNameList(_templateNameList);
	_templateInfoView->prepareImageCache(_templateInfoList);
	// Force template list view
	if(templateInfoList.empty())
	{
		_welcomeView->setButtonEnabled(false);
		setCurrentView(MainViewInterface::ViewType::Welcome);
	}
	else
	{
		_welcomeView->setButtonEnabled(true);
		if (_currentViewType != MainViewInterface::ViewType::Welcome)
			setCurrentView(MainViewInterface::ViewType::TemplateList);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Controller
//////////////////////////////////////////////////////////////////////////////////////////////////

Controller::UniquePointer Controller::create(model::Server& server)
{
	return UniquePointer(new Controller(server));
}

Controller::Controller(model::Server& server)
	: _pImpl(std::make_unique<PImpl>(*this, server))
{
	_pImpl->_server.setTemplateInfoListChangeHandler([this] (type::TemplateInfoList const& templateInfoList)
		{ 
			_pImpl->onTemplateInfoListChanged(templateInfoList);
		});
}

Controller::~Controller()
{
}

void Controller::setView(MainViewInterface& mainViewInterface)
{
	_pImpl->_mainView = &mainViewInterface;
	_pImpl->_mainView->setController(*this);

	// Display default view
	_pImpl->setCurrentView(MainViewInterface::ViewType::Welcome);
}

void Controller::setView(WelcomeViewInterface& welcomeViewInterface)
{
	_pImpl->_welcomeView = &welcomeViewInterface;
	_pImpl->_welcomeView->setController(*this);
	_pImpl->_welcomeView->setButtonEnabled(_pImpl->_templateInfoList.size());
}

void Controller::setView(TemplateBrowserViewInterface& templateBrowserViewInterface)
{
	_pImpl->_templateBrowserView = &templateBrowserViewInterface;
	_pImpl->_templateBrowserView->setController(*this);
	_pImpl->_templateBrowserView->setTemplateNameList(_pImpl->_templateNameList);
}


void Controller::setView(TemplateInfoViewInterface& templateInfoViewInterface)
{
	_pImpl->_templateInfoView = &templateInfoViewInterface;
	_pImpl->_templateInfoView->setController(*this);
	_pImpl->_templateInfoView->setTemplateInfo(0, {});
}

void Controller::onStartButtonClicked()
{
	_pImpl->setCurrentView(MainViewInterface::ViewType::TemplateList);
}

void Controller::onListItemClicked(std::uint32_t const itemIndex)
{
	assert(itemIndex < _pImpl->_templateInfoList.size());
	_pImpl->_templateInfoView->setTemplateInfo(itemIndex, _pImpl->_templateInfoList[itemIndex]);
	_pImpl->setCurrentView(MainViewInterface::ViewType::TemplateInfo);
}

void Controller::onStepsEnded()
{
	_pImpl->setCurrentView(MainViewInterface::ViewType::TemplateList);
}

void Controller::onStepsCancelled()
{
	_pImpl->setCurrentView(MainViewInterface::ViewType::TemplateList);
}

} // namespace firmwareUpdater::core::controller
