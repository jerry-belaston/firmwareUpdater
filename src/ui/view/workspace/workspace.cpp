#include "workspace.hpp"

#include <QVBoxLayout>

#include "core/controller/controller.hpp"
#include "core/controller/controllerSingle.hpp"
#include "core/controller/controllerMulti.hpp"

namespace firmwareUpdater::ui::view::workspace
{

Workspace::Workspace(QWidget* parent)
	: QFrame{ parent }
{	
	// Main layout
	_layout.setContentsMargins(0, 0, 0, 0);
	_layout.setSpacing(0);

	// Stacked layout
	_stackedModeWidget.setContentsMargins(0, 0, 0, 0);
	_stackedModeWidget.addWidget(&_singleModeWidget);
	_stackedModeWidget.addWidget(&_multiModeWidget);
	_layout.addWidget(&_stackedModeWidget);

	// Main window properties
	setContentsMargins(0, 0, 0, 0);

	connect(&_singleModeWidget, &modeSingle::SingleModeView::modeSwitched, this, &Workspace::modeSwitched);
	connect(&_multiModeWidget, &modeMulti::MultiModeView::modeSwitched, this, &Workspace::modeSwitched);
}

void Workspace::setController(core::controller::Controller& controller)
{
	_controller = &controller;
	_controller->getControllerSingle().setView(_singleModeWidget);
	_controller->getControllerMulti().setView(_multiModeWidget);
}

void Workspace::setPageType(bool const isMulti)
{
	QMetaObject::invokeMethod(this, [this, isMulti]
	{
		QSignalBlocker lock{ &_stackedModeWidget };
		_stackedModeWidget.setCurrentIndex(isMulti ? 1 : 0);
	}, Qt::QueuedConnection);
}

} // namespace firmwareUpdater::ui::view::workspace
