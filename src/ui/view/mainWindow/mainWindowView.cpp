#include "mainWindowView.hpp"

#include <QKeyEvent>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>

#include "core/controller/controller.hpp"
#include "core/controller/controllerSingle.hpp"

#include "ui/toolkit/qssHelper.hpp"

namespace firmwareUpdater::ui::view::mainWindow
{

MainWindowView::MainWindowView(QWidget* parent)
	: QMainWindow{ parent }
{
	_stackedWidget.addWidget(&_workspace);
	setCentralWidget(&_stackedWidget);

	_toolbar.hide();

	connect(&_workspace, &workspace::Workspace::modeSwitched, this, [this](bool isModeMulti)
	{
		_controller->onModeButtonClicked(isModeMulti);
	});
}

void MainWindowView::keyPressEvent(QKeyEvent* event)
{
	// Live reload QSS located in the project resource directory (for easy debug during dev)
	if (event->key() == Qt::Key_F1)
	{
		if (firmwareUpdater::ui::toolkit::qssHelper::loadProjectQSS())
			QMessageBox::information(this, "QSS", "main.qss has been reloaded.", QMessageBox::Yes);
	}
	// Live reload QSS located closed to the root of the application executable
	else if (event->key() == Qt::Key_F5)
	{
		if (firmwareUpdater::ui::toolkit::qssHelper::loadQSS(
			QCoreApplication::applicationDirPath() + QDir::separator() + "main.qss"))
			QMessageBox::information(this, "QSS", "main.qss has been reloaded.", QMessageBox::Yes);
	}
}

void MainWindowView::setController(firmwareUpdater::core::controller::Controller& controller)
{
	_controller = &controller;
	_workspace.setController(controller);
}

void MainWindowView::setPageType(core::controller::PageType const& pageType,
	bool const shouldExpand, bool const /*shouldDisplayToolbar*/)
{
	// Change page
	_workspace.setPageType(pageType == core::controller::PageType::MultiModeWorkspace);

	// Mode Multi
	if (shouldExpand)
	{
		auto const availableGeometry = QGuiApplication::screenAt(this->mapToGlobal(QPoint{ this->width() / 2, 0 }))->availableGeometry();
		auto const proportion = 0.95f;
		auto const newWidth = availableGeometry.width() * proportion;
		auto const newHeight= availableGeometry.height() * proportion;
		this->setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
		this->resize(newWidth, newHeight);
		this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(), availableGeometry));
	}
	// Mode Single
	else
	{
		auto const availableGeometry = QGuiApplication::screenAt(this->mapToGlobal(QPoint{ this->width() / 2, 0 }))->availableGeometry();
		auto const newWidth = availableGeometry.width() / 3.f;
		this->setFixedSize(newWidth, newWidth * 1.1f);
		this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(), availableGeometry));
	}

	// For now toolbar is hidden
	//if (shouldDisplayToolbar)
	//{
	//	_toolbar.setVisible(true);
	//	this->addToolBar(Qt::LeftToolBarArea, &_toolbar);
	//}
	//else
	//{
	//	this->removeToolBar(&_toolbar);
	//}
}

} // namespace firmwareUpdater::ui::view::mainWindow
