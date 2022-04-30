#include "stepperWidget.hpp"

#include <QVBoxLayout>
#include <QTabBar>
#include <QMouseEvent>
#include <cassert>
#include <numeric>

namespace firmwareUpdater::ui::toolkit
{

StepWidget::StepWidget(QWidget* parent)
	: QFrame{ parent }
{
}

StepperWidget::StepperWidget(QWidget* parent)
	: QFrame{ parent }
{
	// Layout
	auto* layout = new QVBoxLayout{ this };
	layout->setContentsMargins(0, 0, 0, 0);

	// TabWidget
	_tabWidget.setTabBarAutoHide(true);

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

void StepperWidget::startProcessing()
{
	auto const firstTabWidgetIndex{ 0 };
	auto* stepWidget = static_cast<StepWidget*>(_tabWidget.widget(firstTabWidgetIndex));
	assert(stepWidget && "StepWidget must always be valid here");
	stepWidget->startProcessing();
	_tabWidget.setCurrentIndex(firstTabWidgetIndex);
}

void StepperWidget::stopProcessing()
{
	for (auto i = 0; i < _tabWidget.count(); ++i)
	{
		auto* stepWidget = static_cast<StepWidget*>(_tabWidget.widget(i));
		stepWidget->stopProcessing();
	}
}

void StepperWidget::clear()
{
	_durations.clear();
	_tabWidget.clear();
}

void StepperWidget::addStep(StepWidget* widget, std::uint32_t const duration)
{
	auto computeAndEmitProgress = [this](auto const newTabIndex)
	{
		auto const durationsTotal = std::accumulate(std::begin(_durations), std::end(_durations), 0.0);
		auto const durationsDone = std::accumulate(std::begin(_durations), std::begin(_durations) + newTabIndex, 0.0);
		auto const ratio = durationsDone / durationsTotal;
		emit progressed(ratio);
	};

	connect(widget, &StepWidget::next, this, [this, computeAndEmitProgress]
	{
		if (_tabWidget.currentIndex() == _tabWidget.count() - 1)
			emit ended();
		else
		{
			auto const newTabWidgetIndex{ _tabWidget.currentIndex() + 1 };
			auto* stepWidget = static_cast<StepWidget*>(_tabWidget.widget(newTabWidgetIndex));
			stepWidget->startProcessing();
			_tabWidget.setCurrentIndex(newTabWidgetIndex);
			computeAndEmitProgress(newTabWidgetIndex);
		}
	});

	connect(widget, &StepWidget::previous, this, [this, computeAndEmitProgress]
	{
		if (_tabWidget.currentIndex() == 0)
			emit cancelled();
		else
		{
			auto const newTabWidgetIndex{ _tabWidget.currentIndex() - 1 };
			auto* stepWidget = static_cast<StepWidget*>(_tabWidget.widget(newTabWidgetIndex));
			stepWidget->startProcessing();
			_tabWidget.setCurrentIndex(newTabWidgetIndex);
			computeAndEmitProgress(newTabWidgetIndex);
		}
	});

	_tabWidget.addTab(widget, "Step " + QString::number(_tabWidget.count() + 1));
	_durations.push_back(duration);
}

int StepperWidget::count() const
{
	return _tabWidget.count();
}

} // namespace firmwareUpdater::ui::toolkit
