#include "commander.hpp"

#include <QVBoxLayout>

namespace firmwareUpdater::ui::view::workspace::modeSingle::commander
{

Commander::Commander(QWidget* parent)
	: QFrame{ parent }
{
	// Layout
	auto* layout = new QVBoxLayout{ this };
	layout->setContentsMargins(30, 30, 30, 30);
	layout->setAlignment(Qt::AlignHCenter);

	// Start button
	auto const buttonSize = QSize{ 100, 100 };
	_uploadButton.setFixedSize(buttonSize);
	_uploadButton.setCheckable(true);
	layout->addWidget(&_uploadButton);
	connect(&_uploadButton, &QPushButton::clicked, this, [this](bool /*checked*/)
	{
		emit uploadButtonClicked();
	});
}

void Commander::resetView()
{
	_uploadButton.setDisabled(true);
	_uploadButton.setChecked(false);
}

void Commander::setUploadButtonState(core::controller::workspace::UploadButtonState const uploadButtonState)
{
	_uploadButton.setDisabled(uploadButtonState == core::controller::workspace::UploadButtonState::Disabled);
	_uploadButton.setChecked(uploadButtonState == core::controller::workspace::UploadButtonState::On);
}

} // namespace firmwareUpdater::ui::view::workspace::modeSingle::commander
