#pragma once

#include <QFrame>
#include <QWidget>
#include <QListWidget>
#include <map>
#include <string>

#include "core/common/type.hpp"
#include "deviceWidget.hpp"

namespace firmwareUpdater::ui::view::workspace::modeMulti::projectZone
{

class ProjectZoneMulti
	: public QFrame
{
	Q_OBJECT

public:
	explicit ProjectZoneMulti(QWidget* parent = nullptr);

	// ViewInterface overrides
	void resetView();
	void setTemplateNameList(core::type::TemplateNameList const& templateNameList);
	void addDeviceUserName(core::type::Device::UserName const& deviceUserName);
	void removeDeviceUserName(core::type::Device::UserName const& deviceUserName);
	void setUploadButtonState(core::type::Device::UserName const& deviceUserName,
		core::controller::workspace::UploadButtonState const uploadButtonState);
	void setProgress(core::type::Device::UserName const& deviceUserName, double const ratio);

public:
	Q_SIGNAL void deviceUploadButtonClicked(std::string deviceUserName);
	Q_SIGNAL void deviceSelectionChanged(std::string deviceUserName);
	Q_SIGNAL void deviceTemplateIndexChanged(std::string deviceUserName, int index);

private:
	// Widgets
	QListWidget _deviceListWidget{ this };
	std::map<core::type::Device::UserName, std::pair<QListWidgetItem*, DeviceWidget*>> _deviceUserNameMap{};

	// Cache
	core::type::TemplateNameList _templateNameList{};
};

} // namespace firmwareUpdater::ui::view::workspace::modeMulti::projectZone
