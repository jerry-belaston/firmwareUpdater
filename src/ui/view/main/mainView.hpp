#pragma once

#include <Qwidget>
#include <QStackedLayout>

#include "core/controller/viewInterface.hpp"
#include "ui/view/welcome/welcomeView.hpp"
#include "ui/view/templateBrowser/templateBrowserView.hpp"
#include "ui/view/templateInfo/templateInfoView.hpp"

namespace firmwareUpdater::ui::view::main
{

class MainView 
	: public QWidget
	, public core::controller::MainViewInterface
{
	Q_OBJECT

public:
	explicit MainView(QWidget* parent = nullptr);

	welcome::WelcomeView* getWelcomeView();
	templateList::TemplateBrowserView* getTemplateBrowserView();
	templateInfo::TemplateInfoView* getTemplateInfoView();

	// ViewInterface overrides
	void setController(core::controller::Controller& controller) override;
	void setCurrentView(ViewType const viewType) override;

private:
	// Views
	welcome::WelcomeView _welcomeView{ this };
	templateList::TemplateBrowserView _templateBrowserView{ this };
	templateInfo::TemplateInfoView _stepperView{ this };

	// Widgets
	QStackedLayout _stackedLayout{ this };

	// Controller
	core::controller::Controller* _controller{ nullptr };

};

} // namespace firmwareUpdater::ui::view::main
