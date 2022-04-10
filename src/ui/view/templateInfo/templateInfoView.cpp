#include "templateInfoView.hpp"
#include "templateStepView.hpp"

#include <QVBoxLayout>

namespace firmwareUpdater::ui::view::templateInfo
{

TemplateInfoView::TemplateInfoView(QWidget* parent)
	: QWidget{ parent }
{
	// Layout
	auto* layout = new QVBoxLayout{ this };
	layout->setContentsMargins(30, 30, 30, 30);

	layout->addWidget(&_stepperWidget);
	connect(&_stepperWidget, &toolkit::StepperWidget::ended, this, [this]
	{
		if (_controller)
			_controller->onStepsEnded();
	});

	connect(&_stepperWidget, &toolkit::StepperWidget::cancelled, this, [this]
	{
		if (_controller)
			_controller->onStepsCancelled();
	});
}

void TemplateInfoView::setController(core::controller::Controller& controller)
{
	_controller = &controller;
}

void TemplateInfoView::setStepInfoList(core::type::TemplateInfo::StepInfoList const& stepInfoList)
{
	QMetaObject::invokeMethod(this, [this, stepInfoList]
	{
		_stepperWidget.clear();
		for (auto const& stepInfo : stepInfoList)
			_stepperWidget.addStep(new TemplateStepView{ stepInfo, this });
	}, Qt::QueuedConnection);

}

} // namespace firmwareUpdater::ui::view::templateInfo