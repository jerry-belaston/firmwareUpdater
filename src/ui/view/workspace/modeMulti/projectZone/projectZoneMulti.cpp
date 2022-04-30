#include "projectZoneMulti.hpp"
#include "deviceWidget.hpp"
#include <QVBoxLayout>

namespace firmwareUpdater::ui::view::workspace::modeMulti::projectZone
{

ProjectZoneMulti::ProjectZoneMulti(QWidget* parent)
	: QFrame{ parent }
{
	// Layout
	auto* layout = new QVBoxLayout{ this };
	layout->setContentsMargins(0, 0, 0, 0);

	//addDeviceUserName("Test 3");
	layout->addWidget(&_deviceListWidget);
	connect(&_deviceListWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* item)
	{
		if (item->isSelected())
			emit deviceSelectionChanged(item->data(Qt::UserRole).toString().toStdString());
	});
}

void ProjectZoneMulti::resetView()
{
	_deviceListWidget.clear();
	_deviceUserNameMap.clear();
}

void ProjectZoneMulti::setTemplateNameList(core::type::TemplateNameList const& templateNameList)
{
	_templateNameList = templateNameList;
	for (auto& [_, deviceWidget] : _deviceUserNameMap)
		deviceWidget.second->setTemplateNameList(templateNameList);
}

void ProjectZoneMulti::addDeviceUserName(core::type::Device::UserName const& deviceUserName)
{
	QListWidgetItem* item = new QListWidgetItem{};
	auto* deviceWidget = new DeviceWidget{ QString::fromStdString(deviceUserName), _templateNameList, this };
	item->setSizeHint(deviceWidget->sizeHint());
	item->setData(Qt::UserRole, QString::fromStdString(deviceUserName));
	_deviceListWidget.addItem(item);
	_deviceListWidget.setItemWidget(item, deviceWidget);
	_deviceUserNameMap[deviceUserName] = { item, deviceWidget };
		
	connect(deviceWidget, &DeviceWidget::uploadButtonClicked, this, [this, deviceUserName]()
	{
		emit deviceUploadButtonClicked(deviceUserName);
	});
	connect(deviceWidget, &DeviceWidget::templateIndexChanged, this, [this, deviceUserName](auto index)
	{
		emit deviceTemplateIndexChanged(deviceUserName, index);
	});

	// Row selection
	_deviceListWidget.setCurrentRow(_deviceListWidget.count() == 1 ? 0 : -1);
}

void ProjectZoneMulti::removeDeviceUserName(core::type::Device::UserName const& deviceUserName)
{
	if (auto it = _deviceUserNameMap.find(deviceUserName); it != std::end(_deviceUserNameMap))
	{
		auto const& [item, _] = it->second;
		auto const rowToRemove{ _deviceListWidget.row(item) };
		auto const wasCurrentRow{ rowToRemove == _deviceListWidget.currentRow() };
		_deviceListWidget.takeItem(_deviceListWidget.row(item));
		_deviceUserNameMap.erase(deviceUserName);

		// Row selection
		if(wasCurrentRow)
			_deviceListWidget.setCurrentRow(_deviceListWidget.count() ? 0 : -1);
	}
}

void ProjectZoneMulti::setUploadButtonState(core::type::Device::UserName const& deviceUserName,
	core::controller::workspace::UploadButtonState const uploadButtonState)
{
	if (auto it = _deviceUserNameMap.find(deviceUserName); it != std::end(_deviceUserNameMap))
	{
		auto const& [_, deviceWidget] = it->second;
		deviceWidget->setUploadButtonState(uploadButtonState);
	}
}

void ProjectZoneMulti::setProgress(core::type::Device::UserName const& deviceUserName, double const ratio)
{
	if (auto it = _deviceUserNameMap.find(deviceUserName); it != std::end(_deviceUserNameMap))
	{
		auto const& [_, deviceWidget] = it->second;
		deviceWidget->setProgress(ratio);
	}
}

} // namespace firmwareUpdater::ui::view::workspace::modeMulti::projectZone
