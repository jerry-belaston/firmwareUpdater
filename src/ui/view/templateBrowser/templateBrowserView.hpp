#pragma once

#include <QWidget>
#include <QListWidget>

#include "core/controller/viewInterface.hpp"

namespace firmwareUpdater::ui::view::templateList
{

class TemplateBrowserView
	: public QWidget
	, public core::controller::TemplateBrowserViewInterface
{
	Q_OBJECT

public:
	explicit TemplateBrowserView(QWidget* parent = nullptr);

	// ViewInterface overrides
	void setController(core::controller::Controller& controller) override;
	void setTemplateNameList(core::type::TemplateNameList const& templateNameList) override;

private:
	// Widgets
	QListWidget _templateListWidget{ this };

	// Controller
	core::controller::Controller* _controller{ nullptr };
};

} // namespace firmwareUpdater::ui::view::templateList
