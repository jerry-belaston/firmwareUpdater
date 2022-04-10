#include <QApplication>
#include <QDesktopWidget>
#include <QStyle>
#include <QStyleFactory>

#include "core/controller/controller.hpp"
#include "ui/view/main/mainView.hpp"

int main(int argc, char *argv[])
{
	// Create Qt application
	QApplication application(argc, argv);

	// Create view
	auto mainView = firmwareUpdater::ui::view::main::MainView{};
	mainView.setWindowTitle("Firmware Updater 1.0.0");
	auto const availableGeometry = QApplication::desktop()->availableGeometry(&mainView);
	auto const newWidth = availableGeometry.width() / 3.f;
	mainView.setFixedSize(newWidth, newWidth * 1.1f);
	mainView.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, mainView.size(), QApplication::desktop()->availableGeometry()));

	// Create controller
	auto controller = firmwareUpdater::core::controller::Controller::create();

	// Set view
	controller->setView(mainView);
	controller->setView(*mainView.getWelcomeView());
	controller->setView(*mainView.getTemplateBrowserView());
	controller->setView(*mainView.getTemplateInfoView());

	// Display view
	mainView.show();

	// Enter the QApplication main event loop and wait until exit is called
	return application.exec();
}