#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QToolBar>

#include "core/controller/viewInterface.hpp"
#include "ui/view/workspace/workspace.hpp"

namespace firmwareUpdater::ui::view::mainWindow
{

class MainWindowView
	: public QMainWindow
	, public core::controller::MainWindowViewInterface
{
	Q_OBJECT

public:
	explicit MainWindowView(QWidget* parent = nullptr);

	// QWidget overrides
	void keyPressEvent(QKeyEvent* event) override;

	// ViewInterface overrides
	void setController(core::controller::Controller& controller) override;
	void setPageType(core::controller::PageType const& /*pageType*/,
		bool const /*shouldExpand*/, bool const /*shouldDisplayToolbar*/) override;

private:
	// Widgets
	QStackedWidget _stackedWidget{ this };
	QToolBar _toolbar{ this };

	// Views
	workspace::Workspace _workspace{ &_stackedWidget };

	// Controller
	core::controller::Controller* _controller{ nullptr };
};

} // namespace firmwareUpdater::ui::view::mainWindow
