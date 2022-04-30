#pragma once

#include <QFrame>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

#include "core/common/type.hpp"
#include "core/controller/viewInterface.hpp"

namespace firmwareUpdater::ui::view::workspace::modeMulti::projectZone
{

class DeviceWidget
	: public QFrame
{
	Q_OBJECT

public:
	explicit DeviceWidget(QString const& deviceUserName, core::type::TemplateNameList const& templateNameList, 
		QWidget* parent = nullptr);
	void setTemplateNameList(core::type::TemplateNameList const& templateNameList);
	void setUploadButtonState(core::controller::workspace::UploadButtonState const uploadButtonState);
	void setProgress(double const ratio);

public:
	Q_SIGNAL void uploadButtonClicked();
	Q_SIGNAL void templateIndexChanged(int index);

private:
	QLabel _deviceUserNameLabel{ this };
	QComboBox _templatesComboBox{ this };
	QPushButton _uploadButton{ this };
	QProgressBar _progressBar{ this };
};

} // namespace firmwareUpdater::ui::view::workspace::modeMulti::projectZone
