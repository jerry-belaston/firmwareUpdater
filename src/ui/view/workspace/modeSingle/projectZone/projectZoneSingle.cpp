#include "projectZoneSingle.hpp"

#include <QVBoxLayout>

namespace firmwareUpdater::ui::view::workspace::modeSingle::projectZone
{

ProjectZoneSingle::ProjectZoneSingle(QWidget* parent)
	: QFrame{ parent }
{
	// Layout
	auto* layout = new QVBoxLayout{ this };
	layout->setContentsMargins(30, 30, 30, 30);


	// Device label & comboBox
	auto* deviceLayout = new QHBoxLayout{ this };
	_deviceLabel.setText("Device:");
	deviceLayout->addWidget(&_deviceLabel, 1);
	deviceLayout->addWidget(&_deviceComboBox, 2);
	layout->addLayout(deviceLayout);

	// Template label & comboBox
	auto* templateLayout = new QHBoxLayout{ this };
	_templateLabel.setText("Template:");
	templateLayout->addWidget(&_templateLabel, 1);
	templateLayout->addWidget(&_templateComboBox, 2);
	layout->addLayout(templateLayout);

	connect(&_deviceComboBox, &QComboBox::currentIndexChanged, this, [this](int /*index*/)
	{
		emit deviceListItemChanged(_deviceComboBox.currentData().toString().toStdString());
	});

	connect(&_templateComboBox, &QComboBox::currentIndexChanged, this, [this](int index)
	{
		if (index > -1)
			emit templateIndexChanged(index);
	});
}

void ProjectZoneSingle::resetView()
{
	QMetaObject::invokeMethod(this, [this]
	{
		QSignalBlocker lock1{ &_templateComboBox };
		_templateComboBox.clear();
		QSignalBlocker lock2{ &_deviceComboBox };
		_deviceComboBox.clear();
	}, Qt::QueuedConnection);
}

void ProjectZoneSingle::setTemplateNameList(core::type::TemplateNameList const& templateNameList)
{
	_templateComboBox.clear();
	for (auto const& templateName : templateNameList)
		_templateComboBox.addItem(QString::fromStdString(templateName));
}

void ProjectZoneSingle::addDeviceUserName(core::type::Device::UserName const& deviceUserName)
{
	_deviceComboBox.addItem(QString::fromStdString(deviceUserName), QString::fromStdString(deviceUserName));
}

void ProjectZoneSingle::removeDeviceUserName(core::type::Device::UserName const& deviceUserName)
{
	if (auto const index = _deviceComboBox.findData(QString::fromStdString(deviceUserName)); index != -1)
	{
		_deviceComboBox.removeItem(index);
	}
}

void ProjectZoneSingle::selectDeviceUserName(core::type::Device::UserName const& deviceUserName)
{
	if (deviceUserName.empty())
		_deviceComboBox.setCurrentIndex(-1);
	else if (auto const index = _deviceComboBox.findData(QString::fromStdString(deviceUserName)); index != -1)
		_deviceComboBox.setCurrentIndex(index);
}

} // namespace firmwareUpdater::ui::view::workspace::modeSingle::projectZone
