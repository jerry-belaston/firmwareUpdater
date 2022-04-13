#include "templateInfoView.hpp"
#include "templateStepView.hpp"	
#include "ui/toolkit/dataDownloader.hpp"
#include <unordered_map>
#include <QVBoxLayout>

namespace firmwareUpdater::ui::view::templateInfo
{

class TemplateInfoView::ImageCache
{
public:
	using StepId = std::uint32_t;
	std::unordered_map<StepId, std::unique_ptr<toolkit::DataDownloader>> _dataDownloaders;
};

TemplateInfoView::TemplateInfoView(QWidget* parent)
	: QWidget{ parent }
	, _imageCache{ std::make_unique<ImageCache>() }
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

TemplateInfoView::~TemplateInfoView()
{
}

void TemplateInfoView::setController(core::controller::Controller& controller)
{
	_controller = &controller;
}

void TemplateInfoView::setTemplateInfo(std::uint32_t const templateIndex, core::type::TemplateInfo const& templateInfo)
{
	QMetaObject::invokeMethod(this, [this, templateIndex, templateInfo]
	{
		_imageCache->_dataDownloaders.clear();
		_stepperWidget.clear();
		for (auto i = 0u; i < templateInfo.stepInfoList.size(); ++i)
		{
			auto const& stepInfo = templateInfo.stepInfoList[i];
			_imageCache->_dataDownloaders[i] = std::make_unique<toolkit::DataDownloader>(QString::fromStdString(stepInfo.imageUrl), this);
			_stepperWidget.addStep(new TemplateStepView{ stepInfo, *_imageCache->_dataDownloaders[i], this });
		}
	}, Qt::QueuedConnection);

}

} // namespace firmwareUpdater::ui::view::templateInfo
