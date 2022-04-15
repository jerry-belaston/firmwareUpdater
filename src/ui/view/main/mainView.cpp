#include "mainView.hpp"
#include "core/controller/controller.hpp"

namespace firmwareUpdater::ui::view::main
{

MainView::MainView(QWidget* parent)
	: QWidget{ parent }
{
	// Background color
	QPalette p = palette();
	p.setColor(QPalette::Window, Qt::black);
	setPalette(p);

	_stackedLayout.setContentsMargins(0, 0, 0, 0);
	_stackedLayout.setSpacing(0);
	_stackedLayout.addWidget(&_welcomeView);
	_stackedLayout.addWidget(&_templateBrowserView);
	_stackedLayout.addWidget(&_stepperView);

	// Main window properties
	setContentsMargins(0, 0, 0, 0);
}

welcome::WelcomeView* MainView::getWelcomeView()
{
	return &_welcomeView;
}

templateList::TemplateBrowserView* MainView::getTemplateBrowserView()
{
	return &_templateBrowserView;
}

templateInfo::TemplateInfoView* MainView::getTemplateInfoView()
{
	return &_stepperView;
}

void MainView::setController(core::controller::Controller& controller)
{
	_controller = &controller;
}

void MainView::setCurrentView(ViewType const viewType)
{
	QMetaObject::invokeMethod(this, [this, viewType]
	{
		QSignalBlocker lock{ _stackedLayout };
		_stackedLayout.setCurrentIndex(
			viewType == ViewType::Welcome ? 0 :
			viewType == ViewType::TemplateList ? 1 :
			2);
	}, Qt::QueuedConnection);
}

} // namespace firmwareUpdater::ui::view::main
