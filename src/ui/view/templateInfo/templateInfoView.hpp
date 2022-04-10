#pragma once

#include <Qwidget>
#include "core/controller/viewInterface.hpp"

#include "ui/toolkit/stepperWidget.hpp"

namespace firmwareUpdater::ui::view::templateInfo
{

class TemplateInfoView
	: public QWidget
	, public core::controller::TemplateInfoViewInterface
{
	Q_OBJECT

public:
	explicit TemplateInfoView(QWidget* parent = nullptr);

	// ViewInterface overrides
	void setController(core::controller::Controller& controller) override;
	void setStepInfoList(core::type::TemplateInfo::StepInfoList const& stepInfoList) override;

private:
	// Widgets
	toolkit::StepperWidget _stepperWidget{ this };

	// Controller
	core::controller::Controller* _controller{ nullptr };
};

} // namespace firmwareUpdater::ui::view::templateInfo
