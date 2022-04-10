#pragma once

#include <Qwidget>
#include <QPixmap>
#include <QLabel>

#include "ui/toolkit/stepperWidget.hpp"
#include "core/common/type.hpp"

namespace firmwareUpdater::ui::view::templateInfo
{

class TemplateStepView
	: public toolkit::StepWidget
{
	Q_OBJECT

public:
	explicit TemplateStepView(core::type::TemplateInfo::StepInfo const& stepInfo, QWidget* parent = nullptr);
	void resizeEvent(QResizeEvent*) override;

private:
	QPixmap _pixmap;
	QLabel _imageLabel{ this };
};

} // namespace firmwareUpdater::ui::view::templateInfo