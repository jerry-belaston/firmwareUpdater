#include "templateInfo.hpp"

#include <QVBoxLayout>

#include "templateStep.hpp"	
#include "ui/toolkit/dataDownloader.hpp"
#include "ui/view/workspace/common/imageCacheManager.hpp"

namespace firmwareUpdater::ui::view::workspace::templateInfo
{

TemplateInfo::TemplateInfo(ImageCacheManager const& imageCacheManager, QWidget* parent)
	: QFrame{ parent }
	, _imageCacheManager{ imageCacheManager }
{
	auto* layout = new QVBoxLayout{ this };
	layout->setContentsMargins(0, 0, 0, 0);

	// Empty label
	_emptyLabel.setText("No Template available");
	_emptyLabel.setAlignment(Qt::AlignCenter);
	_emptyLabel.setObjectName("TemplateInfo-EmptyLabel");
	_stackedWidget.addWidget(&_emptyLabel);
	_stackedWidget.addWidget(&_stepperWidget);
	_stackedWidget.setCurrentIndex(0);

	layout->addWidget(&_stackedWidget);

	connect(&_stepperWidget, &toolkit::StepperWidget::progressed, this, [this](auto ratio)
	{
		emit stepsProgressed(ratio);
	});

	connect(&_stepperWidget, &toolkit::StepperWidget::ended, this, [this]
	{
		emit stepsEnded();
	});

	connect(&_stepperWidget, &toolkit::StepperWidget::cancelled, this, [this]
	{
		emit stepsCancelled();
	});
}

TemplateInfo::~TemplateInfo()
{
}

void TemplateInfo::resetView()
{
	_stepperWidget.clear();
}

void TemplateInfo::setTemplateInfo(std::uint32_t const templateIndex, core::type::TemplateInfo const& templateInfo)
{
	_stepperWidget.clear();
	for (auto i = 0u; i < templateInfo.stepInfoList.size(); ++i)
	{
		auto const& stepInfo = templateInfo.stepInfoList[i];
		_stepperWidget.addStep(new TemplateStep{ stepInfo, 
			_imageCacheManager.getDataDownloader(templateIndex, i), this }, stepInfo.duration.count());
	}

	// Update stacked widget current index
	_stackedWidget.setCurrentIndex(_stepperWidget.count() ? 1 : 0);
}

void TemplateInfo::startProcessing()
{
	_stepperWidget.startProcessing();
}

void TemplateInfo::stopProcessing()
{
	_stepperWidget.stopProcessing();
}

} // namespace firmwareUpdater::ui::view::workspace::templateInfo
