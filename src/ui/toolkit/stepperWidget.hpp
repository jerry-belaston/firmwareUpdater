#pragma once

#include <vector>

#include <QFrame>
#include <QWidget>
#include <QTabWidget>

namespace firmwareUpdater::ui::toolkit
{

class StepWidget : public QFrame
{
	Q_OBJECT

public:
	explicit StepWidget(QWidget* parent = nullptr);
	virtual void startProcessing() = 0;
	virtual void stopProcessing() = 0;
	Q_SIGNAL void next();
	Q_SIGNAL void previous();
};


class StepperWidget
	: public QFrame
{
	Q_OBJECT

public:
	explicit StepperWidget(QWidget* parent = nullptr);
	bool eventFilter(QObject* watched, QEvent* event) override;

	void startProcessing();
	void stopProcessing();
	void clear();
	void addStep(StepWidget* widget, std::uint32_t const duration);
	int count() const;

	Q_SIGNAL void progressed(double ratio);
	Q_SIGNAL void ended();
	Q_SIGNAL void cancelled();

private:
	QTabWidget _tabWidget{ this };
	std::vector<std::uint32_t> _durations{};
};

} // namespace firmwareUpdater::ui::toolkit
