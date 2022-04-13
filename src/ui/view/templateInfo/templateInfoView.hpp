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
	virtual ~TemplateInfoView();

	// ViewInterface overrides
	void setController(core::controller::Controller& controller) override;
	void prepareImageCache(core::type::TemplateInfoList const& templateInfoList) override;
	void setTemplateInfo(std::uint32_t const templateIndex, core::type::TemplateInfo const& templateInfo) override;

private:
	// Widgets
	toolkit::StepperWidget _stepperWidget{ this };

	// Controller
	core::controller::Controller* _controller{ nullptr };

	class ImageCache;
	std::unique_ptr<ImageCache> _imageCache{};
};

} // namespace firmwareUpdater::ui::view::templateInfo
