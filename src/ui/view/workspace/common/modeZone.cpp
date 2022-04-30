#include "modeZone.hpp"

#include <QVBoxLayout>

namespace firmwareUpdater::ui::view::workspace
{

ModeZone::ModeZone(bool const checked, QWidget* parent)
	: QFrame{ parent }
{
	// Layout
	auto* layout = new QVBoxLayout{ this };
	layout->setAlignment(Qt::AlignHCenter);

	// Start button
	_modeCheckBox.setChecked(checked);
	_modeCheckBox.setText(checked ? "Multi" : "Single");
	_modeCheckBox.setLayoutDirection(checked ? Qt::RightToLeft : Qt::LeftToRight);
	layout->addWidget(&_modeCheckBox);

	connect(&_modeCheckBox, &QCheckBox::toggled, this, [this](bool checked)
	{
		{
			QSignalBlocker lock{ &_modeCheckBox };
			_modeCheckBox.setChecked(!checked);
		}
		emit modeSwitched(checked);
	});
}

} // namespace firmwareUpdater::ui::view::workspace::commander
