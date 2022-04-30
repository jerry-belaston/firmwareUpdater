#include "deviceWidget.hpp"

#include <QHBoxLayout>

auto constexpr ProgressMinRange = 0;
auto constexpr ProgressMaxRange = 100;

namespace firmwareUpdater::ui::view::workspace::modeMulti::projectZone
{

DeviceWidget::DeviceWidget(QString const& deviceUserName, core::type::TemplateNameList const& templateNameList,
	QWidget* parent)
	: QFrame{ parent }
{
	auto* layout = new QHBoxLayout{ this };

	// Device User Name Label
	auto metrics = QFontMetrics{ _deviceUserNameLabel.font() };
	QString elidedText = metrics.elidedText(deviceUserName, Qt::ElideRight, _deviceUserNameLabel.width());
	_deviceUserNameLabel.setText(elidedText);
	_deviceUserNameLabel.setObjectName("DeviceWidget-DeviceUserNameLabel");
	layout->addWidget(&_deviceUserNameLabel);

	// Templates combobox
	_templatesComboBox.setObjectName("DeviceWidget-TemplatesComboBox");
	setTemplateNameList(templateNameList);
	layout->addWidget(&_templatesComboBox);

	// Upload Button
	auto const buttonSize = QSize{ 40, 40 };
	_uploadButton.setObjectName("DeviceWidget-UploadButton");
	_uploadButton.setFixedSize(buttonSize);
	_uploadButton.setCheckable(true);
	layout->addWidget(&_uploadButton);

	// Progress Bar
	_progressBar.setObjectName("DeviceWidget-ProgressBar");
	_progressBar.setRange(ProgressMinRange, ProgressMaxRange);
	layout->addWidget(&_progressBar);
	setLayout(layout);

	connect(&_uploadButton, &QPushButton::clicked, this, [this](bool /*checked*/)
	{
		emit uploadButtonClicked();
	});
	connect(&_templatesComboBox, &QComboBox::currentIndexChanged, this, [this](int index)
	{
		if (index > -1)
			emit templateIndexChanged(index);
	});
}

void DeviceWidget::setTemplateNameList(core::type::TemplateNameList const& templateNameList)
{
	_templatesComboBox.clear();
	for (auto const& templateName : templateNameList)
		_templatesComboBox.addItem(QString::fromStdString(templateName));
}

void DeviceWidget::setUploadButtonState(core::controller::workspace::UploadButtonState const uploadButtonState)
{
	_uploadButton.setDisabled(uploadButtonState == core::controller::workspace::UploadButtonState::Disabled);
	_uploadButton.setChecked(uploadButtonState == core::controller::workspace::UploadButtonState::On);
}

void DeviceWidget::setProgress(double const ratio)
{
	_progressBar.setValue(static_cast<int>(ratio * (ProgressMaxRange - ProgressMinRange)));
}

} // namespace firmwareUpdater::ui::view::workspace::modeMulti::projectZone
