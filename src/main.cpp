#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QStyle>
#include <QStyleFactory>

#include "core/model/server.hpp"
#include "core/controller/controller.hpp"
#include "ui/view/main/mainView.hpp"

int main(int argc, char *argv[])
{
	// Create Qt application
	QApplication application(argc, argv);

	// Create model
	auto server = firmwareUpdater::core::model::Server::create();

	// Create view
	auto mainView = firmwareUpdater::ui::view::main::MainView{};
	mainView.setWindowTitle("Firmware Updater 1.0.2");
	auto const availableGeometry = QApplication::desktop()->availableGeometry(&mainView);
	auto const newWidth = availableGeometry.width() / 3.f;
	mainView.setFixedSize(newWidth, newWidth * 1.1f);
	mainView.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, mainView.size(), QGuiApplication::primaryScreen()->geometry()));

	// Create controller
	auto controller = firmwareUpdater::core::controller::Controller::create(*server);

	// Set view and start server model
	controller->setView(mainView);
	controller->setView(*mainView.getWelcomeView());
	controller->setView(*mainView.getTemplateBrowserView());
	controller->setView(*mainView.getTemplateInfoView());
	server->start();

	// Display view
	mainView.show();

	// Enter the QApplication main event loop and wait until exit is called
	return application.exec();
}