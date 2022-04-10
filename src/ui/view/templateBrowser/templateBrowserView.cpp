#include "templateBrowserView.hpp"

#include <QVBoxLayout>

namespace firmwareUpdater::ui::view::templateList
{

TemplateBrowserView::TemplateBrowserView(QWidget* parent)
	: QWidget{ parent }
{
	// Layout
	auto* layout = new QVBoxLayout{ this };
	layout->setContentsMargins(30, 30, 30, 30);

	// ListWidget
	_templateListWidget.setStyleSheet(R"(
		QListWidget {
			border: none;
			font: 13pt bold;
			background-color: black
		}
		QListWidget::item {
			min-height: 50px;
			background: rgb(216, 216, 216);
			margin: 3px;
			color: rgb(94, 94, 94);
		}
		QListWidget:item:hover{
			background-color: rgb(161,161,161);
		}
		QListWidget:item:selected,
		QListWidget:item:selected:hover {
			background-color: rgb(100,100,100);
			color: black;
		}
	)");
	layout->addWidget(&_templateListWidget);
	connect(&_templateListWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* item)
	{
		if (item->isSelected())
		{
			// Send selected row to controller but reset it on UI
			// because when we comeback to this screen, no row 
			// must be selected.
			auto selectedRow = _templateListWidget.currentRow();
			_templateListWidget.setCurrentRow(-1);
			if (_controller)
				_controller->onListItemClicked(selectedRow);
		}
	});
}

void TemplateBrowserView::setController(core::controller::Controller& controller)
{
	_controller = &controller;
}

void TemplateBrowserView::setTemplateNameList(core::type::TemplateNameList const& templateNameList)
{
	QMetaObject::invokeMethod(this, [this, templateNameList]
	{
		_templateListWidget.clear();
		for (auto const& templateName : templateNameList)
		{
			auto* item = new QListWidgetItem{ &_templateListWidget };
			item->setTextAlignment(Qt::AlignCenter);
			item->setText(QString::fromStdString(templateName));
			_templateListWidget.addItem(item);
		}
	}, Qt::QueuedConnection);
}

} // namespace firmwareUpdater::ui::view::templateList
