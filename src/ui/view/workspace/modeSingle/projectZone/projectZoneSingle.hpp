#pragma once

#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QComboBox>

#include "core/common/type.hpp"

namespace firmwareUpdater::ui::view::workspace::modeSingle::projectZone
{

class ProjectZoneSingle
	: public QFrame
{
	Q_OBJECT

public:
	explicit ProjectZoneSingle(QWidget* parent = nullptr);

	// ViewInterface overrides
	void resetView();
	void setTemplateNameList(core::type::TemplateNameList const& templateNameList);
	void addDeviceUserName(core::type::Device::UserName const& deviceUserName);
	void removeDeviceUserName(core::type::Device::UserName const& deviceUserName);
	void selectDeviceUserName(core::type::Device::UserName const& /*deviceUserName*/);

public:
	Q_SIGNAL void deviceListItemChanged(std::string const& device);
	Q_SIGNAL void templateIndexChanged(int index);

private:
	// Widgets
	QLabel _deviceLabel{ this };
	QComboBox _deviceComboBox{ this };
	QLabel _templateLabel{ this };
	QComboBox _templateComboBox{ this };
};

} // namespace firmwareUpdater::ui::view::workspace::modeSingle::projectZone
