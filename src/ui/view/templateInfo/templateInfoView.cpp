#include "templateInfoView.hpp"
#include "templateStepView.hpp"	
#include "ui/toolkit/dataDownloader.hpp"
#include <map>
#include <QVBoxLayout>

namespace firmwareUpdater::ui::view::templateInfo
{

class TemplateInfoView::ImageCache
{
public:
	using Key = std::pair<std::uint32_t, std::uint32_t>;
	std::map<Key, std::unique_ptr<toolkit::DataDownloader>> _dataDownloaders;
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

void TemplateInfoView::prepareImageCache(core::type::TemplateInfoList const& templateInfoList)
{
	QMetaObject::invokeMethod(this, [this, templateInfoList]
	{
		_imageCache->_dataDownloaders.clear();
		for (auto i = 0u; i < templateInfoList.size(); ++i)
		{
			for (auto j = 0u; j < templateInfoList[i].stepInfoList.size(); ++j)
			{
				_imageCache->_dataDownloaders[{ i, j }] = std::make_unique<toolkit::DataDownloader>(
					QString::fromStdString(templateInfoList[i].stepInfoList[j].imageUrl));
			}
		}
	}, Qt::QueuedConnection);
}

void TemplateInfoView::setTemplateInfo(std::uint32_t const templateIndex, core::type::TemplateInfo const& templateInfo)
{
	QMetaObject::invokeMethod(this, [this, templateIndex, templateInfo]
	{
		_stepperWidget.clear();
		for (auto i = 0u; i < templateInfo.stepInfoList.size(); ++i)
		{
			auto const& stepInfo = templateInfo.stepInfoList[i];
			_stepperWidget.addStep(new TemplateStepView{ stepInfo, 
				*_imageCache->_dataDownloaders[{ templateIndex, i }], this });
		}
	}, Qt::QueuedConnection);

}

} // namespace firmwareUpdater::ui::view::templateInfo
