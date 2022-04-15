#include "stepperWidget.hpp"

#include <QVBoxLayout>
#include <QTabBar>
#include <QMouseEvent>

namespace firmwareUpdater::ui::toolkit
{

StepWidget::StepWidget(QWidget* parent)
	: QWidget{ parent }
{
}

StepperWidget::StepperWidget(QWidget* parent)
	: QWidget{ parent }
{
	// Layout
	auto* layout = new QVBoxLayout{ this };
	layout->setContentsMargins(0, 0, 0, 0);

	// TabWidget
	_tabWidget.setTabBarAutoHide(true);
	_tabWidget.setStyleSheet(R"(
		QTabBar::tab {
			border: 1px solid rgba(255, 255, 255, 50);
			color: rgba(255, 255, 255, 50);
			font: 13pt bold;
			min-height: 50px;
			min-width: 100px;
			margin-left:5px;
			margin-right:5px;
		}

		QTabWidget::pane {
			border: none;
			margin:5px;
			margin-top:30px;
			background: rgb(161, 161, 161);
		}

		QTabBar::tab::selected {
			background: rgb(161, 161, 161);
			color: white;
		}
	)");

	// Intall event filter to lock tab selection
	_tabWidget.tabBar()->installEventFilter(this);
	layout->addWidget(&_tabWidget);
}

bool StepperWidget::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == _tabWidget.tabBar())
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			auto pos = dynamic_cast<QMouseEvent*>(event)->pos();
			auto index = _tabWidget.tabBar()->tabAt(pos);
			if (_tabWidget.widget(index) != this)
				return true; // cancell event
		}
	}
	return QWidget::eventFilter(watched, event);
}

void StepperWidget::clear()
{
	_tabWidget.clear();
}

void StepperWidget::addStep(StepWidget* widget)
{
	connect(widget, &StepWidget::next, this, [this]
	{
		if (_tabWidget.currentIndex() == _tabWidget.count() - 1)
			emit ended();
		else
			_tabWidget.setCurrentIndex(_tabWidget.currentIndex() + 1);
	});

	connect(widget, &StepWidget::previous, this, [this]
	{
		if (_tabWidget.currentIndex() == 0)
			emit cancelled();
		else
			_tabWidget.setCurrentIndex(_tabWidget.currentIndex() - 1);
	});

	_tabWidget.addTab(widget, "Step " + QString::number(_tabWidget.count() + 1));
}

} // namespace firmwareUpdater::ui::toolkit
