#include "controller.hpp"

#include <algorithm>
#include <cassert>

#include "viewInterface.hpp"

namespace firmwareUpdater::core::controller
{
//////////////////////////////////////////////////////////////////////////////////////////////////
// PImpl
//////////////////////////////////////////////////////////////////////////////////////////////////

class Controller::PImpl
{
public:
	PImpl(Controller& parent);
	virtual ~PImpl() = default;
	void setCurrentView(MainViewInterface::ViewType const viewType);

	static type::TemplateNameList toTemplateNameList(type::TemplateInfoList const& templateInfoList);

	// Members
	Controller& _parent;

	// Model
	type::TemplateInfoList _templateInfoList;
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
};

Controller::PImpl::PImpl(Controller& parent)
	: _parent{ parent }
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

type::TemplateNameList Controller::PImpl::toTemplateNameList(type::TemplateInfoList const& templateInfoList)
{
	auto templateNameList = type::TemplateNameList(templateInfoList.size());
	for (auto i = 0u; i < templateInfoList.size(); ++i)
		templateNameList[i] = templateInfoList[i].name;
	return templateNameList;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Controller
//////////////////////////////////////////////////////////////////////////////////////////////////

Controller::UniquePointer Controller::create()
{
	return UniquePointer(new Controller());
}

Controller::Controller()
	: _pImpl(std::make_unique<PImpl>(*this))
{
	// TO REMOVE: Fake template info list just for test
	_pImpl->_templateInfoList = type::TemplateInfoList(3, {
		.name = "Template",
		.stepInfoList = {
			{
				.imageUrl = "https://d3kqkuy1hpjocx.cloudfront.net/s3fs-public/styles/thumbnail/public/pictures/picture-134773-1541674067.jpg",
					.descriptionText = "First step",
					.descriptionColor = "black", // Format #RRGGBB or standard color name text
					.descriptionSize = 8, // in pt
					.previousButtonText = "Previous",
					.previousButtonDisplayed = true,
					.nextButtonText = "Next",
			},
			{
				.imageUrl = "https://upload.wikimedia.org/wikipedia/commons/5/54/Light_shining1.JPG",
				.descriptionText = "Second step which must be used with attention",
				.descriptionColor = "blue", // Format #RRGGBB or standard color name text
				.descriptionSize = 20, // in pt
				.previousButtonText = "Previous",
				.previousButtonDisplayed = true,
				.nextButtonText = "Next",
			},
			{
				.imageUrl = "https://www.webstickersmuraux.com/fr/img/asmu071-png/folder/products-detalle-png/autocollants-homer-simpson-reggae-.png",
				.descriptionText = "Last and funny step.",
				.descriptionColor = "red", // Format #RRGGBB or standard color name text
				.descriptionSize = 8, // in pt
				.previousButtonText = "Previous",
				.previousButtonDisplayed = false,
				.nextButtonText = "Next",
			},
		}
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
	_pImpl->_templateBrowserView->setTemplateNameList(_pImpl->toTemplateNameList(_pImpl->_templateInfoList));
}


void Controller::setView(TemplateInfoViewInterface& templateInfoViewInterface)
{
	_pImpl->_templateInfoView = &templateInfoViewInterface;
	_pImpl->_templateInfoView->setController(*this);
	_pImpl->_templateInfoView->setStepInfoList({});
}

void Controller::onStartButtonClicked()
{
	_pImpl->setCurrentView(MainViewInterface::ViewType::TemplateList);
}

void Controller::onListItemClicked(std::uint32_t const itemIndex)
{
	assert(itemIndex < _pImpl->_templateInfoList.size());
	_pImpl->_templateInfoView->setStepInfoList(_pImpl->_templateInfoList[itemIndex].stepInfoList);
	_pImpl->setCurrentView(MainViewInterface::ViewType::Stepper);
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
