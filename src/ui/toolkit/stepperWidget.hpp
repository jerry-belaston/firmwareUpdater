#pragma once

#include <QWidget>
#include <QTabWidget>

namespace firmwareUpdater::ui::toolkit
{

class StepWidget : public QWidget
{
	Q_OBJECT

public:
	explicit StepWidget(QWidget* parent = nullptr);
	Q_SIGNAL void next();
	Q_SIGNAL void previous();
};


class StepperWidget
	: public QWidget
{
	Q_OBJECT

public:
	explicit StepperWidget(QWidget* parent = nullptr);
	bool eventFilter(QObject* watched, QEvent* event) override;

	void clear();
	void addStep(StepWidget* widget);
	Q_SIGNAL void cancelled();
	Q_SIGNAL void ended();

private:
	QTabWidget _tabWidget{ this };
};

} // namespace firmwareUpdater::ui::toolkit
