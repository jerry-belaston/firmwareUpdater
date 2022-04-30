#pragma once

#include <QFrame>
#include <QCheckBox>

namespace firmwareUpdater::ui::view::workspace
{

class ModeZone
	: public QFrame
{
	Q_OBJECT

public:
	explicit ModeZone(bool const checked, QWidget* parent = nullptr);
	Q_SIGNAL void modeSwitched(bool isModeMulti);

private:
	QCheckBox _modeCheckBox{ this };
};

} // namespace firmwareUpdater::ui::view::workspace
