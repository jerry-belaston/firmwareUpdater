#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QStyle>
#include <QStyleFactory>

#include "config.h"
#include "core/core.hpp"
#include "ui/view/mainWindow/mainWindowView.hpp"
#include "ui/toolkit/qssHelper.hpp"

int main(int argc, char *argv[])
{
	// Create Qt application
	QApplication application(argc, argv);
	auto const applicationVersion = 
		QString::number(FU_VERSIONMAJ) + "." +
		QString::number(FU_VERSIONMIN) + "." +
		QString::number(FU_VERSIONSUB);
	QCoreApplication::setApplicationVersion(applicationVersion);
	QCoreApplication::setApplicationName(FU_PRODUCTNAME);
	QCoreApplication::setOrganizationName(FU_COMPANYNAME);
	QCoreApplication::setOrganizationDomain(FU_URLABOUTINFO);

	// Set Stylesheet
	firmwareUpdater::ui::toolkit::qssHelper::loadProjectQSS();

	// Create view
	auto mainWindowView = firmwareUpdater::ui::view::mainWindow::MainWindowView{};
	mainWindowView.setWindowIcon(QIcon{ ":/logo.png" });
	mainWindowView.setWindowTitle(QString(FU_PRODUCTNAME) + " " + applicationVersion);
	auto availableGeometry = QGuiApplication::screenAt(mainWindowView.mapToGlobal(QPoint{ mainWindowView.width() / 2, 0 }))->availableGeometry();
	auto const newWidth = availableGeometry.width() / 3.f;
	mainWindowView.setFixedSize(newWidth, newWidth * 1.1f);
	mainWindowView.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, mainWindowView.size(), availableGeometry));

	// Create model
	auto core = firmwareUpdater::core::Core::create();

	// Set view and start server model
	core->getController().setView(mainWindowView);
	core->start();

	// Display view
	mainWindowView.show();

	// Enter the QApplication main event loop and wait until exit is called
	return application.exec();
}