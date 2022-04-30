#pragma once

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QTimer>

#include "ui/toolkit/stepperWidget.hpp"
#include "core/common/type.hpp"

namespace firmwareUpdater::ui::toolkit
{
	class DataDownloader;
}

namespace firmwareUpdater::ui::view::workspace::templateInfo
{

class TemplateStep
	: public toolkit::StepWidget
{
	Q_OBJECT

public:
	explicit TemplateStep(core::type::TemplateInfo::StepInfo const& stepInfo, toolkit::DataDownloader const& dataDownloader, QWidget* parent = nullptr);
	void startProcessing() override;
	void stopProcessing() override;
	void resizeEvent(QResizeEvent*) override;

private:
	QPixmap _pixmap;
	QLabel _imageLabel{ this };
	QTimer _timer{ this };
};

} // namespace firmwareUpdater::ui::view::workspace::templateInfo
