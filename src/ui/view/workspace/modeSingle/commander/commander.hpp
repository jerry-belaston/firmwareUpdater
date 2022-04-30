#pragma once

#include <QFrame>
#include <QWidget>
#include <QPushButton>

#include "core/controller/viewInterface.hpp"

namespace firmwareUpdater::ui::view::workspace::modeSingle::commander
{

class Commander
	: public QFrame
{
	Q_OBJECT

public:
	explicit Commander(QWidget* parent = nullptr);

	// ViewInterface overrides
	void resetView();
	void setUploadButtonState(core::controller::workspace::UploadButtonState const uploadButtonState);

public:
	Q_SIGNAL void uploadButtonClicked();

private:
	QPushButton _uploadButton{ this };
};

} // namespace firmwareUpdater::ui::view::workspace::modeSingle::commander
