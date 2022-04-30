#pragma once

#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QStackedWidget>

#include "core/common/type.hpp"
#include "ui/toolkit/stepperWidget.hpp"

namespace firmwareUpdater::ui::view::workspace
{
	class ImageCacheManager;
}

namespace firmwareUpdater::ui::view::workspace::templateInfo
{

class TemplateInfo
	: public QFrame
{
	Q_OBJECT

public:
	explicit TemplateInfo(ImageCacheManager const& imageCacheManager, QWidget* parent = nullptr);
	virtual ~TemplateInfo();

	// ViewInterface overrides
	void resetView();
	void setTemplateInfo(std::uint32_t const templateIndex, core::type::TemplateInfo const& templateInfo);
	void startProcessing();
	void stopProcessing();

public:
	Q_SIGNAL void stepsProgressed(double ratio);
	Q_SIGNAL void stepsEnded();
	Q_SIGNAL void stepsCancelled();

private:
	ImageCacheManager const& _imageCacheManager;
	QStackedWidget _stackedWidget{ this };
	QLabel _emptyLabel{ &_stackedWidget };
	toolkit::StepperWidget _stepperWidget{ &_stackedWidget };

	// Widgets
};

} // namespace firmwareUpdater::ui::view::workspace::templateInfo
