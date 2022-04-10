#include "welcomeView.hpp"

#include <QVBoxLayout>

namespace firmwareUpdater::ui::view::welcome
{

WelcomeView::WelcomeView(QWidget* parent)
	: QWidget{ parent }
{
	// Layout
	auto* layout = new QVBoxLayout{ this };
	layout->setContentsMargins(30, 0, 30, 0);

	// Start button
	_startButton.setStyleSheet(R"(
		QPushButton {
			min-height: 50px;
			font: 13pt bold;
			background-color: rgba(0, 162, 255, 255);
		}
		QPushButton:hover{
			background-color: rgba(0, 136, 217, 255);
		}
	)");
	layout->addWidget(&_startButton);
	connect(&_startButton, &QPushButton::clicked, this, [this](bool /*checked*/)
	{
		if (_controller)
			_controller->onStartButtonClicked();
	});
}

void WelcomeView::setController(core::controller::Controller& controller)
{
	_controller = &controller;
}

void WelcomeView::setButtonEnabled(bool isEnabled)
{
	QMetaObject::invokeMethod(this, [this, isEnabled]
		{
			_startButton.setEnabled(isEnabled);
		}, Qt::QueuedConnection);
}

} // namespace firmwareUpdater::ui::view::welcome
