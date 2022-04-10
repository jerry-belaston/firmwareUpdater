#pragma once

#include <Qwidget>
#include <QPushButton>

#include "core/controller/viewInterface.hpp"

namespace firmwareUpdater::ui::view::welcome
{

class WelcomeView
	: public QWidget
	, public core::controller::WelcomeViewInterface
{
	Q_OBJECT

public:
	explicit WelcomeView(QWidget* parent = nullptr);

	// ViewInterface overrides
	void setController(core::controller::Controller& controller) override;
	virtual void setButtonEnabled(bool isEnabled) override;

private:
	// Controller
	core::controller::Controller* _controller{ nullptr };
	QPushButton _startButton{ "Start", this };

};

} // namespace firmwareUpdater::ui::view::welcome
