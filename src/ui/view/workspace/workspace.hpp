#pragma once

#include <QFrame>
#include <QWidget>
#include <QStackedWidget>

#include "core/controller/viewInterface.hpp"

#include "modeSingle/modeSingleView.hpp"
#include "modeMulti/multiModeView.hpp"

namespace firmwareUpdater::ui::view::workspace
{

class Workspace 
	: public QFrame
{
	Q_OBJECT

public:
	explicit Workspace(QWidget* parent = nullptr);

	// (here it's a hack, there is no inheritance... just to put it simple) 
	void setController(core::controller::Controller& controller);
	void setPageType(bool const isMulti);

	Q_SIGNAL void modeSwitched(bool isModeMulti);

private:

	// Widgets
	QVBoxLayout _layout{ this };
	QStackedWidget _stackedModeWidget{ this };
	modeSingle::SingleModeView _singleModeWidget{ &_stackedModeWidget };
	modeMulti::MultiModeView _multiModeWidget{ &_stackedModeWidget };

	// Controller
	core::controller::Controller* _controller{ nullptr };
};

} // namespace firmwareUpdater::ui::view::workspace
